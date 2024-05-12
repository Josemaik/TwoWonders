#pragma once
#include "node.hpp"
#include <random>
#include <cmath>
#include <numbers>
#include <algorithm>
#include <utils/sb/steeringbehaviour.hpp>

static const double dist_to_spawn{ 35.0 };
struct BTAction_Patrol : BTNode_t {
    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        if (!ectx.ai->tactive) return BTNodeStatus_t::fail;
        ectx.ai->bh = "patrolling";
        vec3d current{}, next{};
        if (!ectx.ai->path_initialized) {
            ectx.ai->pathIt = ectx.ai->path.begin();
            ectx.ai->path_initialized = true;
        }
        //Enemigos que no patruyen y esten estáticos
        current = *ectx.ai->pathIt;
        if (!ectx.ai->followpatrol) {
            double xd = ectx.phy.position.distance(current);
            if (xd <= 1.0) {
                ectx.phy.velocity = vec3d{};
                // if(ectx.phy.orientation != ectx.phy.previousState.orientation)
                ectx.phy.orientation = ectx.phy.orientationonrespawn;
                return BTNodeStatus_t::success;
            }
        }

        //Pathfinding
        auto& li = ectx.em.getSingleton<LevelInfo>();
        auto& navs = ectx.em.getSingleton<NavmeshInfo>();
        //check distance
        if (!ectx.ai->check_distance && li.mapID == 2) {
            vec3d begin = *ectx.ai->path.begin();
            double disaux = ectx.phy.position.distance(begin);
            // double dify = std::abs((ectx.phy.position.y()-2.0)  - begin.y());
            if (ectx.ai->seeking && disaux >= 3.0) {
                //calculate path 
                auto& debug = ectx.em.getSingleton<Debug_t>();
                uint16_t startnode = findNearestNode(ectx.em, ectx.phy.position, navs.selectednodes);
                uint16_t endnode = findNearestNode(ectx.em, *ectx.ai->path.begin(), navs.selectednodes);
                if (startnode != endnode) {
                    ectx.ai->found_path = li.level_graph.PathFindAStar(debug,
                        startnode, endnode);
                    //creamos iterador que apunte a la primera posición
                    if (ectx.ai->found_path.size() != 0) {
                        ectx.ai->it_path = &ectx.ai->found_path[0];
                        ectx.ai->check_distance = true;
                        ectx.ai->pathfind_active = true;
                        ectx.ai->seeking = false;
                    }
                }
                else {
                    ectx.ai->pathfind_active = false;
                    ectx.ai->seeking = true;
                    ectx.ai->check_distance = false;
                }
            }
        }
        //find path
        Steer_t steering{};
        if (ectx.ai->pathfind_active && ectx.ai->found_path.size() != 0) {
            //run found path
            steering = STBH::Arrive(ectx.phy, *ectx.ai->it_path, ectx.ai->arrival_radius);

            if (*ectx.ai->it_path == ectx.ai->found_path[ectx.ai->found_path.size() - 1]) {
                ectx.ai->pathfind_active = false;
                ectx.ai->check_distance = false;
                ectx.ai->found_path.clear();
            }
            if (steering.arrived) {
                ++ectx.ai->it_path;
            }
            //booleano pathcalculated = true;
            //pathfind_active = true
        }
        else {
            //Do patrol
            steering = STBH::Arrive(ectx.phy, *ectx.ai->pathIt, ectx.ai->arrival_radius);
            if (steering.arrived) {
                ++ectx.ai->pathIt;
            }
        }
        //path_calculated == true && !pathfind_finished
            // recorrer path como se hace el patrol
            // pathfind_finished = true
            // path_calculated = false;
            // pathfind_active = false

        //pathfind_finished == true && path_calculated == true

        //Do patrol
        //if pathfind_active == true
            // if pathfind_finished == true
                //patrol
        //else
            //patrol

        ectx.phy.velocity = vec3d{ steering.v_x,0.0,steering.v_z };
        return BTNodeStatus_t::success;
    }

private:
    uint16_t findNearestNode(EntityManager& em, const vec3d& position, const std::map<uint16_t, vec3d>& nodes) {
        uint16_t nearestNodeId = 0; // Suponemos que el primer nodo es el más cercano inicialmente
        double minDistance = std::numeric_limits<double>::max(); // Inicializamos la distancia mínima con un valor muy grande
        vec3d nearestpos{};
        for (const auto& node : nodes) {
            double dist = position.distance(node.second); // Calculamos la distancia entre la posición y el nodo actual
            if (dist < minDistance) { // Si encontramos un nodo más cercano
                minDistance = dist;
                nearestNodeId = node.first;
                nearestpos = node.second;
            }
        }
        auto& debug = em.getSingleton<Debug_t>();
        debug.nodes.push_back(nearestpos);
        return nearestNodeId;
    }
};
