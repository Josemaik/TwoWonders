#pragma once

#include "entity.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Camera projection
enum struct CameraProjection { CAMERA_PERSPECTIVE, CAMERA_ORTHOGRAPHIC };

struct Camera : Entity{
public:
    // CameraProjection
    CameraProjection cameraProjection;

    // Camera Attributes
    glm::vec3 position { 0.0f, 2.0f, 3.0f };
    glm::vec3 target {0.0f, 0.0f, 0.0f };
    glm::vec3 up { 0.0f, 1.0f, 0.0f };

    glm::vec3 direction;

    glm::vec3 front { 0.0f, 0.0f, -1.0f };
    glm::vec3 right;
    float fovy { 45.0f };

    Camera(){
        direction = glm::normalize(position - target);
    }

    // Contructor with vectors
    // Camera(glm::vec3 pos, glm::vec3 up){
    //     this->position = pos;
    //     this->up = up;
    // }

    // Returns the view matrix
    glm::mat4 getViewMatrix(){
        return glm::lookAt(position, target, up);
    }

    void draw(glm::mat4) const override {};
};