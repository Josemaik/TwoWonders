#pragma once

#include "entity.hpp"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace DarkMoon {
    // Camera projection
    enum struct CameraProjection { CAMERA_PERSPECTIVE, CAMERA_ORTHOGRAPHIC };

    struct Ray {
        glm::vec3 origin;
        glm::vec3 direction;
    };

    struct Camera : Entity {
    public:
        // CameraProjection
        CameraProjection cameraProjection{ CameraProjection::CAMERA_ORTHOGRAPHIC };

        // Camera Attributes
        glm::vec3 position{ 1.0f, 1.0f, 1.0f };
        glm::vec3 target{ 0.0f, 0.0f, 0.0f };
        glm::vec3 up{ 0.0f, 1.0f, 0.0f };

        glm::vec3 direction;

        glm::vec3 front{ 0.0f, 0.0f, -1.0f };
        glm::vec3 right;
        float fovy{ 60.0f };

        Camera() {
            updateCameraVectors();
        }

        // Returns the view matrix
        glm::mat4 getViewMatrix() {
            return glm::lookAt(position, target, up);
        }

        // Return the projection matrix
        glm::mat4 getProjectionMatrix(int screenWidth, int screenHeight) const {
            // Camera Perspective
            if (cameraProjection == CameraProjection::CAMERA_PERSPECTIVE) {
                return glm::perspective(glm::radians(fovy), 
                                        static_cast<float>(screenWidth) / static_cast<float>(screenHeight), 
                                        0.1f, 100.0f);
            }
            // Camera Orthographic
            else if (cameraProjection == CameraProjection::CAMERA_ORTHOGRAPHIC) {
                float aspect = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);
                float top = fovy / 2.0f;
                float right = top * aspect;

                return glm::ortho(-right, right, -top, top, 0.1f, 1000.0f);
            }

            return glm::mat4(1.0f);
        }

        Ray getMouseRay(int mouseX, int mouseY, int screenWidth, int screenHeight) {            
            Ray ray = {};

            // Normalized device coordinates
            float x = (2.0f * static_cast<float>(mouseX)) / static_cast<float>(screenWidth) - 1.0f;
            float y = 1.0f - (2.0f * static_cast<float>(mouseY)) / static_cast<float>(screenHeight);
            float z = 1.0f;

            glm::vec3 deviceCoords = { x, y, z };

            // View matrix and projection matrix
            auto matView = getViewMatrix();
            auto matProj = getProjectionMatrix(screenWidth, screenHeight);

            // Unproject far/near points
            glm::vec3 nearPoint = glm::unProject(glm::vec3(deviceCoords.x, deviceCoords.y, 0.0f), matView, matProj, glm::vec4(0, 0, screenWidth, screenHeight));
            glm::vec3 farPoint = glm::unProject(glm::vec3(deviceCoords.x, deviceCoords.y, 1.0f), matView, matProj, glm::vec4(0, 0, screenWidth, screenHeight));
            
            // Normalize direction vector
            ray.direction = glm::normalize(farPoint - nearPoint);

            // Unproject mouse cursor in the near plane
            glm::vec3 cameraPlanePointerPos = glm::unProject(glm::vec3(deviceCoords.x, deviceCoords.y, -1.0f), matView, matProj, glm::vec4(0, 0, screenWidth, screenHeight));

            if(this->cameraProjection == CameraProjection::CAMERA_PERSPECTIVE)
                ray.origin = this->position;
            else if(this->cameraProjection == CameraProjection::CAMERA_ORTHOGRAPHIC)
                ray.origin = cameraPlanePointerPos;

            return ray;
        }

        void updateCameraVectors() {
            front = glm::normalize(target - position);
            right = glm::normalize(glm::cross(front, up));
        }

        void draw(glm::mat4) override {};
    };
}