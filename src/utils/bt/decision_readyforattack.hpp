#pragma once
#include "node.hpp"
#include <utils/vec3D.hpp>


struct BTDecisionReadyforAttack : BTNode_t {

    BTDecisionReadyforAttack() {}

    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        ectx.ai->bh = "check ready attack";
        auto& li = ectx.em.getSingleton<LevelInfo>();
        auto* playerEn = ectx.em.getEntityByID(li.playerID);
        if (not playerEn) {
            return BTNodeStatus_t::fail; // No hay player
        }
        // Si hay player
        auto& plphy = ectx.em.getComponent<PhysicsComponent>(*playerEn);
        auto const distance = (ectx.phy.position - plphy.position).lengthSQ();
        //Compruebo si esta dentro del radio de ataque y se acabo el culldown
        //ectx.phy.orientated_before = false;
        if (ectx.ai->playerdetected) {
            if (distance < (ectx.ai->attack_radius * ectx.ai->attack_radius)) {
                ectx.ai->on_attack_radius = true;

                //Oriento hacia el jugador
                // if(!ectx.phy.orientated_to_player){
                //     vec3d direction = plphy.position - ectx.phy.position;
                //     auto target_orientation = atan2(direction.z(), direction.x());
                //     if (target_orientation < 0) target_orientation += 2 * K_PI;
                //     adjustAnglePiMinusPi(target_orientation);
                //     ectx.phy.orientation = target_orientation;
                //     double angle_difference = std::abs(ectx.phy.orientation - target_orientation);
                //     if (angle_difference < 0.1) {
                //         ectx.phy.orientated_to_player = true;
                //     }
                // }
                // // ectx.phy.orientated_before = true;

                if (ectx.ai->elapsed_shoot >= ectx.ai->countdown_shoot) {
                    // paro al enemigo
                    ectx.phy.velocity = vec3d{};
                    if (ectx.ai->elapsed_stop >= ectx.ai->countdown_stop) {
                        ectx.ai->elapsed_shoot = 0;
                        ectx.ai->elapsed_stop = 0;
                        ectx.ai->ready_attack = true;
                        // ectx.phy.v_linear = 0;
                        return BTNodeStatus_t::success;
                    }
                    ectx.ai->plusDeltatime(ectx.deltaTime, ectx.ai->elapsed_stop);
                    //reinicio culldown
                    return BTNodeStatus_t::running;
                    // activo ataque
                }
                else {
                    ectx.ai->plusDeltatime(ectx.deltaTime, ectx.ai->elapsed_shoot);
                    return BTNodeStatus_t::success;
                }
            }
            else {
                return BTNodeStatus_t::fail;
            }
        }
        return BTNodeStatus_t::fail;
    }
};
