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
        bool state = STBH::Arrive(ectx.ai,ectx.phy,ectx.ent);
        if ( state ) {
            return BTNodeStatus_t::success;
        }
        return BTNodeStatus_t::running;
    }
};