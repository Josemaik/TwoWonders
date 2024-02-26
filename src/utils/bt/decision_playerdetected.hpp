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
        //Calculamos percepción de vista
        // Calcula la dirección de visión del enemigo utilizando su orientación
        vec3d enemyDirection = vec3d(sin(ectx.phy.orientation),0.0, cos(ectx.phy.orientation));
        // Vector que apunta desde el enemigo al jugador
        vec3d toPlayer = plphy.position - ectx.phy.position;
        //normalizamos vectores
        enemyDirection.normalize();
        toPlayer.normalize();
        //producto escalar
        double dotProduct = enemyDirection.dotProduct(toPlayer);
        // Calcula el ángulo entre la dirección de visión del enemigo y la dirección hacia el jugador
        double angle = acos(dotProduct);
        // Convierte el ángulo de radianes a grados
        double angleDegrees = angle * 180.0f / PI;
        if(ectx.ent.hasTag<SnowmanTag>()){
            std::cout << "AngleDeggreess:" << angleDegrees << "\n";
        }
        // Comprueba si el ángulo está dentro del rango de visión del enemigo
        if (angleDegrees < ectx.ai.field_of_view / 2.0) {
            //Raycast
            bool rayintercepted{false};
            // Realiza un raycast en la dirección en la que el enemigo está mirando para detectar obstáculos
            RayCast ray = { ectx.phy.position, enemyDirection };
            //Compruebo si la caja de colisión de un obstaculo ha colisionado con el rayo
            for(Entity &ent : ectx.em.getEntities()){
                if(ent.hasTag<WallTag>()){
                    auto& col = ectx.em.getComponent<ColliderComponent>(ent);
                    if(col.boundingBox.intersectsRay(ray.origin,ray.direction)){
                        // si es el caso el enemigo no puede detectar al player
                        rayintercepted = true;
                    }
                }
            }
            if(rayintercepted){
                ectx.ai.playerdetected = false;
                return BTNodeStatus_t::fail;
            }else{
                ectx.ai.playerdetected = true;
                return BTNodeStatus_t::success;
            }
        }
        // }else{
        //     // si el campo de visión no detecta al player
        //    ectx.ai.playerdetected = false;
        //    return BTNodeStatus_t::fail;
        // }
        return BTNodeStatus_t::fail;
        //Calculamos percepción por oido
        //Calculo la distancia del player al enemigo
        // auto const distance = (ectx.phy.position - plphy.position).lengthSQ();
        // //Compruebo si esta dentro del radio de detección
        // if( distance < (ectx.ai.detect_radius * ectx.ai.detect_radius)){
        //     if(!ectx.ent.hasTag<BossFinalTag>()){
        //         ectx.ai.path_initialized = false;
        //     }
        //     ectx.ai.playerdetected = true;
        //     return BTNodeStatus_t::success;
        // }else{
        //     ectx.ai.playerdetected = false;
        // }
        
    }

// private:
//     bool alternative { false };
};
