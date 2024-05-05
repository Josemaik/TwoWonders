#pragma once

#include "../utils/animation.hpp"


struct animator_manager
{
    animator_manager(animation* currentAnimation)
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
	
        bone* Bone = m_CurrentAnimation->FindBone(nodeName);
	
        if (Bone)
        {
            Bone->Update(m_CurrentTime);
            nodeTransform = Bone->GetLocalTransform();
        }
	
        glm::mat4 globalTransformation = parentTransform * nodeTransform;
	
        auto boneInfoVector = m_CurrentAnimation->GetBonesVector();
        for (auto& boneInfo : boneInfoVector) {
            if (boneInfo.GetBoneName() == nodeName) {
                int index = boneInfo.GetBoneID();
                glm::mat4 offset = boneInfo.Getoffset();
                m_FinalBoneMatrices[index] = globalTransformation * offset;
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

    

    static animator_manager& getInstance() {
            static animator_manager instance;
            return instance;
    }
		
private:
    animator_manager() = default;

    std::vector<glm::mat4> m_FinalBoneMatrices;
    animation* m_CurrentAnimation;
    float m_CurrentTime;
    float m_DeltaTime;	
};
