#pragma once
#include "node.hpp"
#include <printf.h>
#include <cmath>
#include <numbers>
#include <algorithm>
#include <utils/sb/steeringbehaviour.hpp>

struct BTAction_Seek : BTNode_t{
    // BTActionPatrol() = default;


    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        if( !ectx.ai.tactive ){
            std::cout << "ME VOY DE SEEK";
             return BTNodeStatus_t::fail;
        }
         std::cout << "TOY SEEK \n";
        Steer_t steering = STBH::Seek(ectx.phy,{ectx.ai.tx,0.0,ectx.ai.tz},ectx.ai.time2arrive);
        ectx.phy.velocity = vec3d{steering.v_x,0.0,steering.v_z};
        return BTNodeStatus_t::success;
    }
};
