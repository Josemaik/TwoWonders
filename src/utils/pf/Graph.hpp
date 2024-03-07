#pragma once

#include <utils/types.hpp>
#include <vector>
#include<Conection.hpp> 

struct Graph
{
    /* data */
    void createGraph(){
        //cargo del json los nodos
    }
    ArrayConections getConections(uint16_t FromNode){
        //Nos devuelve las conexiones que salen de un nodo
        return ArrayConections;
    }
    //array de conexiones
    //funcion getconections(uint16_t node);
    std::vector<Conection> ArrayConections;
    // número de nodo y su posición
    std::pair<uint16_t,vec3d> Nodes;
};
