#pragma once
#include "node.hpp"
#include "entity.hpp"
#include "../resources/resource_mesh.hpp"
#include "../managers/resource_manager.hpp"

#include <vector>
#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct Model : Entity{
private:
    const char* m_name;
    std::vector<Mesh*> m_meshes; 
    bool m_loaded { false };

    void processNode(aiNode*, const aiScene*, ResourceManager& rm);
    void processMesh(aiMesh*, const aiScene*, ResourceManager& rm);

public:
    void load(const char* filePath, ResourceManager& rm);
    void unload(ResourceManager& rm);
    void draw(glm::mat4) override;

    bool isLoaded(){ return m_loaded; };

    const std::vector<Mesh*>& getMeshes() const { return m_meshes; };
};