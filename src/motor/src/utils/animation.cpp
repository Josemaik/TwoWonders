#include "animation.hpp"

void animation::ReadMissingBones(const aiAnimation* animation, DarkMoon::Model& model){
    int size = animation->mNumChannels;

    auto& boneInfoMap = model.getboneInfoMap();//getting m_BoneInfoMap from Model class
    int& boneCount = model.getBoneCount(); //getting the m_BoneCounter from Model class

    //reading channels(bones engaged in an animation and their keyframes)
    for (int i = 0; i < size; i++)
    {
        auto channel = animation->mChannels[i];
        std::string boneName = channel->mNodeName.data;

        if (boneInfoMap.find(boneName) == boneInfoMap.end())
        {
            boneInfoMap[boneName].id = boneCount;
            boneCount++;
        }
        m_Bones.push_back(bone(channel->mNodeName.data,
            boneInfoMap[channel->mNodeName.data].id, channel));
    }

    m_BoneInfoMap = boneInfoMap;
}

void animation::ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src){
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