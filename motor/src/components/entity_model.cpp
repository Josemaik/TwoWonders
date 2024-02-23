#include "entity_model.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void Model::load(const char* filePath, std::shared_ptr<Node> nScene){
    // Create node
    auto nModel = std::make_unique<Node>();

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);
    if(!scene)
        std::cerr << "[ERROR ASSIMP] : " << importer.GetErrorString() << std::endl;

    nModel->name = scene->GetShortFilename(filePath);
    nModel->setEntity(shared_from_this());

    nScene->addChild(std::move(nModel));
};

void Model::draw(glm::mat4) const { 
    std::cout << "Draw a model" << std::endl; 
    for(int i=0; i<static_cast<int>(meshes.size()); i++)
        meshes[i]->draw();
};