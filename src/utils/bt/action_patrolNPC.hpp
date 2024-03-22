#pragma once
#include "node.hpp"
#include <random>
#include <cmath>
#include <numbers>
#include <algorithm>
#include <utils/sb/steeringbehaviour.hpp>

struct BTAction_PatrolNPC : BTNode_t {

    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        //ectx.ai->bh = "patrolling";
        if (!ectx.npc->path_initialized) {
            ectx.npc->pathIt = ectx.npc->path.begin();
            ectx.npc->path_initialized = true;
        }

        //Do patrol
        auto& li = ectx.em.getSingleton<LevelInfo>();
        if(!ectx.npc->path_finaliced && li.door_open){
            Steer_t steering = STBH::Arrive(ectx.phy, *ectx.npc->pathIt, ectx.npc->arrival_radius);
            if (steering.arrived) {
                if(*ectx.npc->pathIt == *ectx.npc->path.end()){
                    ectx.npc->path_finaliced = true;
                }else{
                    ++ectx.npc->pathIt;
                }
            }
            ectx.phy.velocity = vec3d{ steering.v_x,0.0,steering.v_z };
        }
        return BTNodeStatus_t::success;
    }
};
