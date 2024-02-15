#pragma once
#include "node.hpp"
#include <utils/types.hpp>


struct BTDecisionActivateorQuit_Shield : BTNode_t {
    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        ectx.ai.bh = "Activate or quit shield";
        if (ectx.ai.elapsed_shield >= ectx.ai.countdown_shield) {
            ectx.ai.elapsed_shield = 0;
            auto& bb = ectx.em.getSingleton<BlackBoard_t>();
            //bb.activate_shield = !bb.activate_shield;
           // ectx.ren.colormutable = !ectx.ren.colormutable;
            // if(bb.activate_shield){
            // std::cout << bb.subditosData.size() << "\n";
            if (bb.subditosData.size() != 0) {
                for (const auto& sub : bb.subditosData) {
                    if ((*ectx.em.getEntityByID(sub.first)).hasComponent<RenderComponent>()) {
                        auto& subComp = ectx.em.getComponent<SubjectComponent>(*ectx.em.getEntityByID(sub.first));
                        subComp.activeShield = !subComp.activeShield;
                    }
                }
            }
        }
        ectx.ai.plusdeltatime(ectx.deltatime, ectx.ai.elapsed_shield);
        return BTNodeStatus_t::fail;
    }
};
