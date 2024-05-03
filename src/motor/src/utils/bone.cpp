#include "bone.hpp"

float bone::GetScaleFactor(float lastTimeStamp, float nexttimeStamp, float animationTime){
    float scaleFactor = 0.0f;
    float midwayLength = animationTime - lastTimeStamp;
    float framesDiff = nexttimeStamp - lastTimeStamp;
    scaleFactor = midwayLength / framesDiff;
    return scaleFactor;
}

int bone::GetPositionIndex(float animationTime){
    for(int i = 0; i < m_numPositions;i++){
        if(animationTime < m_Positions[i + 1].timeStamp)
            return i;
    }
    assert(0);
}

int bone::GetRotationIndex(float animationTime){
    for(int i = 0; i < m_numRotations;i++){
        if(animationTime < m_Rotations[i + 1].timeStamp)
            return i;
    }
    assert(0);
}

int bone::GetScaleIndex(float animationTime){
    for(int i = 0; i < m_numScalings;i++){
        if(animationTime < m_Scales[i + 1].timeStamp)
            return i;
    }
    assert(0);
}

glm::mat4 bone::InterpolatePosition(float animationTime){
    if(m_numPositions == 1)
        return glm::translate(glm::mat4(1.0f),m_Positions[0].position);
    
    int p0index = GetPositionIndex(animationTime);
    int p1index = p0index + 1;

    float scaleFactor = GetScaleFactor(m_Positions[p0index].timeStamp,
    m_Positions[p1index].timeStamp, animationTime);

    glm::vec3 finalPos = glm::mix(m_Positions[p0index].position,
    m_Positions[p1index].position, scaleFactor);

    return glm::translate(glm::mat4(1.0f), finalPos);
}

glm::mat4 bone::InterpolateRotation(float animationTime){
    if(m_numRotations == 1){
        glm::quat rotation = glm::normalize(m_Rotations[0].orientation);
        return glm::mat4_cast(rotation);
    }
    
    int p0index = GetRotationIndex(animationTime);
    int p1index = p0index + 1;

    float scaleFactor = GetScaleFactor(m_Rotations[p0index].timeStamp,
    m_Rotations[p1index].timeStamp, animationTime);

    glm::quat finalRotation = glm::slerp(m_Rotations[p0index].orientation,
    m_Rotations[p1index].orientation, scaleFactor);

    finalRotation = glm::normalize(finalRotation);

    return glm::mat4_cast(finalRotation);
}

glm::mat4 bone::InterpolateScale(float aniamtionTime){
    if(m_numScalings == 1)
        return glm::scale(glm::mat4(1.0f), m_Scales[0].scale);
    
    int p0index = GetScaleIndex(aniamtionTime);
    int p1index = p0index + 1;
    float scaleFactor = GetScaleFactor(m_Scales[p0index].timeStamp,
    m_Scales[p1index].timeStamp, aniamtionTime);
    glm::vec3 finalScale = glm::mix(m_Scales[p0index].scale,
    m_Scales[p1index].scale, scaleFactor);
    return glm::scale(glm::mat4(1.0f), finalScale);
}

void bone::Update(float animationTime){
    glm::mat4 translation = InterpolatePosition(animationTime);
    glm::mat4 rotation = InterpolateRotation(animationTime);
    glm::mat4 scale = InterpolateScale(animationTime);
    m_LocalTransform = translation * rotation * scale;
}