#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Virtual Entity
struct TEntity{

};

// TNode
struct TNode{
private:
    TEntity* entity;
    std::vector<TNode*> children;
    TNode* parent;
    glm::vec3 translation;
    glm::vec3 rotation;
    glm::vec3 scale;
    glm::mat4 transformationMatrix;

public:
    TNode();
    int addChild(TNode* child);
    // 0 : child erase | -1 : child not found
    int removeChild(TNode* child);
    bool setEntity(TEntity* newEntity);

    // Transform
    void traverse(glm::mat4 parentMatrix);
    void setTranslation(glm::vec3 newTranslation);
    void setRotation(glm::vec3 newRotation);
    void setScale(glm::vec3 newScale);
    void setTransformationMatrix(glm::mat4 newMatrix);

    // Getters
    TEntity* getEntity();
    TNode* getParent();
    glm::vec3 getTranslation();
    glm::vec3 getRotation();
    glm::vec3 getScale();
    glm::mat4 getTransformationMatrix();
};
