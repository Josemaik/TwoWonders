#pragma once
#include "node.hpp"
#include <utils/sb/steeringbehaviour.hpp>


struct BTAction_HealMate : BTNode_t{
    double calculatedistance(vec3d const& target, vec3d const& origin) {
        auto dx{ target.x() - origin.x() };
        auto dz{ target.z() - origin.z() };
        return std::sqrt(dx * dx + dz * dz);
    }
    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        if( !ectx.ai.tactive ) return BTNodeStatus_t::fail;
        ectx.ai.bh = "Healing mate";
        std::cout << "txp: " << ectx.ai.slimex << "tzp:" << ectx.ai.slimez << "\n";
        auto const& phyTarget = ectx.em.getComponent<PhysicsComponent>(*ectx.em.getEntityByID(ectx.ai.slimetarget));
        Steer_t steering = STBH::Pursue(phyTarget ,ectx.phy);
        double tdist = calculatedistance(phyTarget.position,ectx.phy.position);
        if(tdist < ectx.ai.arrival_radius){
            //Nos matamos
            ectx.lifec.life = 0;
            ectx.ai.healbeforedie = true;
            std::cout << "He llegadoooooooooo \n";
            return BTNodeStatus_t::success;
        }
        ectx.phy.velocity = vec3d{steering.v_x,0.0,steering.v_z};
        // ectx.phy.v_angular = steering.angular;
        return BTNodeStatus_t::running;
    }
};
