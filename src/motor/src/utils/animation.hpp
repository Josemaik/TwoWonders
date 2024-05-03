#pragma once

#include "bone.hpp"
#include "../components/entity_model.hpp"

struct AssimpNodeData
{
    glm::mat4 transformation{};
    std::string name{};
    int childrenCount{};
    std::vector<AssimpNodeData> children{};
};

struct animation
{
    public:
        animation() = default;

        animation(const std::string& animationPath, DarkMoon::Model* model)
        {
                Assimp::Importer importer;
                const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
                assert(scene && scene->mRootNode);
                auto animation = scene->mAnimations[0];
                m_Duration = static_cast<float>(animation->mDuration);
                m_TicksPerSecond = static_cast<float>(animation->mTicksPerSecond);
                ReadHeirarchyData(m_RootNode, scene->mRootNode);
                ReadMissingBones(animation, *model);
        }

        ~animation()
        {
        }

        bone* FindBone(const std::string& name)
        {
            auto iter = std::find_if(m_Bones.begin(), m_Bones.end(),
                [&](const bone& Bone)
                {
                    return Bone.GetBoneName() == name;
                }
            );
            if (iter == m_Bones.end()) return nullptr;
            else return &(*iter);
        }

        
        inline float GetTicksPerSecond() { return m_TicksPerSecond; }

        inline float GetDuration() { return m_Duration;}

        inline const AssimpNodeData& GetRootNode() { return m_RootNode; }

        inline const std::map<std::string,DarkMoon::Model::BoneInfo>& GetBoneIDMap() 
        { 
            return m_BoneInfoMap;
        }
    private:

        void ReadMissingBones(const aiAnimation* animation, DarkMoon::Model& model);
        void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src);

        float m_Duration{};
        float m_TicksPerSecond{};
        std::vector<bone> m_Bones{};
        AssimpNodeData m_RootNode{};
        std::map<std::string, DarkMoon::Model::BoneInfo> m_BoneInfoMap{};

};

