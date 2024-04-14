#pragma once
#include "node.hpp"
#include <random>
#include <cmath>
#include <numbers>
#include <algorithm>
#include <utils/sb/steeringbehaviour.hpp>

struct BTAction_PatrolNPC : BTNode_t {
    template <size_t N>
    bool calculateSteering(EntityContext_t& ectx, typename Path_t<N>::iterator& pathIt, Path_t<N>& path) noexcept {
        Steer_t steering = STBH::Arrive(ectx.phy, *pathIt, ectx.npc->arrival_radius);
        if (steering.arrived) {
            if (*pathIt == *path.end()) {
                ectx.npc->path_finaliced = true;
                return true;
            } else {
                ++pathIt;
            }
        }
        ectx.phy.velocity = vec3d{ steering.v_x, 0.0, steering.v_z };
        return false;
    }


    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        //ectx.ai->bh = "patrolling";
        if (!ectx.npc->path_initialized) {
            ectx.npc->pathIt = ectx.npc->path.begin();
            ectx.npc->path_initialized = true;
        }

        //Do patrol
        auto& li = ectx.em.getSingleton<LevelInfo>();
        if(ectx.ent.hasTag<NomadTag>()){
            if(!ectx.npc->path_finaliced && (li.door_open || ectx.npc->tp)){
               if(calculateSteering<4>(ectx, ectx.npc->pathIt, ectx.npc->path)){
                    ectx.npc->tp = false;
                    ectx.npc->path_finaliced = true;
               }
            }
        }
        if(ectx.ent.hasTag<InvestigatorTag>()){
             if(!ectx.npc->path_finaliced && li.investigatorstartwalk && !ectx.npc->tp){
               if(calculateSteering<4>(ectx, ectx.npc->pathIt, ectx.npc->path)){
                    //Si ha terminado la primera vex hace tp
                    ectx.phy.position = vec3d{ 27.2022,14,-104.252};
                    ectx.npc->tp = true;
                    ectx.npc->path_finaliced = true;
                    ectx.npc->pathIt_inestigador = ectx.npc->path_investigador.begin();
               }
             }else{
                if( ectx.npc->tp == true && !ectx.npc->path_investigador_finalized){
                    if(calculateSteering<5>(ectx, ectx.npc->pathIt_inestigador, ectx.npc->path_investigador)){
                        std::cout << "he terminado";
                        ectx.npc->path_investigador_finalized = true;
                        //rotar al npc
                        //quitar mas adelante
                        li.npcflee = true;
                    }
                }
             }
        }
        return BTNodeStatus_t::success;
    }
};
