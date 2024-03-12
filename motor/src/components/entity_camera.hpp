#pragma once

#include "entity.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Camera projection
enum struct CameraProjection { CAMERA_PERSPECTIVE, CAMERA_ORTHOGRAPHIC };

struct Camera : Entity{
public:
    // CameraProjection
    CameraProjection cameraProjection { CameraProjection::CAMERA_PERSPECTIVE };

    // Camera Attributes
    glm::vec3 position { 0.0f, 2.0f, 3.0f };
    glm::vec3 target { 0.0f, 0.0f, 0.0f };
    glm::vec3 up { 0.0f, 1.0f, 0.0f };

    glm::vec3 direction;

    glm::vec3 front { 0.0f, 0.0f, -1.0f };
    glm::vec3 right;
    float fovy { 95.0f };

    Camera(){
        updateCameraVectors();
    }

    // Returns the view matrix
    glm::mat4 getViewMatrix(){
        return glm::lookAt(position, position + front /* target ? */, up);
    }

    // Return the projection matrix
    glm::mat4 getProjectionMatrix(int screenWidth, int screenHeight) const {
        if (cameraProjection == CameraProjection::CAMERA_PERSPECTIVE) {
            return glm::perspective(glm::radians(fovy), static_cast<float>(screenWidth) / static_cast<float>(screenHeight), 0.1f, 100.0f);
        } else if (cameraProjection == CameraProjection::CAMERA_ORTHOGRAPHIC) {
            // Ajusta estos valores según tus necesidades
            float left = -1.0f;
            float right = 1.0f;
            float bottom = -1.0f;
            float top = 1.0f;
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
        //front = -direction;
    }

    void draw(glm::mat4) const override {};
};