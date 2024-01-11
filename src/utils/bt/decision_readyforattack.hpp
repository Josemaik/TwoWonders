#pragma once
#include "node.hpp"
#include <utils/vec3D.hpp>


struct BTDecisionReadyforAttack : BTNode_t{

    BTDecisionReadyforAttack()  {}

    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        std::cout << "Compruebo ataque \n";
        auto& li = ectx.em.getSingleton<LevelInfo>();
        auto* playerEn = ectx.em.getEntityByID(li.playerID);
        if (not playerEn) {
            std::cout << "ME VOY DE ATTACK";
            return BTNodeStatus_t::fail; // No hay player
        }
        // Si hay player
        auto& plphy = ectx.em.getComponent<PhysicsComponent>(*playerEn);
        auto const distance = (ectx.phy.position - plphy.position).lengthSQ();
        //Compruebo si esta dentro del radio de ataque y se acabo el culldown
        if(distance < (ectx.ai.attack_radius * ectx.ai.attack_radius) &&
        ectx.ai.elapsed_shoot >= ectx.ai.countdown_shoot){
            std::cout << "Atacar player \n";
            // paro al enemigo
            ectx.phy.velocity = vec3d{};
            if(ectx.ai.elapsed_stop >= ectx.ai.countdown_stop){
                ectx.ai.elapsed_shoot = 0;
                ectx.ai.elapsed_stop = 0;
                ectx.ai.ready_attack = true;
                // ectx.phy.v_linear = 0;
                return BTNodeStatus_t::success;
            }
            ectx.ai.dec_countdown_stop(ectx.deltatime);
            //reinicio culldown
             return BTNodeStatus_t::running;
            // activo ataque
        }else{
             ectx.ai.dec_countdown_shoot(ectx.deltatime);
            return BTNodeStatus_t::success;
        }

        //decremento tiempo culldown

        //go to next node


    }

// private:
//     bool alternative { false };
};
