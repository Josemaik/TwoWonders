#pragma once
#include "node.hpp"
#include <random>
#include <cmath>
#include <numbers>
#include <algorithm>
#include <utils/sb/steeringbehaviour.hpp>

struct BTAction_Patrol : BTNode_t {

    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        if (!ectx.ai->tactive) return BTNodeStatus_t::fail;
        ectx.ai->bh = "patrolling";
        vec3d current{},next{};
        if (!ectx.ai->path_initialized) {
            ectx.ai->pathIt = ectx.ai->path.begin();
            ectx.ai->path_initialized = true;
        }

        //Enemigos que no patruyen y esten estáticos
        current =  *ectx.ai->pathIt;
        if(!ectx.ai->followpatrol){
            double xd = ectx.phy.position.distance(current);
            if(xd <= 1.0){
                ectx.phy.velocity = vec3d{};
                // if(ectx.phy.orientation != ectx.phy.previousState.orientation)
                ectx.phy.orientation = ectx.phy.orientationonrespawn;
                return BTNodeStatus_t::success;
            }
        }

        //Do patrol
        Steer_t steering = STBH::Arrive(ectx.phy, *ectx.ai->pathIt, ectx.ai->arrival_radius);
        if (steering.arrived) {
            //vec3d last = *ectx.ai->pathIt;
            ++ectx.ai->pathIt;
            // vec3d newpos = *ectx.ai->pathIt;
            // if(newpos.x() == last.z() && newpos.z() == last.z()){
            //     ectx.phy.velocity = vec3d{};

            // }
        }
        ectx.phy.velocity = vec3d{ steering.v_x,0.0,steering.v_z };
        return BTNodeStatus_t::success;
    }
};
