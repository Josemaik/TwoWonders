#include "components/node.hpp"
#include "components/entity.hpp"
#include "components/entity_model.hpp"
#include "managers/resource_manager.hpp"

#include <iostream>

int main(){

    std::cout << "--------" << std::endl;
    std::cout << "- Load -" << std::endl;
    std::cout << "--------" << std::endl;

    // Model
    std::shared_ptr<Model> model = ModelLoader::loadModel("assets/main_character.obj");

    std::cout << "--------" << std::endl;
    std::cout << "- Draw -" << std::endl;
    std::cout << "--------" << std::endl;

    if(!model)
        std::cerr << "Failed to load model" << std::endl;
    else
        model->draw(glm::mat4(1.0f));

    //// Load resources
    //ResourceManager rm;
    //Mesh& rMeshWheel = rm.loadResource<Mesh>("mesh_wheel.obj");
//
    //// Create scene
    //auto nScene = std::make_unique<Node>();
    //auto nLight = std::make_unique<Node>();
    //auto nCamera = std::make_unique<Node>();
    //auto nWheel = std::make_unique<Node>();
//
    //nScene->addChild(nLight.get());
    //nScene->addChild(nCamera.get());
    //nScene->addChild(nWheel.get());
//
    //auto eModelWheel = std::make_unique<Model>();
    //eModelWheel->meshes.push_back(std::make_shared<Mesh>(rMeshWheel));
    //nWheel->setEntity(eModelWheel.get());
//
    //// Draw entities
    //nScene->traverse(glm::mat4());
//
    //// Unload resources
    //rm.unloadResource("mesh_wheel.obj");

    // //---- Crear la estructura del árbol ----
    // auto nScene = std::make_unique<Node>();
    // auto nLight = std::make_unique<Node>();
    // auto nCamera = std::make_unique<Node>();
    // auto nCarGroup = std::make_unique<Node>();
    // auto nChassis = std::make_unique<Node>();
    // auto nWheel1 = std::make_unique<Node>();
    // auto nWheel2 = std::make_unique<Node>();
    // auto nWheel3 = std::make_unique<Node>();
    // auto nWheel4 = std::make_unique<Node>();
// 
    // nScene->nodeName = "Scene";
    // nLight->nodeName = "Light";
    // nCamera->nodeName = "Camera";
    // nCarGroup->nodeName = "CarGroup";
    // nChassis->nodeName = "Chassis";
    // nWheel1->nodeName = "Wheel1";
    // nWheel2->nodeName = "Wheel2";
    // nWheel3->nodeName = "Wheel3";
    // nWheel4->nodeName = "Wheel4";
// 
    // nScene->addChild(nLight.get());
    // nScene->addChild(nCamera.get());
    // nScene->addChild(nCarGroup.get());
    // 
    // nCarGroup->addChild(nChassis.get());
    // nCarGroup->addChild(nWheel1.get());
    // nCarGroup->addChild(nWheel2.get());
    // nCarGroup->addChild(nWheel3.get());
    // nCarGroup->addChild(nWheel4.get());
    // 
    // //---- Añadir entidades a los nodos ----
    // auto eLight = std::make_unique<Light>();
    // auto eCamera = std::make_unique<Camera>();
// 
    // auto eMeshChassis = std::make_unique<Mesh>();
    // auto eMeshWheel1 = std::make_unique<Mesh>();
    // auto eMeshWheel2 = std::make_unique<Mesh>();
    // auto eMeshWheel3 = std::make_unique<Mesh>();
    // auto eMeshWheel4 = std::make_unique<Mesh>();
// 
    // nLight->setEntity(eLight.get());
    // nCamera->setEntity(eCamera.get());
// 
    // nChassis->setEntity(eMeshChassis.get());
    // nChassis->setEntity(eMeshWheel1.get());
    // nChassis->setEntity(eMeshWheel2.get());
    // nChassis->setEntity(eMeshWheel3.get());
    // nChassis->setEntity(eMeshWheel4.get());
// 
    // //---- Aplicar transformaciones a nodos
    // nLight->setTranslation({0, 100, 0});
    // nCamera->setRotation({1, 0, 0}, 10.0f);
    // nCamera->translate({0, 0, 200});
    // nCarGroup->setScale({2, 2, 2});
// 
    // //---- Recorrer el árbol (dibujarlo) ----
    // nScene->traverse(glm::mat4x4());
}