#include "components/node.hpp"
#include "components/entity.hpp"
#include "components/entity_model.hpp"
#include "managers/resource_manager.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>

int main(){

    //--- Initialize managers ---//
    ResourceManager rm;

    //--- Create scene tree ---//
    auto nScene = std::make_unique<Node>();
    nScene->name = "Scene";

    auto nLight = std::make_unique<Node>();
    nLight->name = "Light";
    nScene->addChild(nLight.get());
    auto eLight =  std::make_unique<Light>();
    nLight->setEntity(eLight.get());

    auto nCamera = std::make_unique<Node>();
    nCamera->name = "Camera";
    nScene->addChild(nCamera.get());
    auto eCamera =  std::make_unique<Camera>();
    nCamera->setEntity(eCamera.get());

    //--- Load model ---// 
    auto nModel = std::make_unique<Node>();
    nScene->addChild(nModel.get());

    auto filePath = "assets/main_character.obj";
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);
    if(!scene)
        std::cerr << "[ERROR ASSIMP] : " << importer.GetErrorString() << std::endl;

    nModel->name = scene->GetShortFilename(filePath);

    for(int i=0; i<scene->mNumMeshes; i++){
        auto nMesh = std::make_unique<Node>();
        nMesh->name = "mesh_" + std::to_string(i);
        nModel->addChild(nMesh.get());
    }
    
    //processNode(scene->mRootNode, scene, rm);

    /*
    std::cout << scene->mNumMeshes << std::endl;

    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[scene->mRootNode->mMeshes[i]];
        // Process mesh
        auto nMesh = std::make_unique<Node>();
        nMesh->name = "mesh_" + std::to_string(i) + "_" + nModel->name;
        std::cout << nMesh->name << std::endl;
        nModel.get()->addChild(nMesh.get());
    }
    */
    //nScene->drawTree();

    //nScene->traverse(glm::mat4());
}

/*
void processNode(aiNode* node, const aiScene* scene, ResourceManager& rm){
    // Process all nodes in the scene
    for(int i=0; i<node->mNumMeshes; i++){
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene, rm);
    }

    // Call child nodes
    for(int i=0; i<node->mNumChildren; i++)
        processNode(node->mChildren[i], scene, rm);
}

void processMesh(aiMesh* mesh, const aiScene* scene, ResourceManager& rm){
    // Iterate over the vertices of the mesh
    for(int i=0; i<mesh->mNumVertices; i++){

    }
}
*/
/*Node* processMesh(aiMesh* mesh, ResourceManager& rm) {

    // Crear nodo entidad (mesh)
    auto nMesh = std::make_unique<Node>();


    // Devolver nodo entidad (mesh)
    return nMesh.get();

    
    Mesh& resultMesh = rm.loadResource<Mesh>(mesh->mName.C_Str());

    // Process vertex
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        auto vertices = std::make_shared<Vertex>();
        vertices->position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertices->normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        if (mesh->mTextureCoords[0])
            vertices->textCoords = glm::vec3(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y, 0.0f);

        resultMesh.vertex.push_back(vertices);
    }

    // Process index
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j)
            resultMesh.index.push_back(static_cast<unsigned short>(face.mIndices[j]));
    }

    // Load mesh in memory
    if(resultMesh.isLoaded())
        resultMesh.aux();

    return std::make_shared<Mesh>(resultMesh);
    
};*/

/*void processMeshes(const aiScene* scene, aiNode* node, Node* parentNode, ResourceManager rm) {
    for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        parentNode->addChild(processMesh(mesh));
    }

    for (unsigned int i = 0; i < node->mNumChildren; ++i)
        processMeshes(scene, node->mChildren[i], node, rm);
    
};*/