#include "tnode.hpp"
#include <algorithm>

TNode::TNode()
    : entity(nullptr), 
      parent(nullptr), 
      translation(0.0f), 
      rotation(0.0f), 
      scale(1.0f), 
      transformationMatrix(1.0f) {}

int TNode::addChild(TNode* child) {
    children.push_back(child);
    child->parent = this;
    return static_cast<int>(children.size() - 1);
}

int TNode::removeChild(TNode* child) {
    auto it = std::find(children.begin(), children.end(), child);
    if (it != children.end()) {
        children.erase(it);
        child->parent = nullptr;
        return 0;
    }
    return -1;
}

bool TNode::setEntity(TEntity* newEntity) {
    entity = newEntity;
    return true;
}

void TNode::traverse(glm::mat4 parentMatrix) {

    // TODO: check bool updateMatrix

    transformationMatrix = parentMatrix * glm::translate(glm::mat4(1.0f), translation)
                          * glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f))
                          * glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f))
                          * glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f))
                          * glm::scale(glm::mat4(1.0f), scale);

    // Draw Entity | entity->draw(transformationMatrix)

    for (TNode* child : children) {
        child->traverse(transformationMatrix);
    }
}

void TNode::setTranslation(glm::vec3 newTranslation) {
    translation = newTranslation;
    // TODO: bool true updateMatrix
}

void TNode::setRotation(glm::vec3 newRotation) {
    rotation = newRotation;
    // TODO: bool true updateMatrix
}

void TNode::setScale(glm::vec3 newScale) {
    scale = newScale;
    // TODO: bool true updateMatrix
}

void TNode::setTransformationMatrix(glm::mat4 newMatrix) {
    transformationMatrix = newMatrix;
}

// GETTERS

TEntity* TNode::getEntity() {
    return entity;
}

TNode* TNode::getParent() {
    return parent;
}

glm::vec3 TNode::getTranslation() {
    return translation;
}

glm::vec3 TNode::getRotation() {
    return rotation;
}

glm::vec3 TNode::getScale() {
    return scale;
}

glm::mat4 TNode::getTransformationMatrix() {
    return transformationMatrix;
}