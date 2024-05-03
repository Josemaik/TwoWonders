#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <vector>
#include <string>
#include <assert.h>

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

        float GetScaleFactor(float lastTimeStamp, float nexttimeStamp, float animationTime);
        glm::mat4 InterpolatePosition(float animationTime);
        glm::mat4 InterpolateRotation(float animationTime);
        glm::mat4 InterpolateScale(float animationTime);

    public:
        bone(const std::string& name,int ID, const aiNodeAnim* channel)
        : 
        m_LocalTransform(1.0f),
        m_Name(name),
        m_ID(ID)
        {
            m_numPositions = channel->mNumPositionKeys;
            for(int posindex = 0; posindex < m_numPositions; ++posindex){
                aiVector3D aiPosition = channel->mPositionKeys[posindex].mValue;
                float timeStamp = static_cast<float>(channel->mPositionKeys[posindex].mTime);
                KeyPosition data{};
                data.position = glm::vec3(aiPosition.x,aiPosition.y,aiPosition.z);
                data.timeStamp = timeStamp;
                m_Positions.push_back(data);
            }
            m_numRotations = channel->mNumRotationKeys;
            for(int rotationindex = 0; rotationindex < m_numRotations; ++rotationindex){
                aiQuaternion aiOrientation = channel->mRotationKeys[rotationindex].mValue;
                float timeStamp = static_cast<float>(channel->mRotationKeys[rotationindex].mTime);
                KeyRotation data{};
                data.orientation = glm::quat(aiOrientation.w,aiOrientation.x,aiOrientation.y,aiOrientation.z);
                data.timeStamp = timeStamp;
                m_Rotations.push_back(data);
            }
            m_numScalings = channel->mNumScalingKeys;
            for(int scalindex = 0; scalindex < m_numScalings; ++scalindex){
                aiVector3D scale = channel->mScalingKeys[scalindex].mValue;
                float timeStamp = static_cast<float>(channel->mScalingKeys[scalindex].mTime);
                KeyScale data{};
                data.scale = glm::vec3(scale.x,scale.y,scale.z);
                data.timeStamp = timeStamp;
                m_Scales.push_back(data);
            }
        }

        void Update(float animationTime);
        std::string GetBoneName() const {return m_Name; }
        int GetBoneID() { return m_ID; }

        int GetPositionIndex(float animationTime);
        int GetRotationIndex(float animationTime);
        int GetScaleIndex(float animationTime);
};
