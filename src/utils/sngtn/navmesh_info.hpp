#pragma once

#include "../../components/navmesh_component.hpp"
#include "../pf/Conection.hpp"
#include <vector>
#include <map>

struct NavmeshInfo
{
    /* data */
    //Contador de ids de los nodos
    uint16_t num_nodes{};
    //Navmeshes
    std::vector<Navmesh> NavMeshes{};
    //Nodos
    //todos
    std::map<uint16_t, vec3d> nodes{};
    //seleccionados
    std::map<uint16_t, vec3d> selectednodes;
    //Pares de ndos que forman una conexion
    std::set<std::pair<uint16_t, uint16_t>> conexids;
    //Puntos medios de los nodos
    std::set<vec3d> midpoints{};
    //centers
    std::set<std::pair<uint16_t, vec3d>> centers{};
    //only nodes of quads
    std::set<vec3d> corners{};
    //Conexiones
    std::vector<Conection> conexiones;
    //Conexiones pero con posiciones
    std::set<std::pair<vec3d, vec3d>> conexpos{};
    //vector de bounding boxes
    std::vector<BBox> boundingnavmesh{};

    // recorrer navmeshes y rellenar array de nodos sin repetir
    // cojo el componente collider y compruebo si colisiona con otra entidad navmes
    // recorro los nodos de ambos y elimino los que se repitan
    // luego creo las conexiones
    // al añadirla comprobar que no exista ya
    bool insert_ids(uint16_t id1, uint16_t id2) {
        uint16_t min{}, max{};
        if (id1 < id2) {
            min = id1;
            max = id2;
        }
        else {
            max = id1;
            min = id2;
        }
        auto pair = std::make_pair(min, max);
        if (conexids.find(pair) == conexids.end()) {
            conexids.insert(pair);
            return true;
        }
        return false;
    }

    bool checkmidpoints(vec3d p1, vec3d p2) {
        vec3d puntoMedio;
        puntoMedio.setX((p1.x() + p2.x()) / 2);
        puntoMedio.setY((p1.y() + p2.y()) / 2);
        puntoMedio.setZ((p1.z() + p2.z()) / 2);
        if (midpoints.find(puntoMedio) == midpoints.end()) {
            return true;
        }
        return false;
    }

    //hacer una funcion que compruebe que los nodos sean primos excepto
    //cuando sean centros que se encuentran a la misma altura.
    bool checkcousins(vec3d p, vec3d c){
        //Si no son centros pero son primos
        if(std::abs(c.x() - p.x()) < 0.05 && std::abs(c.z() - p.z()) < 0.05 && 
        std::abs(c.y() - p.y()) < 0.05){
            return true;
        }
        return false;
    }
};
