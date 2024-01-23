#pragma once
#include "node.hpp"
#include <utils/types.hpp>


struct BTDecisionReadyforHeal : BTNode_t{

    BTDecisionReadyforHeal()  {}

    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
         ectx.ai.bh = "check ready heal mate";
        // comprobar radio de cura
        //obtenemos blackboard
        auto& bb = ectx.em.getSingleton<BlackBoard_t>();
        // si encuentro un slime que tenga menos de 3 vidas, guardo como objetivo su posición
        for(auto& slime : bb.slimeData)
            if(slime.second.life < 3){
                ectx.ai.txp = slime.second.position.x();
                ectx.ai.tzp = slime.second.position.z();
                return BTNodeStatus_t::success;
            }
        return BTNodeStatus_t::fail;
        // recorrer unorderd map del bb , comprobar si tienen < de 3 vidas y si lo cumple, comprobar si esta en radio de cura. El primero que lo cumpla
        // sale del bucle
        // tx y tz la posicion de ese slime
    }
};
