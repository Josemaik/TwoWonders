#pragma once
#include "node.hpp"
#include <utils/types.hpp>


struct BTDAction_GenerateSubditos : BTNode_t {
    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        ectx.ai.bh = "generating subditos";
        // comprobar radio de cura
        //obtenemos blackboard
        auto& bb = ectx.em.getSingleton<BlackBoard_t>();
        if(bb.subditosData.size() == 0){
            //genero 2 subditos

        }else{
            //genero 1 subdito
        }
    }
};
