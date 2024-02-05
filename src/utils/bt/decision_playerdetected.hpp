#pragma once
#include "node.hpp"
#include <utils/vec3D.hpp>


struct BTDecisionPlayerDetected : BTNode_t{

    BTDecisionPlayerDetected()  {}

    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
         ectx.ai.bh = "detecting player";
        auto& li = ectx.em.getSingleton<LevelInfo>();
        auto* playerEn = ectx.em.getEntityByID(li.playerID);
        if (not playerEn) return BTNodeStatus_t::fail; // No hay player
        // Si hay player
        auto& plphy = ectx.em.getComponent<PhysicsComponent>(*playerEn);
        auto const distance = (ectx.phy.position - plphy.position).lengthSQ();
        //Compruebo si esta dentro del radio de detección
        if( distance < (ectx.ai.detect_radius * ectx.ai.detect_radius)){
            ectx.ai.path_initialized = false;
            ectx.ai.playerdetected = true;
            return BTNodeStatus_t::success;
        }else{
            ectx.ai.playerdetected = false;
        }
        return BTNodeStatus_t::fail;
    }

// private:
//     bool alternative { false };
};
