#pragma once
#include "node.hpp"
#include <utils/types.hpp>


struct BTActionShoot : BTNode_t{
    // BTActionPatrol() = default;

    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
         if (ectx.ai.elapsed_shoot >= ectx.ai.countdown_shoot) {
            ectx.ai.elapsed_shoot = 0;
            //shoot one time
            auto& att = ectx.em.getComponent<AttackComponent>(ectx.ent);
            att.attack(AttackType::TripleShot);
         }
        ectx.ai.dec_countdown_shoot(ectx.deltatime);
        return BTNodeStatus_t::success;
    }
};