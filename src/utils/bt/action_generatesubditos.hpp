#pragma once
#include "node.hpp"
#include <utils/types.hpp>


struct BTDAction_GenerateSubditos : BTNode_t {
    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        ectx.ai->bh = "generating subditos";
        // comprobar radio de cura
        //obtenemos blackboard
        if (ectx.ai->elapsed_spawning >= ectx.ai->couldown_spawning) {
            ectx.ai->elapsed_spawning = 0;
            auto& bb = ectx.em.getSingleton<BlackBoard_t>();
            bb.create_subdito = true;
            //debilitar
            ectx.lifec->decreaseLife();
            return BTNodeStatus_t::fail;
        }
        else {
            ectx.ai->plusDeltatime(ectx.deltaTime, ectx.ai->elapsed_spawning);
        }
        return BTNodeStatus_t::fail;
    }
};
