#pragma once
#include <vector>
#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "entity.hpp"

// Node
struct Node{
private:
    std::shared_ptr<Entity> m_entity;
    std::vector<std::unique_ptr<Node>> m_children;
    Node* m_parent;
    glm::vec3 m_translation;
    glm::quat m_rotation;
    glm::vec3 m_scale;
    glm::mat4 m_transformationMatrix;
    bool m_updateMatrix {false};

public:
    std::string name;

    Node();
    int addChild(std::unique_ptr<Node> child);
    // 0 : child erase | -1 : child not found
    int removeChild(Node* child);
    bool setEntity(std::shared_ptr<Entity> newEntity);

    // Transform
    void traverse(glm::mat4 parentMatrix);
    void setTranslation(glm::vec3 newTranslation);
    void setRotation(glm::vec3 axis, float angle);
    void setScale(glm::vec3 newScale);
    void setTransformationMatrix(glm::mat4 newMatrix);
    void translate(glm::vec3 newTranslate);
    void rotate(glm::vec3 axis, float angle);
    void scale(glm::vec3 newScale);

    // Getters
    std::shared_ptr<Entity> getEntity();
    Node* getParent();
    glm::vec3 getTranslation();
    glm::quat getRotation();
    glm::vec3 getScale();
    glm::mat4 getTransformationMatrix();

    // Auxiliars
    void printTransformationMatrix();
    void drawTree(std::string prefix = "", bool isLeft = false);
};
