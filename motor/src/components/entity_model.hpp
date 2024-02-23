#pragma once
#include "node.hpp"
#include "entity.hpp"
#include "resource_mesh.hpp"

#include <vector>
#include <glm/glm.hpp>

struct Model : Entity{
private:
    std::vector<std::shared_ptr<Mesh>> meshes; 

public:
    void load(const char* filePath, std::shared_ptr<Node> nScene);
    void draw(glm::mat4) const override;
};