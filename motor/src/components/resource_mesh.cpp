#include "resource_mesh.hpp"

void Mesh::draw(){ 
    std::cout << "Draw a mesh" << std::endl; 
    /* OpenGL */ 
}

bool Mesh::load(){ 
    std::cout << "Load a mesh" << std::endl;
    return true; 
}

void Mesh::unload(){ 
    std::cout << "Unload a mesh" << std::endl; 
}

bool Mesh::isLoaded() const{ 
    return true; 
}

void Mesh::setupMesh(){

}
