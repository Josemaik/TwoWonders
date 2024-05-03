#pragma once

#include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/quaternion.hpp>

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
// #include <GL/glew.h>
// #include <GLFW/glfw3.h>

struct KeyPosition
{
    glm::vec3 position{};
    float timeStamp{};
};

struct KeyRotation
{
    glm::quat orientation{};
    float timeStamp{};
};

struct KeyScale
{
    glm::vec3 scale{};
    float timeStamp{};
};


struct bone
{
    private:
        std::vector<KeyPosition> m_Positions{};
        std::vector<KeyRotation> m_Rotations{};
        std::vector<KeyScale> m_Scales{};
        int m_numPositions{};
        int m_numRotations{};
        int m_numScalings{};

        glm::mat4 m_LocalTransform{};
        std::string m_Name{};
        int m_ID{};
    public:
        bone(const std::string& name,int ID, const aiNodeAnim* channel)
        : 
        m_Name(name),
        m_ID(ID),
        m_LocalTransform(1.0f)
        {
            m_numPositions = channel->mNumPositionKeys;
            for(int posindex = 0; posindex < m_numPositions; ++posindex){
                aiVector3D aiPosition = channel->mPositionKeys[posindex].mValue;
                float timeStamp = channel->mPositionKeys[posindex].mTime;
                KeyPosition data{};
                data.position = glm::make_vec3(aiPosition);
                data.timeStamp = timeStamp;
                m_Positions.push_back(data);
            }
            m_numRotations = channel->mNumRotationKeys;
            for(int rotationindex = 0; rotationindex < m_numRotations; ++rotationindex){
                aiQuaternion aiOrientation = channel->mRotationKeys[rotationindex].mValue;
                float timeStamp = channel->mRotationKeys[rotationindex].mTime;
                KeyRotation data{};
                data.orientation = glm::make_quat(aiOrientation);
                data.timeStamp = timeStamp;
                m_Rotations.push_back(data);
            }
            m_numScalings = channel->mNumScalingKeys;
            for(int scalindex = 0; scalindex < m_numScalings; ++scalindex){
                aiVector3D scale = channel->mScalingKeys[scalindex].mValue;
                float timeStamp = channel->mScalingKeys[scalindex].mTime;
                KeyScale data{};
                data.scale = glm::make_vec3(scale);
                data.timeStamp = timeStamp;
                m_Scales.push_back(data);
            }
        }

        void Update(float animationTime);

};
