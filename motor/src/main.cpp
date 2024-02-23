#include "components/node.hpp"
#include "components/entity.hpp"
#include "components/entity_model.hpp"
#include "managers/resource_manager.hpp"

#include <iostream>

std::shared_ptr<Node> createSceneTree();
std::shared_ptr<Model> loadModel(const char*, std::shared_ptr<Node>);

int main(){

    //----- Initialize managers -----//
    ResourceManager rm;

    //----- Create scene tree -----//
    auto nScene = createSceneTree();

    //----- Load model -----// 
    std::cout << "┌──────┐" << std::endl;
    std::cout << "│ Load │" << std::endl;
    std::cout << "└──────┘" << std::endl;
    // USER //
    // Model model = engine.loadModel("assets/main_character.obj");
    auto filePath = "assets/main_character.obj";
    auto eModel = loadModel(filePath, nScene);

    //----- View tree -----//
    std::cout << "┌──────┐" << std::endl;
    std::cout << "│ Tree │" << std::endl;
    std::cout << "└──────┘" << std::endl;
    nScene->drawTree();

    //----- Draw -----//
    std::cout << "┌──────┐" << std::endl;
    std::cout << "│ Draw │" << std::endl;
    std::cout << "└──────┘" << std::endl;
    nScene->traverse(glm::mat4());

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

std::shared_ptr<Model> loadModel(const char* filePath, std::shared_ptr<Node> nScene){
    auto nModel = std::make_unique<Node>();
    nModel->name = filePath;
    auto eModel = std::make_shared<Model>();
    eModel->load(filePath);
    if(eModel->isLoaded()){
        nModel->setEntity(eModel);
        nScene->addChild(std::move(nModel));
    }
    return eModel;
}