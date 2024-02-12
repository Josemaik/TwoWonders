#pragma once
#include "node.hpp"
#include <utils/vec3D.hpp>


struct BTDecisionReadyforAirAttack : BTNode_t{
    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        ectx.ai.bh = "air attack?";
        if(ectx.ai.elapsed_air_attack >= ectx.ai.countdown_air_attack){
            ectx.ai.elapsed_air_attack = 0;
            ectx.ai.ready_attack = true;
            return BTNodeStatus_t::success;
        }
        ectx.ai.plusdeltatime(ectx.deltatime,ectx.ai.elapsed_air_attack);
        return BTNodeStatus_t::fail;
    }
};