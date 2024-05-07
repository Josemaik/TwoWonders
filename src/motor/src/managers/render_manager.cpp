#include "render_manager.hpp"

#include <chrono>
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
        //glEnable(GL_CULL_FACE);
    }

    void RenderManager::endMode3D() {
        //glPopMatrix();
        //glPopAttrib();

        glDisable(GL_DEPTH_TEST);
        //glDisable(GL_CULL_FACE);
        useShader(shaders["color"]);
    }

    void RenderManager::updateLights(){
        pointLights.clear();
        directionalLights.clear();
    }

    void RenderManager::checkLights(){
        if(activeLights){
            useShader(shaders["lights"]);

            // Point Lights //

            if(!pointLights.empty()){
                for(int i=0; i<static_cast<int>(pointLights.size()); i++){
                    std::string positionUniformName  = "pointsLights[" + std::to_string(i) + "].position";
                    std::string colorUniformName     = "pointsLights[" + std::to_string(i) + "].color";
                    std::string constantUniformName  = "pointsLights[" + std::to_string(i) + "].constant";
                    std::string linearUniformName    = "pointsLights[" + std::to_string(i) + "].linear";
                    std::string quadraticUniformName = "pointsLights[" + std::to_string(i) + "].quadratic";
                
                    glUniform4fv(glGetUniformLocation(shaders["lights"]->getIDShader(), positionUniformName.c_str()), 1, glm::value_ptr(pointLights[i]->position));
                    glUniform4fv(glGetUniformLocation(shaders["lights"]->getIDShader(), colorUniformName.c_str()), 1, glm::value_ptr(normalizeColor(pointLights[i]->color)));
                    glUniform1f(glGetUniformLocation(shaders["lights"]->getIDShader(), constantUniformName.c_str()), pointLights[i]->constant);
                    glUniform1f(glGetUniformLocation(shaders["lights"]->getIDShader(), linearUniformName.c_str()), pointLights[i]->linear);
                    glUniform1f(glGetUniformLocation(shaders["lights"]->getIDShader(), quadraticUniformName.c_str()), pointLights[i]->quadratic);
                }
            }
            glUniform1i(glGetUniformLocation(shaders["lights"]->getIDShader(), "NumPointLights"), static_cast<int>(pointLights.size()));

            // Directional Lights //

            if(!directionalLights.empty()){
                for(int i=0; i<static_cast<int>(directionalLights.size()); i++){
                    std::string directionUniformName  = "directionalLights[" + std::to_string(i) + "].direction";
                    std::string colorUniformName     = "directionalLights[" + std::to_string(i) + "].color";

                    glUniform4fv(glGetUniformLocation(shaders["lights"]->getIDShader(), directionUniformName.c_str()), 1, glm::value_ptr(directionalLights[i]->direction));
                    glUniform4fv(glGetUniformLocation(shaders["lights"]->getIDShader(), colorUniformName.c_str()), 1, glm::value_ptr(normalizeColor(directionalLights[i]->color)));
                }

            }
            glUniform1i(glGetUniformLocation(shaders["lights"]->getIDShader(), "NumDirectionalLights"), static_cast<int>(directionalLights.size()));

        }
    }

    // Basic drawing functions

    void RenderManager::clearBackground(Color color) {
        glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}