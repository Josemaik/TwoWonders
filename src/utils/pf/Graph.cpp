#include "Graph.hpp"

#include <unordered_set>
#include <queue>
#include <algorithm>

struct NodeRecord
{
    NodeRecord(uint16_t n, Conection c, double cost, double totalCost) : node(n), connection(c), costSoFar(cost), estimatedTotalCost(totalCost) {}
    
    uint16_t node{};
    Conection connection;
    double costSoFar{};
    double estimatedTotalCost{};
};

// Función auxiliar para encontrar un registro de nodo en un contenedor basado en el nodo
//Devuelve un puntero a NodeRecord
NodeRecord* findRecord(std::vector<NodeRecord>& list, uint16_t node) {
    for (auto& record : list) {
        if (record.node == node) {
            return &record;
        }
    }
    return nullptr;
}
//Devuelve true si lo encuentra, false en caso contrario
bool findRecord_b(std::vector<NodeRecord>& list, uint16_t node) {
    for (auto& record : list) {
        if (record.node == node) {
            return true;
        }
    }
    return false;
}
//Devuelve un iterador que apunta a la posición en la lista de el nodo
std::vector<NodeRecord>::iterator findRecordit(std::vector<NodeRecord>& list, uint16_t node) {
    return std::find_if(list.begin(), list.end(), [node](const NodeRecord& record) {
        return record.node == node;
    });
}

std::vector<vec3d> CompilePath(std::vector<NodeRecord>& closed,NodeRecord& current,uint16_t startNode,  std::map<uint16_t, vec3d>& Nodes) {
    std::vector<vec3d> path;
    //NodeRecord current = closed.back(); // Empezar desde el último nodo cerrado (el objetivo)
    while (current.node != startNode) {
        // Agregar el punto de conexión al camino
        // Suponiendo que haya un método para obtener la posición de un nodo
        path.push_back(Nodes[current.node]);
        // Retroceder al nodo anterior
        current = *findRecord(closed, current.connection.fromNode);
    }
    // Añadir la posición del nodo inicial
    path.push_back(Nodes[startNode]);
    // Invertir el camino ya que se construyó desde el objetivo hasta el inicio
    std::reverse(path.begin(), path.end());
    return path;
}

std::vector<vec3d> Graph::PathFindAStar(Debug_t& debug,uint16_t startNode, uint16_t goalNode){
    //Estructuras de datos para almacenar nodos abiertos y cerrados
    std::vector<NodeRecord> open;
    std::vector<NodeRecord> closed;
    
    //Inicializar record desde el start node
    NodeRecord startRecord(startNode,Conection(0,0,0),0.0,Heuristic(startNode,goalNode));

    //Inicializar open y closed list
    open.push_back(startRecord);
    
    NodeRecord current(0,Conection(0,0,0),0.0,0.0);
    //Iteramos a través de cada nodo
    while(!open.empty()){
        //Encontrar el elemento más pequeño en la lista abierta (usando estimatedTotalCost)
        auto currentIt = std::min_element(open.begin(), open.end(), [](const NodeRecord& a, const NodeRecord& b) {
            return a.estimatedTotalCost < b.estimatedTotalCost;
        });
        current = *currentIt;
        //Comprobar si es el goal node -> en ese caso terminamos
        if(current.node == goalNode){
           //Compilar la lista de conexiones en el camino
            return CompilePath(closed,current,startNode, Nodes);
        }

        // Obtenemos las conexiones salientes
        auto connections = getConections(current.node);

        // Iterar a través de cada conexión
        for (auto& connection : connections) {
            //obtenemos el coste estimado de el endNode
            uint16_t endNode = connection.toNode;
            double endNodeCost = current.costSoFar + connection.cost;

            // Creamos endNodeRecord
            NodeRecord endNodeRecord(startNode,Conection(0,0,0),0.0,0.0); //= *findRecord(open, endNode);
            //inicializo el heuristic
            double endNodeHeuristic{};
            //Comprobamos si esta en la lista cerrada
            if(findRecord_b(closed,endNode)){
                endNodeRecord = *findRecord(closed, endNode);
                //si no encontramos una ruta más corta - skip
                if(endNodeRecord.costSoFar <= endNodeCost){
                    continue;
                }
                //Borramos de la closed list
                closed.erase(findRecordit(closed,endNode));
                
                // obtenemos el heuristic - dist euclidea
                endNodeHeuristic = endNodeCost - endNodeRecord.costSoFar;
                // endNodeHeuristic = Heuristic(endNode,goalNode);
            } else if(findRecord_b(open,endNode)){
                    endNodeRecord = *findRecord(open, endNode);
                    if(endNodeRecord.costSoFar <= endNodeCost){
                        continue;
                    }
                    endNodeHeuristic = endNodeCost - endNodeRecord.costSoFar;
                    // endNodeHeuristic = Heuristic(endNode,goalNode);
            } else{
                        NodeRecord endNodeRecordcopy(0,Conection(0,0,0),0.0,0.0);
                        endNodeRecord = endNodeRecordcopy;
                        endNodeRecord.node = endNode;
                        endNodeHeuristic = Heuristic(endNode,goalNode);
            }
            
            //update node
            endNodeRecord.costSoFar = endNodeCost;
            endNodeRecord.connection = connection;
            endNodeRecord.estimatedTotalCost = endNodeCost + endNodeHeuristic;
            //add to the open list
            if(!findRecord_b(open,endNode)){
                open.push_back(endNodeRecord);
            }
        }
        //Terminamos de buscar conexiones
        //eliminamos de la open list y añadimos a la closed
        open.erase(findRecordit(open,current.node));
        closed.push_back(current);
        //debug
        debug.closedlist.push_back(Nodes[current.node]);
    }

    if(current.node != goalNode){
            return std::vector<vec3d>{};
    }else{
            //Compilar la lista de conexiones en el camino
            return CompilePath(closed,current,startNode, Nodes);
    }
    //Si llega al final sin encontrar path se devuelve uno vacio
    return std::vector<vec3d>();
}