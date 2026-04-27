#pragma once

#include "components/simple_scene.h"

namespace ai_npc
{
    class Animation
    {
        // class methods
    public:
        static void BoneTransform(Mesh* mesh, float timeInSeconds);
    private:
        static void ReadNodeHierarchy(Mesh* mesh, float animationTime, const aiNode* pNode, const glm::mat4& parentTransform, aiAnimation** anim);
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
