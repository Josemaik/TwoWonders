#include "entity_model.hpp"

void Model::load(const char* filePath, ResourceManager& rm){
    // Read file
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);
    if(!scene | !scene->mRootNode){
        std::cerr << "[ERROR ASSIMP] : " << importer.GetErrorString() << std::endl;
        return;
    }
    
    m_loaded = true;
    m_name = filePath;

    std::cout << "Load -> " << m_name << std::endl;

    processNode(scene->mRootNode, scene, rm);
};

void Model::unload(ResourceManager& rm){
    std::cout << "Unload -> " << this->m_name << std::endl; 
    for(int i=0; i<static_cast<int>(m_meshes.size()); i++)
        rm.unloadResource(m_meshes[i]->id);
}

void Model::draw(glm::mat4) const { 
    std::cout << "Draw a model" << std::endl; 
    for(int i=0; i<static_cast<int>(m_meshes.size()); i++)
        m_meshes[i]->draw();
};

// PRIVATE

void Model::processNode(aiNode* node, const aiScene* scene, ResourceManager& rm) {
    // Process all the node's meshes
    for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene, rm);
    }

    // Recursively process each of the node's children
    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        processNode(node->mChildren[i], scene, rm);
    }
}

void Model::processMesh(aiMesh* mesh, const aiScene*, ResourceManager& rm) {
    std::vector<Vertex> vertices(mesh->mNumVertices);
    std::vector<u_int16_t> indices;
    std::vector<Texture> textures;

    // Process the vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        Vertex vertex;

        // Position
        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;

        // Normal
        if (mesh->HasNormals()) {
            vertex.normal.x = mesh->mNormals[i].x;
            vertex.normal.y = mesh->mNormals[i].y;
            vertex.normal.z = mesh->mNormals[i].z;
        }

        // Text Coords
        if (mesh->HasTextureCoords(0)) {
            vertex.textCoords.x = mesh->mTextureCoords[0][i].x;
            vertex.textCoords.y = mesh->mTextureCoords[0][i].y;
        }

        vertices[i] = vertex;
    }

    // Process the indices
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j) {
            indices.push_back(static_cast<unsigned short>(face.mIndices[j]));
        }
    }

    auto currentMesh = rm.loadResource<Mesh>(vertices, indices, textures);
    
    m_meshes.push_back(currentMesh);
}