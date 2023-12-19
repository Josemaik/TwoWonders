#pragma once
#include "node.hpp"
#include <utils/vec3D.hpp>


struct BTDecisionPlayerDetected : BTNode_t{

    BTDecisionPlayerDetected()  {}
    [[nodiscard]] vec3f getPlayerDistance(EntityManager& EM, PhysicsComponent const& p, AIComponent& ai) const noexcept {
        auto& li = EM.getSingleton<LevelInfo>();
        auto* playerEn = EM.getEntityByID(li.playerID);
        if (not playerEn) { ai.playerdetected = false; return vec3f{}; };
        auto& plphy = EM.getComponent<PhysicsComponent>(*playerEn);
        auto const distance = plphy.position - p.position;
        return  distance;
    }
    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        auto& li = ectx.em.getSingleton<LevelInfo>();
        auto* playerEn = ectx.em.getEntityByID(li.playerID);
        if (not playerEn) return BTNodeStatus_t::fail; // No hay player
        // Si hay player
        auto& plphy = ectx.em.getComponent<PhysicsComponent>(*playerEn);
        auto const distance = (ectx.phy.position - plphy.position).lengthSQ();
        //Compruebo si esta dentro del radio de detección
        if( distance < (ectx.ai.detect_radius * ectx.ai.detect_radius)){
            return BTNodeStatus_t::success;
        }
        return BTNodeStatus_t::fail;
    }
    
// private:
//     bool alternative { false };
};