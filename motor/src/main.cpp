#include "components/node.hpp"
#include "components/entity.hpp"

int main()
{
    //---- Crear la estructura del árbol ----
    Node *nScene = new Node ();
    Node *nLight = new Node ();
    Node *nCamera = new Node ();
    Node *nCarGroup = new Node ();
    Node *nChassis = new Node ();
    Node *nWheel1 = new Node ();
    Node *nWheel2 = new Node ();
    Node *nWheel3 = new Node ();
    Node *nWheel4 = new Node ();

    nScene->addChild(nLight);
    nScene->addChild(nCamera);
    nScene->addChild(nCarGroup);
    
    nCarGroup->addChild(nChassis);
    nCarGroup->addChild(nWheel1);
    nCarGroup->addChild(nWheel2);
    nCarGroup->addChild(nWheel3);
    nCarGroup->addChild(nWheel4);
    
    //---- Añadir entidades a los nodos ----
    Light *eLight = new Light();
    Camera *eCamera = new Camera();

    Mesh *eMeshChassis = new Mesh();
    Mesh *eMeshWheel1 = new Mesh();
    Mesh *eMeshWheel2 = new Mesh();
    Mesh *eMeshWheel3 = new Mesh();
    Mesh *eMeshWheel4 = new Mesh();

    nLight->setEntity(eLight);
    nCamera->setEntity(eCamera);

    nChassis->setEntity(eMeshChassis);
    nChassis->setEntity(eMeshWheel1);
    nChassis->setEntity(eMeshWheel2);
    nChassis->setEntity(eMeshWheel3);
    nChassis->setEntity(eMeshWheel4);

    //---- Aplicar transformaciones a nodos
    nLight->setTranslation({0, 100, 0});
    nCamera->setRotation({10, 0, 0});
    nCamera->translate({0, 0, 200});
    nCarGroup->setScale({2, 2, 2});

    //---- Recorrer el árbol (dibujarlo) ----
    nScene->traverse(glm::mat4x4());
}