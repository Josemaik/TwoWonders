#pragma once
#include "node.hpp"
#include <utils/sb/steeringbehaviour.hpp>

struct BTAction_Pendulum : BTNode_t {

    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        if (!ectx.ai.tactive) return BTNodeStatus_t::fail;
        ectx.ai.bh = "penduling";
        if (!ectx.ai.path_initialized) {
            ectx.ai.pathIt = ectx.ai.path.begin();
            ectx.ai.path_initialized = true;
        }
        if(ectx.ai.chargeattack){
                ectx.phy.velocity = vec3d{};
                if (ectx.ai.elapsed_stop >= ectx.ai.countdown_stop) {
                    ectx.ai.elapsed_stop = 0;
                    ectx.ai.chargeattack = false;
                }else{
                    ectx.ai.plusdeltatime(ectx.deltatime,ectx.ai.elapsed_stop);
                }
                 return BTNodeStatus_t::success;
        }

        Steer_t steering = STBH::Arrive(ectx.phy, *ectx.ai.pathIt, ectx.ai.arrival_radius);
        ectx.phy.max_speed += 0.1;
        if (steering.arrived) {
            ++ectx.ai.pathIt;
            ectx.phy.max_speed = 0.4;
            ectx.ai.chargeattack = true;
            return BTNodeStatus_t::success;
        }
        ectx.phy.velocity = vec3d{ steering.v_x,0.0,steering.v_z };
        return BTNodeStatus_t::success;
    }
};