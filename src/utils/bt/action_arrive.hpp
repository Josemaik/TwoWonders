#pragma once
#include "node.hpp"
#include <random>
#include <utils/types.hpp>
#include <printf.h>
#include <cmath>
#include <numbers>
#include <algorithm>


struct BTAction_Arrive : BTNode_t{
    // BTActionPatrol() = default;
    
    
    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        if( !ectx.ai.tactive ) return BTNodeStatus_t::fail;

        ectx.phy.v_angular = ectx.phy.a_linear = 0;
        // Linear distance to target
        auto vtx { ectx.ai.tx - ectx.phy.position.x() };
        auto vtz { ectx.ai.tz - ectx.phy.position.z() };
        auto tdist { std::sqrt(vtx*vtx + vtz*vtz) };
        if(tdist < ectx.ai.arrival_radius){
            ectx.ai.tactive = false;
            std::printf("[%d] He llegado! \n",static_cast<int>(ectx.ent.getID()));
            return BTNodeStatus_t::success;
        }
        
        auto tvelocity    = std::clamp( tdist / ectx.ai.time2arrive, -ectx.phy.kMaxVLin, ectx.phy.kMaxVLin );
        ectx.phy.a_linear = std::clamp( tvelocity - ectx.phy.v_linear , -ectx.phy.kMaxAlin, ectx.phy.kMaxAlin ) ;
        
        //velocidad angular 
        auto torien { std::atan2(vtz,vtx) };
        if( torien < 0 ) torien += 2*PI;
        auto vang { torien - ectx.phy.orientation };
        if      ( vang >  PI ) vang -= 2*PI;
        else if ( vang < -PI ) vang += 2*PI;
        ectx.phy.v_angular = std::clamp(vang/ectx.ai.time2arrive, -ectx.phy.kMaxVAng,ectx.phy.kMaxVAng);
        return BTNodeStatus_t::running;
    }
};