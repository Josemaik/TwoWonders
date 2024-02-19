#pragma once
#include "node.hpp"
#include <cmath>
#include <numbers>
#include <algorithm>
#include <utils/sb/steeringbehaviour.hpp>

struct BTAction_Pursue : BTNode_t {
    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        if (!ectx.ai.tactive) return BTNodeStatus_t::fail;
        ectx.ai.bh = "pursuing";
        //get target entity by id
        auto const& e_opt{ ectx.em.getEntityByID(ectx.ai.teid) };
        if (!e_opt) { ectx.ai.tactive = false; return BTNodeStatus_t::running; };
        auto const& phyTarget = ectx.em.getComponent<PhysicsComponent>(*e_opt);
        Steer_t steering = STBH::Pursue(phyTarget, ectx.phy);
        // ectx.phy.a_linear = steering.linear;
        // ectx.phy.v_angular = steering.angular;
        ectx.phy.velocity = vec3d{ steering.v_x,0.0,steering.v_z };
        return BTNodeStatus_t::success;
    }
};
