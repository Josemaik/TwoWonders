#pragma once

#include <vector>
#include "Conection.hpp"
#include <map>

struct Graph
{
    /* data */
    void createGraph(){
        //cargo del json los nodos
    }
    //Nos devuelve las conexiones que salen de un nodo
    std::vector<Conection> getConections(uint16_t FromNode){
       std::vector<Conection> connections;

        // Iterar sobre todas las conexiones almacenadas en ArrayConections
        for (const auto& connection : ArrayConections) {
            // Verificar si la conexión sale del nodo FromNode
            if (connection.fromNode == FromNode) {
                connections.push_back(connection);
            }
        }

        return connections;
    }
    //Calcula el path con algoritmo A* y lo devuelve dado un start node y un target ndoe
    std::vector<vec3d> PathFindAStar(uint16_t startNode, uint16_t endNode);
    
    //heuristic - Calcula la distancia Euclídea
    double Heuristic(uint16_t currentNode,uint16_t endNode){
        // Verificar si los nodos existen en el mapa
        if (Nodes.find(currentNode) == Nodes.end() || Nodes.find(endNode) == Nodes.end()) {
            // Manejar el caso en que uno o ambos nodos no existen
            // Por ejemplo, lanzar una excepción o devolver un valor predeterminado
            return 0.0;
        }

        // Obtener las posiciones de los nodos actual y objetivo
        vec3d currentPosition = Nodes[currentNode];
        vec3d targetPosition = Nodes[endNode];

        // Calcular la distancia euclidiana entre las posiciones de los nodos
        double distance = currentPosition.distance(targetPosition);

        return distance;
    }
    private:
        //array de conexiones
        std::vector<Conection> ArrayConections;
        // número de nodo y su posición
        std::map<uint16_t, vec3d> Nodes;
};
