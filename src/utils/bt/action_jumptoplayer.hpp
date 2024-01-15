#pragma once
#include "node.hpp"
#include <printf.h>
#include <cmath>
#include <numbers>
#include <algorithm>
#include <utils/sb/steeringbehaviour.hpp>

struct BTAction_JumptoPlayer : BTNode_t{
    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        if( !ectx.ai.tactive ) return BTNodeStatus_t::fail;
         if(ectx.ai.ready_attack == true){
            if(!ectx.ai.target_obtained){
                ectx.ai.txp = ectx.ai.tx;
                ectx.ai.tzp = ectx.ai.tz;
                ectx.ai.target_obtained = true;
            }
            //std::cout << "TOY JUMPING \n";
            ectx.phy.max_speed = 0.6;
            Steer_t steering = STBH::Arrive(ectx.phy,{ectx.ai.txp,0.0,ectx.ai.tzp},ectx.ai.arrival_radius);
            if(steering.arrived) {
                ectx.phy.max_speed = 0.5;
                ectx.phy.velocity = {};
                ectx.ai.target_obtained = false;
                return BTNodeStatus_t::success;
            }
            ectx.phy.velocity = vec3d{steering.v_x,0.0,steering.v_z};
            return BTNodeStatus_t::running; // El salto está en curso
        }
        return BTNodeStatus_t::success;
    }
};
