#pragma once
#include "node.hpp"
#include <utils/types.hpp>


struct BTDecisionPlayerHunted : BTNode_t{
    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        ectx.ai.bh = "Player hunted?";
        if(ectx.em.getSingleton<BlackBoard_t>().playerhunted){
            ectx.phy.max_speed = 0.5;
            return BTNodeStatus_t::success;
        }else{
            ectx.phy.max_speed = 0.3;
            return BTNodeStatus_t::fail;
        }
    }
};
