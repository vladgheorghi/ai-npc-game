#include "animation.h"


namespace ai_npc {
    void Animation::BoneTransform(Mesh* mesh, float timeInSeconds)
    {
        glm::mat4 Identity = glm::mat4(1.0f);

        // Compute the duration of the animation
        float ticksPerSecond = mesh->anim[0]->mTicksPerSecond != 0 ? mesh->anim[0]->mTicksPerSecond : 25.0f;
        float timeInTicks = timeInSeconds * ticksPerSecond;
        float animationTime = fmod(timeInTicks, mesh->anim[0]->mDuration);

        // Compute the final transformations for each bone at the current time stamp
        // starting from the root node
        // TODO (student): Uncomment this to test the animation
        ReadNodeHierarchy(mesh, animationTime, mesh->rootNode, Identity, mesh->anim);
    }

    void Animation::ReadNodeHierarchy(Mesh* mesh, float animationTime, const aiNode* pNode, const glm::mat4& parentTransform, aiAnimation** anim)
    {
        std::string NodeName(pNode->mName.data);

        // Our model has only one animation, which is stored in anim[0].
        // If we had more animations, we would need to select what animation we want
        const aiAnimation* pAnimation = anim[0];

        glm::mat4 nodeTransformation(mesh->ConvertMatrix(pNode->mTransformation));

        const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

        if (pNodeAnim) {
            // Interpolate the scaling and generate the scaling transformation matrix
            aiVector3D Scaling;
            ComputeInterpolatedScaling(Scaling, animationTime, pNodeAnim);
            glm::mat4 ScalingM = glm::scale(glm::mat4(1.0f), glm::vec3(Scaling.x, Scaling.y, Scaling.z));

            // Interpolate the rotation and generate the rotation transformation matrix
            aiQuaternion RotationQ;
            ComputeInterpolatedRotation(RotationQ, animationTime, pNodeAnim);
            glm::quat rotation(RotationQ.w, RotationQ.x, RotationQ.y, RotationQ.z);
            glm::mat4 RotationM = glm::toMat4(rotation);

            // Interpolate the translation and generate the translation transformation matrix
            aiVector3D Translation;
            ComputeInterpolatedPosition(Translation, animationTime, pNodeAnim);
            glm::mat4 TranslationM = glm::translate(glm::mat4(1.0f), glm::vec3(Translation.x, Translation.y, Translation.z));

            // Combine the above transformations
            nodeTransformation = TranslationM * RotationM * ScalingM;
        }

        // TODO (student): Apply the parent transformation to the current transformation
        glm::mat4 GlobalTransformation = parentTransform * nodeTransformation;

        if (mesh->m_BoneMapping.find(NodeName) != mesh->m_BoneMapping.end()) {
            // Bring the vertices from their local space position into their node space.
            // Multiply the result with the combined transformations of all the node parents plus the current transformation.
            // Bring the result back into local space.

            unsigned int BoneIndex = mesh->m_BoneMapping[NodeName];
            mesh->m_BoneInfo[BoneIndex].finalTransformation = mesh->m_GlobalInverseTransform * GlobalTransformation *
                mesh->m_BoneInfo[BoneIndex].boneOffset;
        }

        // Compute the transformations of the children of the current node
        for (unsigned int i = 0; i < pNode->mNumChildren; i++) {
            ReadNodeHierarchy(mesh, animationTime, pNode->mChildren[i], GlobalTransformation, anim);
        }
    }

    const aiNodeAnim* Animation::FindNodeAnim(const aiAnimation* pAnimation, const std::string NodeName)
    {
        for (unsigned int i = 0; i < pAnimation->mNumChannels; i++) {
            const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

            if (std::string(pNodeAnim->mNodeName.data) == NodeName) {
                return pNodeAnim;
            }
        }

        return NULL;
    }

    unsigned int Animation::FindRotation(float animationTime, const aiNodeAnim* pNodeAnim)
    {
        assert(pNodeAnim->mNumRotationKeys > 0);

        // TODO (student): Inside the aiNodeAnim variable we have the mRotationKeys array which stores
        // the rotation transformation of the vertex at different time stamps. The data is sorted. Find
        // the last rotation transformation which is at a lower time stamp so that we can compute the
        // interpolation of the rotation transformation of the bone. Return the index.

        for (unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
            // Return the last index where the next keyframe hasn't started yet
            if (animationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime)
                return i;
        }

        // animationTime is at or beyond the last keyframe — return the second-to-last
        // index so interpolation blends between the last two keyframes rather than crashing.
        return pNodeAnim->mNumRotationKeys - 2;
    }

    void Animation::ComputeInterpolatedRotation(aiQuaternion& out, float animationTime, const aiNodeAnim* pNodeAnim)
    {
        if (pNodeAnim->mNumRotationKeys == 1) {
            out = pNodeAnim->mRotationKeys[0].mValue;
            return;
        }

        // Find the time stamps at which we should interpolate
        unsigned int rotationIndex = FindRotation(animationTime, pNodeAnim);
        unsigned int nextRotationIndex = (rotationIndex + 1);
        assert(nextRotationIndex < pNodeAnim->mNumRotationKeys);

        // TODO (student): Compute the duration between the two selected time stamps
        // HINT! An entry from the mRotationKeys array has an mTime field
        float deltaTime = (float)(pNodeAnim->mRotationKeys[nextRotationIndex].mTime - pNodeAnim->mRotationKeys[rotationIndex].mTime);

        // Compute the factor of interpolation for a frame
        float factor = (animationTime - (float)pNodeAnim->mRotationKeys[rotationIndex].mTime) / deltaTime;
        factor = glm::clamp(factor, 0.0f, 1.0f);

        // TODO (student): Compute the final rotation factor by interpolating the values from the two selected key frames
        // HINT! Use the Interpolate method from the aiQuaternion class! Don't forget to normalize the result!
        // Save the result in the out variable
        aiQuaternion start = pNodeAnim->mRotationKeys[rotationIndex].mValue;
        aiQuaternion end = pNodeAnim->mRotationKeys[nextRotationIndex].mValue;
        aiQuaternion::Interpolate(out, start, end, factor); // slerp
        out.Normalize();
    }

    unsigned int Animation::FindScaling(float animationTime, const aiNodeAnim* pNodeAnim)
    {
        assert(pNodeAnim->mNumScalingKeys > 0);

        // TODO (student): Inside the aiNodeAnim variable we have the mScalingKeys array which stores
        // the scaling transformation of the vertex at different time stamps. The data is sorted. Find
        // the last scale transformation which is at a lower time stamp so that we can compute the
        // interpolation of the scaling transformation of the bone. Return the index.

        for (unsigned int i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
            if (animationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime)
                return i;
        }

        // animationTime is at or beyond the last keyframe — return the second-to-last
        // index so interpolation blends between the last two keyframes rather than crashing.
        return pNodeAnim->mNumScalingKeys - 2;
    }


    void Animation::ComputeInterpolatedScaling(aiVector3D& out, float animationTime, const aiNodeAnim* pNodeAnim)
    {
        if (pNodeAnim->mNumScalingKeys == 1) {
            out = pNodeAnim->mScalingKeys[0].mValue;
            return;
        }

        // Find the time stamps at which we should interpolate
        unsigned int scalingIndex = FindScaling(animationTime, pNodeAnim);
        unsigned int nextScalingIndex = (scalingIndex + 1);
        assert(nextScalingIndex < pNodeAnim->mNumScalingKeys);

        // TODO (student): Compute the duration between the two selected time stamps
        // HINT! An entry from the mScalingKeys array has an mTime field
        float deltaTime = (float)(pNodeAnim->mScalingKeys[nextScalingIndex].mTime - pNodeAnim->mScalingKeys[scalingIndex].mTime);

        // Compute the factor of interpolation for a frame
        float factor = (animationTime - (float)pNodeAnim->mScalingKeys[scalingIndex].mTime) / deltaTime;
        factor = glm::clamp(factor, 0.0f, 1.0f);

        // TODO (student): Compute the final scale factor by interpolating the values from the two selected key frames
        // Save the result in the out variable

        aiVector3D start = pNodeAnim->mScalingKeys[scalingIndex].mValue;
        aiVector3D end = pNodeAnim->mScalingKeys[nextScalingIndex].mValue;
        out = start + factor * (end - start); // linear blend
    }

    unsigned int Animation::FindPosition(float animationTime, const aiNodeAnim* pNodeAnim)
    {
        assert(pNodeAnim->mNumPositionKeys > 0);

        // TODO (student): Inside the aiNodeAnim variable we have the mPositionKeys array which stores
        // the translation transformation of the vertex at different time stamps. The data is sorted. Find
        // the last translation transformation which is at a lower time stamp so that we can compute the
        // interpolation of the translation transformation of the bone. Return the index.
        for (unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
            if (animationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime)
                return i;
        }

        // animationTime is at or beyond the last keyframe — return the second-to-last
        // index so interpolation blends between the last two keyframes rather than crashing.
        return pNodeAnim->mNumPositionKeys - 2;
    }

    void Animation::ComputeInterpolatedPosition(aiVector3D& out, float animationTime, const aiNodeAnim* pNodeAnim)
    {
        if (pNodeAnim->mNumPositionKeys == 1) {
            out = pNodeAnim->mPositionKeys[0].mValue;
            return;
        }

        // Find the time stamps at which we should interpolate
        unsigned int positionIndex = FindPosition(animationTime, pNodeAnim);
        unsigned int nextPositionIndex = (positionIndex + 1);
        assert(nextPositionIndex < pNodeAnim->mNumPositionKeys);

        // TODO (student): Compute the duration between the two selected time stamps
        // HINT! An entry from the mPositionKeys array has an mTime field
        float deltaTime = (float)(pNodeAnim->mPositionKeys[nextPositionIndex].mTime - pNodeAnim->mPositionKeys[positionIndex].mTime);

        // Compute the factor of interpolation for a frame
        float factor = (animationTime - (float)pNodeAnim->mPositionKeys[positionIndex].mTime) / deltaTime;
        factor = glm::clamp(factor, 0.0f, 1.0f);

        // TODO (student): Compute the final translation factor by interpolating the values from the two selected key frames
        // Save the result in the out variable

        aiVector3D start = pNodeAnim->mPositionKeys[positionIndex].mValue;
        aiVector3D end = pNodeAnim->mPositionKeys[nextPositionIndex].mValue;
        out = start + factor * (end - start); // linear blend
    }
}
