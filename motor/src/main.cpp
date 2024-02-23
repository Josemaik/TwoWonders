#include "components/node.hpp"
#include "components/entity.hpp"
#include "components/entity_model.hpp"
#include "managers/resource_manager.hpp"

#include <iostream>

std::shared_ptr<Node> createSceneTree();

int main(){

    //----- Initialize managers -----//
    ResourceManager rm;

    //----- Create scene tree -----//
    auto nScene = createSceneTree();

    //----- Load model -----// 
    // USER //
    //Model model = engine.loadModel("assets/main_character.obj");
    // Model model;
    // model.load("assets/main_character.obj", nScene);

    //----- View tree -----//
    std::cout << "Tree" << std::endl;
    nScene->drawTree();

    /*
    for(int i=0; i<scene->mNumMeshes; i++){
        auto nMesh = std::make_unique<Node>();
        nMesh->name = "mesh_" + std::to_string(i);
        nModel->addChild(nMesh.get());
    }
    */

    
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

    //nScene->traverse(glm::mat4());
}

std::shared_ptr<Node> createSceneTree(){
    //Create scene
    auto nScene = std::make_unique<Node>();
    nScene->name = "Scene";

    // Create Light
    auto nLight = std::make_unique<Node>();
    nLight->name = "Light";
    auto eLight = std::make_shared<Light>();
    nLight->setEntity(eLight);
    nScene->addChild(std::move(nLight));

    // Create Camera
    auto nCamera = std::make_unique<Node>();
    nCamera->name = "Camera";
    auto eCamera = std::make_shared<Camera>();
    nCamera->setEntity(eCamera);
    nScene->addChild(std::move(nCamera));

    return nScene;
}