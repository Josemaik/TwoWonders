#pragma once
#include "node.hpp"
#include "entity.hpp"
#include "resource_mesh.hpp"

#include <vector>
#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct Model : Entity{
private:
    std::vector<std::shared_ptr<Mesh>> m_meshes; 
    bool m_loaded { false };

    void processNode(aiNode*, const aiScene*);
    void processMesh(aiMesh*, const aiScene*);

public:
    void load(const char* filePath);
    void draw(glm::mat4) const override;

    bool isLoaded(){ return m_loaded; };
};