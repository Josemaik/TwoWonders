#pragma once
#include "entity.hpp"
#include "resource_mesh.hpp"

#include <vector>
#include <iostream>
#include <glm/glm.hpp>

struct Model:Entity{
public:
    std::vector<std::shared_ptr<Mesh>> meshes; 
    
    void draw(glm::mat4) const override { 
        std::cout << "Draw a model" << std::endl; 
        for(int i=0; i<static_cast<int>(meshes.size()); i++)
            meshes[i]->draw();
    };
};

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct ModelLoader{
public:
    // Load model from file path using Assimp
    static std::shared_ptr<Model> loadModel(const std::string& filePath){
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE/* || scene->mRootNode*/){
            std::cerr << "[ERROR ASSIMP] : " << importer.GetErrorString() << std::endl;
            return nullptr;
        }

        std::shared_ptr<Model> model = std::make_shared<Model>();
        processNode(scene->mRootNode, scene, model);
        std::cout << "Load model" << std::endl;

        return model;
    };

private:
    // Recursively process a node in the Assimp scene graph
    static void processNode(aiNode* node, const aiScene* scene, std::shared_ptr<Model>& model) {
        for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            model->meshes.push_back(processMesh(mesh));
        }

        for (unsigned int i = 0; i < node->mNumChildren; ++i)
            processNode(node->mChildren[i], scene, model);
    };

    // Process a mesh and convert it into our Mesh representation
    static std::shared_ptr<Mesh> processMesh(aiMesh* mesh) {
        std::shared_ptr<Mesh> resultMesh = std::make_shared<Mesh>();

        // Process vertex
        for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
            auto vertices = std::make_shared<Vertex>();
            vertices->position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
            vertices->normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
            if (mesh->mTextureCoords[0])
                vertices->textCoords = glm::vec3(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y, 0.0f);

            resultMesh->vertex.push_back(vertices);
        }

        // Process index
        for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; ++j)
                resultMesh->index.push_back(static_cast<unsigned short>(face.mIndices[j]));
        }

        // Load mesh in memory
        if(resultMesh)
            resultMesh->load();

        return resultMesh;
    };
};