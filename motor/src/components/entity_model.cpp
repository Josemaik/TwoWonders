#include "entity_model.hpp"

void Model::load(const char* filePath){
    // Read file
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);
    if(!scene | !scene->mRootNode){
        std::cerr << "[ERROR ASSIMP] : " << importer.GetErrorString() << std::endl;
        return;
    }
    
    m_loaded = true;

    std::cout << "Load -> " << filePath << std::endl;

    processNode(scene->mRootNode, scene);
};

void Model::draw(glm::mat4) const { 
    std::cout << "Draw a model" << std::endl; 
    for(int i=0; i<static_cast<int>(m_meshes.size()); i++)
        m_meshes[i]->draw();
};

// PRIVATE

void Model::processNode(aiNode* node, const aiScene* scene) {
    // Process all the node's meshes
    for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene);
    }

    // Recursively process each of the node's children
    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        processNode(node->mChildren[i], scene);
    }
}

void Model::processMesh(aiMesh* mesh, const aiScene*) {
    std::cout << "Mesh with " << mesh->mNumVertices << " vertices." << std::endl;
}