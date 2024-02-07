#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "entity.hpp"

// Node
struct Node{
private:
    Entity* m_entity;
    std::vector<Node*> m_children;
    Node* m_parent;
    glm::vec3 m_translation;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;
    glm::mat4 m_transformationMatrix;
    bool m_updateMatrix {false};

public:
    std::string nodeName;

    Node();
    int addChild(Node* child);
    // 0 : child erase | -1 : child not found
    int removeChild(Node* child);
    bool setEntity(Entity* newEntity);

    // Transform
    void traverse(glm::mat4 parentMatrix);
    void setTranslation(glm::vec3 newTranslation);
    void setRotation(glm::vec3 newRotation);
    void setScale(glm::vec3 newScale);
    void setTransformationMatrix(glm::mat4 newMatrix);
    void translate(glm::vec3 newTranslate);
    void rotate(glm::vec3 newRotate);
    void scale(glm::vec3 newScale);

    // Getters
    Entity* getEntity();
    Node* getParent();
    glm::vec3 getTranslation();
    glm::vec3 getRotation();
    glm::vec3 getScale();
    glm::mat4 getTransformationMatrix();

    // Auxiliars
    void printTransformationMatrix();
};
