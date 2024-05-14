#pragma once

#include "../utils/animation.hpp"


struct AnimationManager
{
    AnimationManager() = default;

    void setCurrentAnimation(animation* currentAnimation)
    {
        m_CurrentTime = 0.0;
        m_CurrentAnimation = currentAnimation;

        m_FinalBoneMatrices.reserve(100);

        for (int i = 0; i < 100; i++)
            m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
    }

    void UpdateAnimation(float dt)
    {
        m_DeltaTime = dt;
        if (m_CurrentAnimation)
        {
            m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * dt;
            m_CurrentTime = static_cast<float>(fmod(m_CurrentTime, m_CurrentAnimation->GetDuration()));
            // std::cout << "CurrentTime: " << m_CurrentTime << "\n";
            CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
        }
    }

    void PlayAnimation(animation* pAnimation)
    {
        m_CurrentAnimation = pAnimation;
        m_CurrentTime = 0.0f;
    }

    void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform)
    {
        std::string nodeName = node->name;
        glm::mat4 nodeTransform = node->transformation;
        // std::cout << "name: " << nodeName << "\n";
        // for(int i = 0; i < 4; i++){
        //     for(int j = 0; j < 4; j++){
        //         std::cout << nodeTransform[i][j] << " ";
        //     }
        //     std::cout << "\n";
        // }

        bone* Bone = m_CurrentAnimation->FindBone(nodeName);

        if (Bone)
        {
            Bone->Update(m_CurrentTime);
            nodeTransform = Bone->GetLocalTransform();
            //  std::cout << "name: " << Bone->GetBoneName() << "\n";
            //  for(int i = 0; i < 4; i++){
            //     for(int j = 0; j < 4; j++){
            //         std::cout << nodeTransform[i][j] << " ";
            //     }
            //     std::cout << "\n";
            // }
        }

        glm::mat4 globalTransformation = parentTransform * nodeTransform;

        auto boneInfoVector = m_CurrentAnimation->GetBonesVector();
        for (auto& boneInfo : boneInfoVector) {
            if (boneInfo.GetBoneName() == nodeName) {
                // std::cout << "NameBone: " << nodeName << "\n";
                int index = boneInfo.GetBoneID();
                glm::mat4 offset = boneInfo.Getoffset();
                //  for(int i = 0; i < 4; i++){
                //     for(int j = 0; j < 4; j++){
                //         std::cout << offset[i][j] << " ";
                //     }
                //     std::cout << "\n";
                // }

                m_FinalBoneMatrices[index] = globalTransformation * offset;
                //  for(int i = 0; i < 4; i++){
                //     for(int j = 0; j < 4; j++){
                //         std::cout <<  m_FinalBoneMatrices[index][i][j] << " ";
                //     }
                //     std::cout << "\n";
                // }
                break; // Salir del bucle una vez que se ha encontrado el hueso
            }
        }
        for (int i = 0; i < node->childrenCount; i++)
            CalculateBoneTransform(&node->children[i], globalTransformation);
    }

    std::vector<glm::mat4> GetFinalBoneMatrices()
    {
        return m_FinalBoneMatrices;
    }

    void AddAnimation(animation* newAnimation)
    {
        m_Animations.push_back(newAnimation);
    }

    static AnimationManager& getInstance() {
        static AnimationManager instance;
        return instance;
    }

private:

    std::vector<glm::mat4> m_FinalBoneMatrices;
    std::vector<animation*> m_Animations;
    animation* m_CurrentAnimation;
    float m_CurrentTime;
    float m_DeltaTime;
};
