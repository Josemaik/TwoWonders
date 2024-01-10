#pragma once
#include "node.hpp"
#include <utils/vec3D.hpp>


struct BTDecisionReadyforAttack : BTNode_t{

    BTDecisionReadyforAttack()  {}

    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        auto& li = ectx.em.getSingleton<LevelInfo>();
        auto* playerEn = ectx.em.getEntityByID(li.playerID);
        if (not playerEn) {
            std::cout << "ME VOY DE ATTACK";
            return BTNodeStatus_t::fail; // No hay player
        }
        // Si hay player
        auto& plphy = ectx.em.getComponent<PhysicsComponent>(*playerEn);
        auto const distance = (ectx.phy.position - plphy.position).lengthSQ();
        //Compruebo si esta dentro del radio de detección
        if( distance < (ectx.ai.attack_radius * ectx.ai.attack_radius)){
            std::cout << "Atacar player \n";
            ectx.phy.v_linear = 0;
            return BTNodeStatus_t::running;
        } else{
             return BTNodeStatus_t::success;
        }

    }

// private:
//     bool alternative { false };
};
