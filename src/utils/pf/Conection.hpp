#pragma once

#include <utils/types.hpp>

struct Conection
{
    Conection::Conection(){};
    /* data */
    //devuelve el coste que tiene esa conexión
    uint16_t getCost(Conection& conection);
    //devuelve el nodo del que sale esa conexion
    uint16_t getfromnode(Conection& conection);
    //devuelve el nodo al cuál llega esa conexión
    uint16_t getToNode(Conection& conection);
    private:
        uint16_t cost{};
        uint16_t fromNode{};
        uint16_t toNode{};
};
