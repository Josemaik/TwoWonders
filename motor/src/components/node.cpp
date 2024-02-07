#include "node.hpp"
#include <algorithm>
#include <iostream>
#include <iomanip>

Node::Node()
    : m_entity(nullptr), 
      m_parent(nullptr), 
      m_translation(0.0f), 
      m_rotation(0.0f), 
      m_scale(1.0f), 
      m_transformationMatrix(1.0f) {}

int Node::addChild(Node* child) {
    m_children.push_back(child);
    child->m_parent = this;
    return static_cast<int>(m_children.size() - 1);
}

int Node::removeChild(Node* child) {
    auto it = std::find(m_children.begin(), m_children.end(), child);
    if (it != m_children.end()) {
        m_children.erase(it);
        child->m_parent = nullptr;
        return 0;
    }
    return -1;
}

bool Node::setEntity(Entity* newEntity) {
    m_entity = newEntity;
    return true;
}

void Node::traverse(glm::mat4 parentMatrix) {

    // Check changes
    if(m_updateMatrix)
        m_transformationMatrix = parentMatrix * glm::translate(glm::mat4(1.0f), m_translation)
                * glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f))
                * glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f))
                * glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f))
                * glm::scale(glm::mat4(1.0f), m_scale);

    // Draw Entity
    if(m_entity)
        m_entity->draw(m_transformationMatrix);

    // printTransformationMatrix();

    for (Node* child : m_children) {
        if(m_updateMatrix)
            child->m_updateMatrix = true;
        child->traverse(m_transformationMatrix);
    }

    m_updateMatrix = false;
}

void Node::setTranslation(glm::vec3 newTranslation) {
    m_translation = newTranslation;
    m_updateMatrix = true;
}

void Node::setRotation(glm::vec3 newRotation) {
    m_rotation = newRotation;
    m_updateMatrix = true;
}

void Node::setScale(glm::vec3 newScale) {
    m_scale = newScale;
    m_updateMatrix = true;
}

void Node::setTransformationMatrix(glm::mat4 newMatrix) {
    m_transformationMatrix = newMatrix;
}

void Node::translate(glm::vec3 newTranslate){ setTranslation(m_translation + newTranslate); }
void Node::rotate(glm::vec3 newRotate){ setRotation(m_rotation + newRotate); }
void Node::scale(glm::vec3 newScale){ setScale(m_scale + newScale); }

// GETTERS

Entity* Node::getEntity() { return m_entity; }
Node* Node::getParent() { return m_parent; }
glm::vec3 Node::getTranslation() { return m_translation;}
glm::vec3 Node::getRotation() { return m_rotation; }
glm::vec3 Node::getScale() { return m_scale; }
glm::mat4 Node::getTransformationMatrix() { return m_transformationMatrix; }

// AUXILIARS

void Node::printTransformationMatrix() {
    std::cout << nodeName << " --> Transformation Matrix:" << std::endl;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << std::setw(2) << m_transformationMatrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}