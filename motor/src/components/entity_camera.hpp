#pragma once

#include "entity.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Camera projection
enum struct CameraProjection { CAMERA_PERSPECTIVE, CAMERA_ORTHOGRAPHIC };

struct Camera : Entity{
public:
    // CameraProjection
    CameraProjection cameraProjection { CameraProjection::CAMERA_ORTHOGRAPHIC };

    // Camera Attributes
    glm::vec3 position { -6.0f, 6.0f, -6.0f }; // -60 66 -60
    glm::vec3 target { 0.0f, 0.0f, 0.0f };   // position -> player
    glm::vec3 up { 0.0f, 1.0f, 0.0f };

    glm::vec3 direction;

    glm::vec3 front { 0.0f, 0.0f, -1.0f };
    glm::vec3 right;
    float fovy { 60.0f };                    // 60

    Camera(){
        updateCameraVectors();
    }

    // Returns the view matrix
    glm::mat4 getViewMatrix(){
        return glm::lookAt(position, position + front, up);
    }

    // Return the projection matrix
    glm::mat4 getProjectionMatrix(int screenWidth, int screenHeight) const {
        if (cameraProjection == CameraProjection::CAMERA_PERSPECTIVE) {
            return glm::perspective(glm::radians(fovy), static_cast<float>(screenWidth) / static_cast<float>(screenHeight), 0.1f, 100.0f);
        } else if (cameraProjection == CameraProjection::CAMERA_ORTHOGRAPHIC) {
            // Calculate the dimensions of the orthographic projection box
            float aspectRatio = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);
            float halfHeight = glm::tan(glm::radians(fovy / 2.0f)) * glm::distance(position, target);
            float halfWidth = aspectRatio * halfHeight;

            // Calculate the bounds of the orthographic projection
            float left = -halfWidth;
            float right = halfWidth;
            float bottom = -halfHeight;
            float top = halfHeight;

            float nearPlane = 0.1f;
            float farPlane = 100.0f;

            return glm::ortho(left, right, bottom, top, nearPlane, farPlane);
        }

        return glm::mat4(1.0f);
    }

    void updateCameraVectors() {
        direction = glm::normalize(position - target);
        right = glm::normalize(glm::cross(up, direction));
        up = glm::cross(direction, right);
        front = -direction;
    }

    void draw(glm::mat4) override {};
};