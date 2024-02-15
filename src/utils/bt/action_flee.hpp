#pragma once
#include "node.hpp"
#include <cmath>
#include <numbers>
#include <algorithm>
#include <utils/sb/steeringbehaviour.hpp>

struct BTAction_Flee : BTNode_t {
    // BTActionPatrol() = default;


    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        if (!ectx.ai.tactive) return BTNodeStatus_t::fail;
        ectx.ai.bh = "Fleeing";
        if (ectx.ai.elapsed_fleeing >= ectx.ai.countdown_fleeing) {
            ectx.ai.elapsed_fleeing = 0;
            return BTNodeStatus_t::success;
        }
        else {
            Steer_t steering = STBH::Flee(ectx.phy, { ectx.ai.tx,0.0,ectx.ai.tz });
            ectx.phy.velocity = vec3d{ steering.v_x,0.0,steering.v_z };
            ectx.ai.plusdeltatime(ectx.deltatime, ectx.ai.elapsed_fleeing);
            return BTNodeStatus_t::running;
        }
    }
};
