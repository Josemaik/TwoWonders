#pragma once
#include "node.hpp"
#include <utils/vec3D.hpp>


struct BTDecisionReadyforCrusherAttk : BTNode_t{
    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        ectx.ai->bh = "crusher attack?";
        auto& bb = ectx.em.getSingleton<BlackBoard_t>();
        if(ectx.ai->elapsed_shoot >= ectx.ai->countdown_shoot){
            ectx.ai->elapsed_shoot = 0;
            ectx.ai->ready_attack = true;
            bb.playerdamagebycrusher = false;
            return BTNodeStatus_t::success;
        }
        ectx.ai->plusdeltatime(ectx.deltatime,ectx.ai->elapsed_shoot);
        return BTNodeStatus_t::fail;
    }
};