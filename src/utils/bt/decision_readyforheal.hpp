#pragma once
#include "node.hpp"
#include <utils/types.hpp>


struct BTDecisionReadyforHeal : BTNode_t {

    BTDecisionReadyforHeal() {}

    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        ectx.ai->bh = "check ready heal mate";

        if (ectx.ent.hasTag<SlimeTag>()) {
            //obtenemos blackboard
            auto& bb = ectx.em.getSingleton<BlackBoard_t>();
            // si encuentro un slime que tenga menos de 3 vidas, guardo como objetivo su posición
            for (auto& slime : bb.slimeData)
                if (slime.first != ectx.ent.getID()) {
                    ectx.ai->slimex = slime.second.position.x();
                    ectx.ai->slimez = slime.second.position.z();
                    ectx.ai->slimetarget = slime.first;
                    ectx.ai->arrival_radius = 2.0;
                    return BTNodeStatus_t::success;
                }
            return BTNodeStatus_t::success;
        }
        if (ectx.ent.hasTag<BossFinalTag>()) {
            if (ectx.ai->elapsed_heal >= ectx.ai->countdown_heal) {
                ectx.ai->elapsed_heal = 0;
                return BTNodeStatus_t::success;
            }
            else {
                ectx.ai->plusdeltatime(ectx.deltaTime, ectx.ai->elapsed_heal);
            }
        }
        return BTNodeStatus_t::fail;
        // recorrer unorderd map del bb , comprobar si tienen < de 3 vidas y si lo cumple, comprobar si esta en radio de cura. El primero que lo cumpla
        // sale del bucle
        // tx y tz la posicion de ese slime
    }
};
