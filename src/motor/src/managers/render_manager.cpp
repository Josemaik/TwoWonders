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
        useShader(shader3D);
        glEnable(GL_DEPTH_TEST);
    }

    void RenderManager::endMode3D() {
        //glPopMatrix();
        //glPopAttrib();

        glDisable(GL_DEPTH_TEST);
        useShader(shaderColor);
    }

    // Basic drawing functions

    void RenderManager::clearBackground(Color color) {
        glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

}