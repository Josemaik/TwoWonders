#pragma once
#include "node.hpp"
#include <utils/types.hpp>


struct BTDecisionPlayerHunted : BTNode_t{
    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        ectx.ai->bh = "Player hunted?";
        if(ectx.em.getSingleton<BlackBoard_t>().playerhunted){
            if(!ectx.ai->boofedvelocity){
                ectx.phy.max_speed += 0.2;
            }
            ectx.ai->boofedvelocity = true;
            return BTNodeStatus_t::success;
        }else{
            if(ectx.ai->boofedvelocity){
                ectx.phy.max_speed -= 0.2;
            }
            ectx.ai->boofedvelocity = false;
            return BTNodeStatus_t::fail;
        }
    }
};
