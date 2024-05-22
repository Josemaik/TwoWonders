#pragma once
#include "node.hpp"
#include <utils/vec3D.hpp>

//Comprobar si la apisonadora ataca
struct BTDecisionReadyforCrusherAttk : BTNode_t {
    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        ectx.ai->bh = "crusher attack?";
        auto& bb = ectx.em.getSingleton<BlackBoard_t>();
        if (ectx.ent.hasComponent<AnimationComponent>()) {
            auto& anc = ectx.em.getComponent<AnimationComponent>(ectx.ent);
            if (anc.aboutToEnd)
            {
                anc.aboutToEnd = false;
                ectx.ai->elapsed_shoot = 0;
                ectx.ai->ready_attack = true;
                bb.playerdamagebycrusher = false;
                return BTNodeStatus_t::success;
            }
        }

        return BTNodeStatus_t::fail;
    }
};