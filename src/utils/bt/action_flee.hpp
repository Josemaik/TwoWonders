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
        // if( !ectx.ai.tactive ) return BTNodeStatus_t::fail;
        // Steer_t steering = STBH::Flee(ectx.phy,{ectx.ai.tx,0.0,ectx.ai.tz},ectx.ai.time2arrive);
        // ectx.phy.a_linear = steering.linear;
        // ectx.phy.v_angular = steering.angular;
        return BTNodeStatus_t::running;
    }
};
