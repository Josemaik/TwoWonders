#pragma once

#include "bone.hpp"
#include <glm/gtc/type_ptr.hpp>
// #include "../components/entity_model.hpp"

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

    animation(const std::string& animationPath)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
        assert(scene && scene->mRootNode);
        auto animation = scene->mAnimations[0];
        m_Duration = static_cast<float>(animation->mDuration);
        m_TicksPerSecond = static_cast<float>(animation->mTicksPerSecond);
        ReadHeirarchyData(m_RootNode, scene->mRootNode);
        FillBones(animation);
        // FindBoneReadMissingBones(animation, *model);
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

    inline float GetDuration() { return m_Duration; }

    inline const AssimpNodeData& GetRootNode() { return m_RootNode; }

    // inline const std::map<std::string, typename ModelType::BoneInfo>& GetBoneIDMap()
    // {
    //     return m_BoneInfoMap;
    // }

    inline const std::vector<bone> GetBonesVector(){ return m_Bones; }
    //set id
    // void setID(int id,int index){
    //     m_Bones[index].SetBoneID(id);
    // }
private:
    void FillBones(const aiAnimation* animation){
        int size = animation->mNumChannels;
        for (int i = 0; i < size; i++)
        {
            auto channel = animation->mChannels[i];
            std::string boneName = channel->mNodeName.data;
            m_Bones.push_back(bone(channel->mNodeName.data,
                1, channel));
        }
    }
    // template<typename ModelType>
    // void ReadMissingBones(const aiAnimation* animation, ModelType& model) {
    //     int size = animation->mNumChannels;

    //     auto& boneInfoMap = model.getBoneInfoMap();
    //     int& boneCount = model.getBoneCount();

    //     for (int i = 0; i < size; i++)
    //     {
    //         auto channel = animation->mChannels[i];
    //         std::string boneName = channel->mNodeName.data;

    //         if (boneInfoMap.find(boneName) == boneInfoMap.end())
    //         {
    //             boneInfoMap[boneName].id = boneCount;
    //             boneCount++;
    //         }
    //         m_Bones.push_back(bone(channel->mNodeName.data,
    //             boneInfoMap[channel->mNodeName.data].id, channel));
    //     }

    //     m_BoneInfoMap = boneInfoMap;
    // }

    void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src) {
        assert(src);

        dest.name = src->mName.data;
        dest.transformation = glm::make_mat4(&src->mTransformation.a1);
        dest.childrenCount = src->mNumChildren;

        for (uint i = 0; i < src->mNumChildren; i++)
        {
            AssimpNodeData newData;
            ReadHeirarchyData(newData, src->mChildren[i]);
            dest.children.push_back(newData);
        }
    }

    float m_Duration{};
    float m_TicksPerSecond{};
    std::vector<bone> m_Bones{};
    AssimpNodeData m_RootNode{};
    // std::map<std::string, typename ModelType::BoneInfo> m_BoneInfoMap{};
};
