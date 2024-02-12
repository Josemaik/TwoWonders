#pragma once
#include "node.hpp"
#include <utils/types.hpp>


struct BTDecisionActivateorQuit_Shield : BTNode_t {
    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        ectx.ai.bh = "Activate or quit shield";
        if (ectx.ai.elapsed_shield >= ectx.ai.countdown_shield) {
            ectx.ai.elapsed_shield = 0;
            auto& bb = ectx.em.getSingleton<BlackBoard_t>();
            bb.activate_shield = !bb.activate_shield;
            if (bb.activate_shield) {
                std::cout << bb.subditosData.size() << "\n";
                if (bb.subditosData.size() != 0) {
                    for (const auto& sub : bb.subditosData) {
                        if ((*ectx.em.getEntityByID(sub.first)).hasComponent<RenderComponent>()) {
                            auto& e{ ectx.em.newEntity() };
                            auto& r = ectx.em.addComponent<RenderComponent>(e, RenderComponent{ .position = ectx.em.getComponent<RenderComponent>(*ectx.em.getEntityByID(sub.first)).position, .scale = { 2.0f, 1.0f, 0.5f }, .color = DARKBROWN });
                            auto& p = ectx.em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position = r.position, .scale = r.scale });
                            ectx.em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::SUBDITOSHIELD });
                            ectx.em.addComponent<ShieldComponent>(e, ShieldComponent{ .activeShield = true,.createdby = EntitieswithShield::Subdito,.id_subdito = sub.first });
                            ectx.em.addComponent<LifeComponent>(e, LifeComponent{ .life = 2 });
                        }
                    }
                }
            }
            else {

            }
        }
        ectx.ai.plusdeltatime(ectx.deltatime, ectx.ai.elapsed_shield);
        return BTNodeStatus_t::fail;
    }
};
