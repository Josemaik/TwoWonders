#pragma once

#include "../utils/animation.hpp"


struct AnimationManager
{
    AnimationManager()
    {
        m_CurrentTime = 0.0f;
        interpolating = false;
        haltTime = 0.0;
        interTime = 0.0;

        m_FinalBoneMatrices.reserve(100);

        for (int i = 0; i < 100; i++)
            m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
    }

    void UpdateAnimation(float dt)
    {
        if (m_CurrentAnimation) {
            m_CurrentTime = myFmod(m_CurrentTime + m_CurrentAnimation->getTicksPerSecond() * dt, m_CurrentAnimation->getDuration());

            float transitionTime = m_CurrentAnimation->getTicksPerSecond() * 0.2f;
            if (interpolating && interTime <= transitionTime) {
                interTime += m_CurrentAnimation->getTicksPerSecond() * dt;
                calculateBoneTransition(m_CurrentAnimation->getRootNode(), glm::mat4(1.0f), m_CurrentAnimation, m_NextAnimation, haltTime, interTime, transitionTime);
                return;
            }
            else if (interpolating) {
                if (m_QueueAnimation) {
                    m_CurrentAnimation = m_NextAnimation;
                    haltTime = 0.0f;
                    m_NextAnimation = m_QueueAnimation;
                    m_QueueAnimation = nullptr;
                    m_CurrentTime = 0.0f;
                    interTime = 0.0;
                    return;
                }

                interpolating = false;
                m_CurrentAnimation = m_NextAnimation;
                m_CurrentTime = 0.0;
                interTime = 0.0;
            }

            calculateBoneTransform(m_CurrentAnimation->getRootNode(), glm::mat4(1.0f), m_CurrentAnimation, m_CurrentTime);
        }
    }

    void PlayAnimation(Animation* pAnimation)
    {
        m_CurrentAnimation = pAnimation;
        m_CurrentTime = 0.0f;
    }

    void calculateBoneTransition(const AssimpNodeData* curNode, glm::mat4 parentTransform, Animation* prevAnimation, Animation* nextAnimation, float haltTime, float currentTime, float transitionTime)
    {
        std::string nodeName = curNode->name;
        glm::mat4 transform = curNode->transformation;

        Bone* prevBone = prevAnimation->findBone(nodeName);
        Bone* nextBone = nextAnimation->findBone(nodeName);

        if (prevBone && nextBone)
        {
            KeyPosition prevPos = prevBone->getPositions(haltTime);
            KeyRotation prevRot = prevBone->getRotations(haltTime);
            KeyScale prevScl = prevBone->getScalings(haltTime);

            KeyPosition nextPos = nextBone->getPositions(0.0f);
            KeyRotation nextRot = nextBone->getRotations(0.0f);
            KeyScale nextScl = nextBone->getScalings(0.0f);

            prevPos.timeStamp = 0.0f;
            prevRot.timeStamp = 0.0f;
            prevScl.timeStamp = 0.0f;

            nextPos.timeStamp = transitionTime;
            nextRot.timeStamp = transitionTime;
            nextScl.timeStamp = transitionTime;

            glm::mat4 p = Bone::interpolatePosition(currentTime, prevPos, nextPos);
            glm::mat4 r = Bone::interpolateRotation(currentTime, prevRot, nextRot);
            glm::mat4 s = Bone::interpolateScaling(currentTime, prevScl, nextScl);

            transform = p * r * s;
        }

        glm::mat4 globalTransformation = parentTransform * transform;

        auto boneProps = nextAnimation->getBoneProps();
        for (unsigned int i = 0; i < boneProps.size(); i++) {
            if (boneProps[i].name == nodeName) {
                glm::mat4 offset = boneProps[i].offset;
                m_FinalBoneMatrices[i] = globalTransformation * offset;
                break;
            }
        }

        for (int i = 0; i < curNode->childrenCount; i++)
            calculateBoneTransition(&curNode->children[i], globalTransformation, prevAnimation, nextAnimation, haltTime, currentTime, transitionTime);
    }

    void calculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform, Animation* animation, float currentTime)
    {
        std::string nodeName = node->name;
        glm::mat4 boneTransform = node->transformation;

        Bone* bone = animation->findBone(nodeName);

        if (bone)
        {
            bone->update(currentTime);
            boneTransform = bone->getTransform();
        }

        glm::mat4 globalTransformation = parentTransform * boneTransform;

        auto boneProps = animation->getBoneProps();

        for (unsigned int i = 0; i < boneProps.size(); i++) {
            if (boneProps[i].name == nodeName) {
                glm::mat4 offset = boneProps[i].offset;
                m_FinalBoneMatrices[i] = globalTransformation * offset;
                break;
            }
        }

        for (int i = 0; i < node->childrenCount; i++)
            calculateBoneTransform(&node->children[i], globalTransformation, animation, currentTime);
    }

    std::vector<glm::mat4> GetFinalBoneMatrices()
    {
        return m_FinalBoneMatrices;
    }

    void AddAnimation(Animation* newAnimation)
    {
        m_Animations.push_back(newAnimation);
    }

    static AnimationManager& getInstance() {
        static AnimationManager instance;
        return instance;
    }

    float myFmod(float a, float b) {
        float result = a - (b * static_cast<int>(a / b));
        if (result < 0) {
            result += b;
        }
        return result;
    }

private:

    std::vector<glm::mat4> m_FinalBoneMatrices{};
    std::vector<Animation*> m_Animations;
    Animation* m_CurrentAnimation{ nullptr };
    Animation* m_QueueAnimation{ nullptr };
    Animation* m_NextAnimation{ nullptr };
    float m_CurrentTime;
    bool interpolating;
    float haltTime;
    float interTime;
};
