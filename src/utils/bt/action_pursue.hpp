#pragma once
#include "node.hpp"
#include <printf.h>
#include <cmath>
#include <numbers>
#include <algorithm>
#include <utils/sb/steeringbehaviour.hpp>

struct BTAction_Pursue : BTNode_t{  
    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        if( !ectx.ai.tactive ) return BTNodeStatus_t::fail;
        //get target entity by id
        auto& e_opt { ectx.em.getEntityByID(ectx.ai.teid) };
        if ( !e_opt ) { ectx.ai.tactive = false; return BTNodeStatus_t::running};
        auto& phy_e_opt = em.getComponent<PhysicsComponent>(e_opt);
        Steer_t steering = STBH::Pursue(phy_e_opt,ectx.phy,);
        ectx.phy.a_linear = steering.linear;
        ectx.phy.v_angular = steering.angular;
        return BTNodeStatus_t::running;
    }
};