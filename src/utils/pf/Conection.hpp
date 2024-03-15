#pragma once

#include <utils/types.hpp>

struct Conection
{
    Conection(uint16_t c,uint16_t fn,uint16_t tn) : 
    cost{c},fromNode{fn},toNode{tn} {};
    /* data */
    //devuelve el coste que tiene esa conexión
    // uint16_t getCost(const Conection& conection){
    //     return conection.cost;
    // }
    // //devuelve el nodo del que sale esa conexion
    // uint16_t getfromNode( const Conection& conection){
    //     return conection.fromNode;
    // }
    // //devuelve el nodo al cuál llega esa conexión
    // uint16_t getToNode(const Conection& conection){
    //     return conection.toNode;
    // }
    uint16_t cost{};
    uint16_t fromNode{};
    uint16_t toNode{};
};
