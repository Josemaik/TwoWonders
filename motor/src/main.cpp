#include "components/node.hpp"
#include "components/entity.hpp"
#include <memory>

int main()
{
    //---- Crear la estructura del árbol ----
    auto nScene = std::make_unique<Node>();
    auto nLight = std::make_unique<Node>();
    auto nCamera = std::make_unique<Node>();
    auto nCarGroup = std::make_unique<Node>();
    auto nChassis = std::make_unique<Node>();
    auto nWheel1 = std::make_unique<Node>();
    auto nWheel2 = std::make_unique<Node>();
    auto nWheel3 = std::make_unique<Node>();
    auto nWheel4 = std::make_unique<Node>();

    nScene->nodeName = "Scene";
    nLight->nodeName = "Light";
    nCamera->nodeName = "Camera";
    nCarGroup->nodeName = "CarGroup";
    nChassis->nodeName = "Chassis";
    nWheel1->nodeName = "Wheel1";
    nWheel2->nodeName = "Wheel2";
    nWheel3->nodeName = "Wheel3";
    nWheel4->nodeName = "Wheel4";

    nScene->addChild(nLight.get());
    nScene->addChild(nCamera.get());
    nScene->addChild(nCarGroup.get());
    
    nCarGroup->addChild(nChassis.get());
    nCarGroup->addChild(nWheel1.get());
    nCarGroup->addChild(nWheel2.get());
    nCarGroup->addChild(nWheel3.get());
    nCarGroup->addChild(nWheel4.get());
    
    //---- Añadir entidades a los nodos ----
    auto eLight = std::make_unique<Light>();
    auto eCamera = std::make_unique<Camera>();

    auto eMeshChassis = std::make_unique<Mesh>();
    auto eMeshWheel1 = std::make_unique<Mesh>();
    auto eMeshWheel2 = std::make_unique<Mesh>();
    auto eMeshWheel3 = std::make_unique<Mesh>();
    auto eMeshWheel4 = std::make_unique<Mesh>();

    nLight->setEntity(eLight.get());
    nCamera->setEntity(eCamera.get());

    nChassis->setEntity(eMeshChassis.get());
    nChassis->setEntity(eMeshWheel1.get());
    nChassis->setEntity(eMeshWheel2.get());
    nChassis->setEntity(eMeshWheel3.get());
    nChassis->setEntity(eMeshWheel4.get());

    //---- Aplicar transformaciones a nodos
    nLight->setTranslation({0, 100, 0});
    nCamera->setRotation({10, 0, 0});
    nCamera->translate({0, 0, 200});
    nCarGroup->setScale({2, 2, 2});

    //---- Recorrer el árbol (dibujarlo) ----
    nScene->traverse(glm::mat4x4());
}