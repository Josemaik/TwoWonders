#pragma once
#include "node.hpp"
#include <cmath>
#include <numbers>
#include <algorithm>
#include <utils/sb/steeringbehaviour.hpp>

struct BTAction_Evade : BTNode_t {
    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        if (!ectx.ai->tactive) return BTNodeStatus_t::fail;
        ectx.ai->bh = "Evading";
        //get target entity by id
        auto const& e_opt{ ectx.em.getEntityByID(ectx.ai->teid) };
        if (!e_opt) { ectx.ai->tactive = false; return BTNodeStatus_t::running; };
        auto const& phyTarget = ectx.em.getComponent<PhysicsComponent>(*e_opt);
        auto const distance = (ectx.phy.position - phyTarget.position).lengthSQ();

        // bool evade{false};
        if (ectx.ai->playerdetected && (distance < (ectx.ai->attack_radius * ectx.ai->attack_radius) / 3.0)) {
            if (ectx.ai->elapsed_evade >= ectx.ai->countdown_evade) {
                Steer_t steering = STBH::Evade(ectx.phy, phyTarget, 1.0);
                ectx.phy.velocity = vec3d{ steering.v_x,0.0,steering.v_z };
                if (ectx.ai->elapsed_stop_evade >= 0.5) {
                    ectx.ai->elapsed_stop_evade = 0;
                    ectx.ai->elapsed_evade = 0;
                }
                ectx.ai->plusDeltatime(ectx.deltaTime, ectx.ai->elapsed_stop_evade);
            }
            ectx.ai->plusDeltatime(ectx.deltaTime, ectx.ai->elapsed_evade);
            return BTNodeStatus_t::running;
        }
        else {
            return BTNodeStatus_t::success;
        }
        // ectx.phy.a_linear = steering.linear;
        // ectx.phy.v_angular = steering.angular;

        return BTNodeStatus_t::running;
    }
};
