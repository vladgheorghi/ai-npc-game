#pragma once

#include "core/gpu/mesh.h"
#include "core/gpu/vertex_bone_data.h"

namespace ai_npc
{
    class Animation
    {
        // class methods
    public:
        static void BoneTransform(Mesh* mesh, float timeInSeconds);
        static void BoneTransform(Mesh* mesh, float timeInSeconds, std::vector<BoneInfo>& outBones);
    private:
        static void ReadNodeHierarchy(Mesh* mesh, float animationTime, const aiNode* pNode, const glm::mat4& parentTransform, aiAnimation** anim, std::vector<BoneInfo>& outBones);
        static const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const std::string nodeName);
        static unsigned int FindRotation(float animationTime, const aiNodeAnim* pNodeAnim);
        static void ComputeInterpolatedRotation(aiQuaternion& Out, float animationTime, const aiNodeAnim* pNodeAnim);
        static unsigned int FindScaling(float animationTime, const aiNodeAnim* pNodeAnim);
        static void ComputeInterpolatedScaling(aiVector3D& Out, float animationTime, const aiNodeAnim* pNodeAnim);
        static unsigned int FindPosition(float animationTime, const aiNodeAnim* pNodeAnim);
        static void ComputeInterpolatedPosition(aiVector3D& Out, float animationTime, const aiNodeAnim* pNodeAnim);
    protected:
    };
}
