#include "render_manager.hpp"

#include <iostream>

// Basic drawing functions
namespace DarkMoon {

    // Get position from 3D world to 2D world
    glm::vec2 RenderManager::getWorldToScreen(glm::vec3 pos3D){
        // Transform world position to clip space
        glm::vec4 clipSpace = m_camera->getProjectionMatrix(m_width, m_height) * m_camera->getViewMatrix() * glm::vec4(pos3D, 1.0f);

        // Convert clip space to normalized device coordinates (NDC)
        glm::vec3 ndc = glm::vec3(clipSpace) / clipSpace.w;

        // Convert NDC to screen space
        glm::vec2 screenPos;
        screenPos.x = (ndc.x + 1.0f) * 0.5f * static_cast<float>(m_width);
        screenPos.y = (1.0f - ndc.y) * 0.5f * static_cast<float>(m_height);

        return screenPos;
    }

    void RenderManager::beginMode3D() {
        useShader(activeLights ? shaders["lights"] : shaders["3D"]);
        glEnable(GL_DEPTH_TEST);

        if(activeLights){
            std::vector<PointLight> pointLightsData;

            for(Light* light : lights)
                if(auto pointLight = dynamic_cast<PointLight*>(light))
                    pointLightsData.push_back(*pointLight);  

            for(int i=0; i<static_cast<int>(pointLightsData.size()); i++){
                std::string positionUniformName  = "pointsLights[" + std::to_string(i) + "].position";
                std::string colorUniformName     = "pointsLights[" + std::to_string(i) + "].color";
                std::string constantUniformName  = "pointsLights[" + std::to_string(i) + "].constant";
                std::string linearUniformName    = "pointsLights[" + std::to_string(i) + "].linear";
                std::string quadraticUniformName = "pointsLights[" + std::to_string(i) + "].quadratic";
            
                glUniform4fv(glGetUniformLocation(shaders["lights"]->getIDShader(), positionUniformName.c_str()), 1, glm::value_ptr(pointLightsData[i].position));
                glUniform4fv(glGetUniformLocation(shaders["lights"]->getIDShader(), colorUniformName.c_str()), 1, glm::value_ptr(normalizeColor(pointLightsData[i].color)));
                glUniform1f(glGetUniformLocation(shaders["lights"]->getIDShader(), constantUniformName.c_str()), pointLightsData[i].constant);
                glUniform1f(glGetUniformLocation(shaders["lights"]->getIDShader(), linearUniformName.c_str()), pointLightsData[i].linear);
                glUniform1f(glGetUniformLocation(shaders["lights"]->getIDShader(), quadraticUniformName.c_str()), pointLightsData[i].quadratic);
            }

            glUniform1i(glGetUniformLocation(shaders["lights"]->getIDShader(), "NumPointLights"), static_cast<int>(pointLightsData.size()));

            /*
            std::vector<PointLight> pointLightsData;

                    pointLightsData.push_back(*pointLight);    
                
            if(!pointLightsData.empty()){
                GLuint pointLightsBuffer;
                size_t bufferSize = sizeof(PointLight) * pointLightsData.size();
                glGenBuffers(1, &pointLightsBuffer);
                glBindBuffer(GL_UNIFORM_BUFFER, pointLightsBuffer);
                glBufferData(GL_UNIFORM_BUFFER, bufferSize, pointLightsData.data(), GL_STATIC_DRAW);

                GLuint blockIndex = glGetUniformBlockIndex(shaders["lights"]->getIDShader(), "PointLights");

                glBindBufferBase(GL_UNIFORM_BUFFER, 0, pointLightsBuffer);
                glUniformBlockBinding(shaders["lights"]->getIDShader(), blockIndex, 0);

                glUniform1i(glGetUniformLocation(shaders["lights"]->getIDShader(), "NumPointLights"), pointLightsData.size());
            }
            */
        }
    }

    void RenderManager::endMode3D() {
        //glPopMatrix();
        //glPopAttrib();

        glDisable(GL_DEPTH_TEST);
        useShader(shaders["color"]);
    }

    // Basic drawing functions

    void RenderManager::clearBackground(Color color) {
        glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

}