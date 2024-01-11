#pragma once
#include "node.hpp"
#include <printf.h>
#include <cmath>
#include <numbers>
#include <algorithm>
#include <utils/sb/steeringbehaviour.hpp>

struct BTAction_Flee : BTNode_t{
    // BTActionPatrol() = default;


    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        if( !ectx.ai.tactive ) return BTNodeStatus_t::fail;
        Steer_t steering = STBH::Flee(ectx.phy,{ectx.ai.tx,0.0,ectx.ai.tz});
        // ectx.phy.a_linear = steering.linear;
        // ectx.phy.v_angular = steering.angular;
        ectx.phy.velocity = vec3d{steering.v_x,0.0,steering.v_z};
        return BTNodeStatus_t::running;
    }
};
