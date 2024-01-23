#pragma once
#include "node.hpp"
#include <utils/sb/steeringbehaviour.hpp>

struct BTAction_HealMate : BTNode_t{
    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        if( !ectx.ai.tactive ) return BTNodeStatus_t::fail;
        ectx.ai.bh = "Healing mate";
        Steer_t steering = STBH::Arrive(ectx.phy,vec3d{ectx.ai.txp,0.0,ectx.ai.tzp},ectx.ai.arrival_radius);
        if(steering.v_x == 0 && steering.v_z == 0){
            //Nos matamos
            ectx.lifec.life = 0;
            return BTNodeStatus_t::success;
        }
        ectx.phy.velocity = vec3d{steering.v_x,0.0,steering.v_z};
        // ectx.phy.v_angular = steering.angular;
        return BTNodeStatus_t::running;
    }
};
