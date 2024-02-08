#pragma once
#include "node.hpp"
#include <utils/types.hpp>


struct BTDecisionSubditosAlreadyGenerated : BTNode_t {
    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        ectx.ai.bh = "subditos already generated?";
        // comprobar radio de cura
        //obtenemos blackboard
        auto& bb = ectx.em.getSingleton<BlackBoard_t>();
        // si encuentro un slime que tenga menos de 3 vidas, guardo como objetivo su posición
        if(bb.subditosData.size() == 0 || bb.subditosData.size() == 1){
            //no hay subditos vivos o hay soolo 1
            return BTNodeStatus_t::success;
        }else{
            // si hay 2 subditos ya vivos
             return BTNodeStatus_t::fail;
        }
    }
};
