#pragma once
#include "resource_mesh.hpp"

#include <vector>
#include <glm/glm.hpp>
#include <iostream>

struct Entity{
    virtual void draw(glm::mat4) const {};
};

struct Light:Entity{
    void draw(glm::mat4) const override {};
};

struct Camera:Entity{
    void draw(glm::mat4) const override {};
};

struct Model:Entity{
    std::vector<std::shared_ptr<Mesh>> meshes; 
    
    void draw(glm::mat4) const override { 
        std::cout << "Draw a model" << std::endl; 
        for(int i=0; i<static_cast<int>(meshes.size()); i++)
            meshes[i]->draw();
    };
};