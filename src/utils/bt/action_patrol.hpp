#pragma once
#include "node.hpp"
#include <random>
#include <printf.h>
#include <cmath>
#include <numbers>
#include <algorithm>
#include <utils/sb/steeringbehaviour.hpp>

struct BTAction_Patrol : BTNode_t{

    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        if( !ectx.ai.tactive ) return BTNodeStatus_t::fail;
        //Do patrol
        Steer_t steering = STBH::Arrive(ectx.phy,*ectx.ai.pathIt,ectx.ai.time2arrive,ectx.ai.arrival_radius);
        if ( steering == Steer_t{} ){
            ++ectx.ai.pathIt;
        }
        ectx.phy.a_linear = steering.linear;
        ectx.phy.v_angular = steering.angular;
        return BTNodeStatus_t::running;
    }
};
