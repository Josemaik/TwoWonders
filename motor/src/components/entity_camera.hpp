#pragma once

#include "entity.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Camera : Entity{
public:
    // Camera Attributes
    glm::vec3 position { 0.0f, 0.0f, 0.0f };
    glm::vec3 front { 0.0f, 0.0f, -1.0f };
    glm::vec3 up { 0.0f, 1.0f, 0.0f };
    glm::vec3 right;
    glm::vec3 worldUp;
    // Euler Angles
    float yaw { -90.0f };
    float pitch { 0.0f };
    // Camera Options
    float movementSpeed { 2.5f };
    float mouseSensivity { 0.1f };
    float zoom { 45.0f };

    Camera(){ updateCameraVectors(); }

    // Contructor with vectors
    Camera(glm::vec3 pos, glm::vec3 up, float yaw, float pitch){
        this->position = pos;
        this->up = up;
        this->yaw = yaw;
        this->pitch = pitch;
        updateCameraVectors();
    }

    // Returns the view matrix
    glm::mat4 getViewMatrix(){
        return glm::lookAt(position, position + front, up);
    }

    void draw(glm::mat4) const override {};

private:
    void updateCameraVectors(){
        // Calculate the new Front vector
        glm::vec3 front;
        front.x = static_cast<float>(cos(glm::radians(yaw)) * cos(glm::radians(pitch)));
        front.y = static_cast<float>(sin(glm::radians(pitch)));
        front.z = static_cast<float>(sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
        front = glm::normalize(front);
        // Also re-calculate the Right and Up vector
        right = glm::normalize(glm::cross(front, worldUp));
        up    = glm::normalize(glm::cross(right, front));
    }
};