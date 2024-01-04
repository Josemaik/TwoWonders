#pragma once
#include "node.hpp"
#include <random>
#include <printf.h>
#include <cmath>
#include <numbers>
#include <algorithm>
#include <utils/sb/steeringbehaviour.hpp>

struct BTAction_Arrive : BTNode_t{
    // BTActionPatrol() = default;
    
    
    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        if( !ectx.ai.tactive ) return BTNodeStatus_t::fail;
        Steer_t steering = STBH::Arrive(ectx.phy,{ectx.ai.tx,0.0,ectx.ai.tz},ectx.ai.time2arrive,ectx.ai.arrival_radius);
        if(steering.linear == 0 && steering.angular == 0){
            return BTNodeStatus_t::success;
        }
        ectx.phy.a_linear = steering.linear;
        ectx.phy.v_angular = steering.angular;
        return BTNodeStatus_t::running;
    }
};