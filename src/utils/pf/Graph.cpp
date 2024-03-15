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

std::vector<vec3d> Graph::PathFindAStar(uint16_t startNode, uint16_t goalNode){
    //Estructuras de datos para almacenar nodos abiertos y cerrados
    std::vector<NodeRecord> open;
    std::unordered_set<uint16_t> closed;
    
    //Inicializar record desde el start node
    NodeRecord startRecord(startNode,Conection(0,0,0),0.0,0.0);

    //Inicializar open y closed list
    open.push_back(startRecord);
    
    //Iteramos a través de cada nodo
    while(!open.empty()){
        //Encontrar el elemento más pequeño en la lista abierta (usando estimatedTotalCost)
        auto currentIt = std::min_element(open.begin(), open.end(), [](const NodeRecord& a, const NodeRecord& b) {
            return a.estimatedTotalCost < b.estimatedTotalCost;
        });
        NodeRecord current = *currentIt;
        //Comprobar si es el goal node -> en ese caso terminamos
        if(current.node == goalNode){
           //Compilar la lista de conexiones en el camino
            std::vector<vec3d> path;
            while (current.node != startNode) {
                // Agregar el punto de conexión al camino
                // Suponiendo que haya un método para obtener la posición de un nodo
                path.push_back(Nodes[current.node]);
                // Retroceder al nodo anterior
                current = *findRecord(open, current.connection.fromNode);
            }
            // Añadir la posición del nodo inicial
            path.push_back(Nodes[startNode]);
            // Invertir el camino ya que se construyó desde el objetivo hasta el inicio
            std::reverse(path.begin(), path.end());
            return path;
        }

        // Obtenemos las conexiones salientes
        auto connections = getConections(current.node);

        // Iterar a través de cada conexión
        for (auto& connection : connections) {
            //obtenemos el coste estimado de el endNode
            uint16_t endNode = connection.toNode;
            double endNodeCost = current.costSoFar + connection.cost;

            // Buscar el registro del nodo en la lista abierta
            NodeRecord endNodeRecord = *findRecord(open, endNode);
            //inicializo el heuristic
            double endNodeHeuristic{};
            //Comprobamos si esta en la lista cerrada
            if(closed.count(endNode) > 0){
                //si no encontramos una ruta más corta - skip
                if(endNodeRecord.costSoFar <= endNodeCost){
                    continue;
                }
                //Borramos de la closed list
                closed.erase(endNode);
                
                // obtenemos el heuristic - dist euclidea
                endNodeHeuristic = Heuristic(endNode,goalNode);
            } else if(findRecord_b(open,endNode)){
                    if(endNodeRecord.costSoFar <= endNodeCost){
                        continue;
                    }
                    endNodeHeuristic = Heuristic(endNode,goalNode);
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
        closed.insert(current.node);

        if(current.node != goalNode){
            return std::vector<vec3d>{};
        }else{
            //Compilar la lista de conexiones en el camino
            std::vector<vec3d> path;
            while (current.node != startNode) {
                // Agregar el punto de conexión al camino
                // Suponiendo que haya un método para obtener la posición de un nodo
                path.push_back(Nodes[current.node]);
                // Retroceder al nodo anterior
                current = *findRecord(open, current.connection.fromNode);
            }
            // Añadir la posición del nodo inicial
            path.push_back(Nodes[startNode]);
            // Invertir el camino ya que se construyó desde el objetivo hasta el inicio
            std::reverse(path.begin(), path.end());
            return path;
        }
    }
    //Si llega al final sin encontrar path se devuelve uno vacio
    return std::vector<vec3d>();
}

// Inicialización del nodo de inicio
    // NodeRecord startRecord(startNode, Conection{0,0,0}, 0, Heuristic(startNode, goalNode));
    // open.push_back(startRecord);

    // // Iteración para procesar cada nodo
    // while (!open.empty()) {
    //     // Encontrar el elemento más pequeño en la lista abierta (usando estimatedTotalCost)
    //     auto currentIt = std::min_element(open.begin(), open.end(), [](const NodeRecord& a, const NodeRecord& b) {
    //         return a.estimatedTotalCost < b.estimatedTotalCost;
    //     });
    //     NodeRecord current = *currentIt;
    //     open.erase(currentIt);

    //     // Si es el nodo objetivo, terminar
    //     if (current.node == goalNode) {
    //         // Compilar la lista de conexiones en el camino
    //         std::vector<vec3d> path;
    //         while (current.node != startNode) {
    //             // Agregar el punto de conexión al camino
    //             // Suponiendo que haya un método para obtener la posición de un nodo
    //             path.push_back(Nodes[current.node]);
    //             // Retroceder al nodo anterior
    //             current = *findRecord(open, current.connection.fromNode);
    //         }
    //         // Añadir la posición del nodo inicial
    //         path.push_back(Nodes[startNode]);
    //         // Invertir el camino ya que se construyó desde el objetivo hasta el inicio
    //         std::reverse(path.begin(), path.end());
    //         return path;
    //     }

    //     // Agregar el nodo actual a la lista cerrada
    //     closed.insert(current.node);

    //     // Obtener las conexiones salientes del nodo actual
    //     auto connections = getConections(current.node);

    //     // Iterar a través de cada conexión
    //     for (auto& connection : connections) {
    //         uint16_t endNode = connection.toNode;
    //         double endNodeCost = current.costSoFar + connection.cost;

    //         // Si el nodo está en la lista cerrada y tiene un costo menor, continuar
    //         if (closed.count(endNode) > 0) {
    //             continue;
    //         }

    //         // Buscar el registro del nodo en la lista abierta
    //         NodeRecord* endNodeRecord = findRecord(open, endNode);

    //         // Si el nodo no está en la lista abierta o tiene un costo menor, actualizar
    //         if (endNodeRecord == nullptr || endNodeRecord->costSoFar <= endNodeCost) {
    //             double endNodeHeuristic;
    //             if (endNodeRecord != nullptr) {
    //                 // Podemos usar los valores de costo antiguos del nodo para calcular su heurística sin llamar
    //                 // a la función heurística posiblemente costosa
    //                 endNodeHeuristic = Heuristic(endNodeRecord->node,goalNode);
    //             } else {
    //                 if (closed.count(endNode) > 0) {
    //                     // Aquí encontramos el registro en la lista cerrada correspondiente al endNode.
    //                     // Si nuestra ruta no es mejor, entonces omitir
    //                     continue;
    //                 }
    //                 if (std::find(open.begin(), open.end(), endNode) != open.end()) {
    //                     // Aquí encontramos el registro en la lista abierta correspondiente al endNode.
    //                     // Si nuestra ruta no es mejor, entonces omitir
    //                     // Buscar el registro del nodo en la lista abierta
    //                     NodeRecord* endNodeRecord = nullptr;
    //                     for (auto& record : open) {
    //                         if (record.node == endNode) {
    //                             endNodeRecord = &record;
    //                             break;
    //                         }
    //                     }

    //                     if (endNodeRecord->costSoFar <= endNodeCost) {
    //                         continue;
    //                     }
    //                     // Podemos usar los valores de costo antiguos del nodo para calcular su heurística sin llamar
    //                     // a la función heurística posiblemente costosa
    //                     endNodeHeuristic = Heuristic(endNodeRecord->node,goalNode);
    //                 } else {
    //                     endNodeRecord = new NodeRecord(0,Conection{0,0,0},0.0,0.0);
    //                     endNodeRecord->node = endNode;
    //                     // Necesitaremos calcular el valor heurístico usando la función, ya que no tenemos un
    //                     // registro existente para usar
    //                     endNodeHeuristic = Heuristic(endNodeRecord->node, goalNode);
    //                 }
    //             }
    //             // Aquí estamos si necesitamos actualizar el nodo
    //             // Actualizar el costo, estimación y conexión
    //             endNodeRecord->costSoFar = endNodeCost;
    //             endNodeRecord->connection = connection;
    //             endNodeRecord->estimatedTotalCost = endNodeCost + endNodeHeuristic;

    //             // Y agregarlo a la lista abierta
    //             if (std::find(open.begin(), open.end(), endNode) == open.end()) {
    //                 open.push_back(*endNodeRecord);
    //             }
    //         }
    //     }

    //     // Hemos terminado de examinar las conexiones para el nodo actual, así que agréguelo a la lista cerrada
    //     // y quítelo de la lista abierta
    //     // Crear una copia de open para evitar la modificación mientras se itera
    //     std::vector<NodeRecord> openCopy = open;

    //     // Eliminar el nodo actual de open
    //     open.erase(std::remove_if(open.begin(), open.end(), [&current](const NodeRecord& record) {
    //         return record.node == current.node;
    //     }), open.end());

    //     // Agregar el nodo actual a closed
    //     closed.insert(current.node);

    //     // Estamos aquí si hemos encontrado el objetivo o
    //     // si no tenemos más nodos para buscar, averiguar cuál es
    //     if (current.node != goalNode) {
    //         // Nos hemos quedado sin nodos sin encontrar el objetivo, así que no hay solución
    //         return std::vector<vec3d>(); // o nullptr si se devuelve un puntero
    //     } else {
    //         // Compilar la lista de conexiones en el camino
    //         std::vector<vec3d> path;
    //         while (current.node != startNode) {
    //             // Agregar el punto de conexión al camino
    //             // Suponiendo que haya un método para obtener la posición de un nodo
    //             path.push_back(Nodes[current.node]);
    //             // Retroceder al nodo anterior
    //             current = *findRecord(open, current.connection.fromNode);
    //         }
    //         // Añadir la posición del nodo inicial
    //         path.push_back(Nodes[startNode]);
    //         // Invertir el camino ya que se construyó desde el objetivo hasta el inicio
    //         std::reverse(path.begin(), path.end());
    //         return path;
    //     }
    // }
    // // Si acaba el bucle y no se ha devuelto un path
    // return std::vector<vec3d>{};