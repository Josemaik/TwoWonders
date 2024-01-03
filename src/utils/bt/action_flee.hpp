#pragma once
#include "node.hpp"
#include <utils/types.hpp>
#include <printf.h>
#include <cmath>
#include <numbers>
#include <algorithm>


struct BTAction_Flee : BTNode_t{
    // BTActionPatrol() = default;
    
    
    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        if( !ectx.ai.tactive ) return BTNodeStatus_t::fail;
        ectx.phy.v_angular = ectx.phy.a_linear = 0;

        // Linear distance to target
        auto vtx {- ectx.ai.tx + ectx.phy.position.x() };
        auto vtz {- ectx.ai.tz + ectx.phy.position.z() };
        
        
        // target orientation 
        auto target_orientation { std::atan2(vtz,vtx) };
        if( target_orientation < 0 ) target_orientation += 2*PI;
        // target angular velocity
        auto angular_distance { target_orientation - ectx.phy.orientation };
        if      ( angular_distance >  PI ) angular_distance -= 2*PI;
        else if ( angular_distance < -PI ) angular_distance += 2*PI;
        auto angular_velocity {angular_distance/ectx.ai.time2arrive};
        ectx.phy.v_angular = std::clamp(angular_velocity, -ectx.phy.kMaxVAng,ectx.phy.kMaxVAng);
         

        // Calculate target linear acceleration based on angular distance
        auto angular_velocity_size { std::fabs(angular_velocity) };
        auto acceleration { ectx.phy.kMaxVLin / (1 + angular_velocity_size) };
        ectx.phy.a_linear = std::clamp( acceleration/ectx.ai.time2arrive , -ectx.phy.kMaxAlin, ectx.phy.kMaxAlin ) ;

        return BTNodeStatus_t::running;
    }
};