#pragma once
#include "node.hpp"
#include <utils/types.hpp>

//Comprobar si has cazado al player
struct BTDecisionPlayerHunted : BTNode_t {
    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        ectx.ai->bh = "Player hunted?";
        if (ectx.em.getSingleton<BlackBoard_t>().playerhunted) {
            if (!ectx.ai->boofedvelocity) {
                ectx.phy.max_speed += 0.2;
            }
            ectx.ai->boofedvelocity = true;
            ectx.ai->attack_radius = 5.0;
            ectx.ai->countdown_shoot = 0.05;
            ectx.ai->countdown_stop = 0.05;
            return BTNodeStatus_t::success;
        }
        else {
            if (ectx.ai->boofedvelocity) {
                ectx.phy.max_speed = 0.2;
            }
            ectx.ai->boofedvelocity = false;
            ectx.ai->attack_radius = 20.5;
            ectx.ai->countdown_shoot = 0.3;
            ectx.ai->countdown_stop = 0.6;
            return BTNodeStatus_t::fail;
        }
    }
};
