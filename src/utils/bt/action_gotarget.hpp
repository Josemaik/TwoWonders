#pragma once
#include "node.hpp"
#include <random>
#include <utils/types.hpp>
#include <printf.h>
#include <cmath>
#include <numbers>
#include <algorithm>


struct BTAction_goTarget : BTNode_t{
    // BTActionPatrol() = default;
    
    
    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        if( !ectx.ai.tactive ) return BTNodeStatus_t::success;

        ectx.phy.v_angular = ectx.phy.v_linear = 0;

        auto vtx { ectx.ai.tx - ectx.phy.position.x() };
        auto vty { ectx.ai.ty - ectx.phy.position.y() };
        auto vtlin { std::sqrt(vtx*vtx + vty*vty) };
        ectx.phy.v_linear = std::clamp( vtlin, -ectx.phy.kMaxVLin, ectx.phy.kMaxVLin );
        
        //velocidad angular 
        auto torien { std::atan2(vty,vtx) };
        if( torien < 0 ) torien += 2*PI;
        auto vang { torien - ectx.phy.orientation };
        ectx.phy.v_angular = std::clamp(vang, -ectx.phy.kMaxVAng,ectx.phy.kMaxVAng);
        return BTNodeStatus_t::success;
    }
};