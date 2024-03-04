#pragma once
#include "node.hpp"
#include <utils/vec3D.hpp>


struct BTDecisionPlayerDetected : BTNode_t{
    // Constantes para ángulos de visión
    const double verticalFOV = 120.0; // Ángulo vertical de visión en grados
    const double horizontalFOV = 200.0; // Ángulo horizontal de visión en grados

    BTDecisionPlayerDetected()  {}

    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        ectx.ai.bh = "detecting player";
        //Diferencia de niveles
        // if(ectx.ent.hasTag<SpiderTag>()){
        //     if(getplayerpos(ectx).y() > ectx.phy.position.y()){
        //         std::cout << getplayerpos(ectx).y() << "jiji" <<
        //         ectx.phy.position.y();
        //         ectx.ai.playerdetected = false;
        //         return BTNodeStatus_t::fail;
        //     }
        // }
        //########## PERCEPCION SENSORIAL - VISTA ################
        // Calcula la dirección de visión del enemigo utilizando su orientación
        vec3d enemyDirection = vec3d(std::sin(ectx.phy.orientation),0.0, std::cos(ectx.phy.orientation));
        // Vector que apunta desde el enemigo al jugador
        vec3d toPlayer = getplayerpos(ectx) - ectx.phy.position;
        //normalizamos vectores
        enemyDirection.normalize();
        toPlayer.normalize();
        //producto escalar
        double dotProduct = enemyDirection.dotProduct(toPlayer);
        // Calcula el ángulo entre la dirección de visión del enemigo y la dirección hacia el jugador
        double angle = std::acos(dotProduct);
        // Convierte el ángulo de radianes a grados
        double angleDegrees = angle * 180.0 / PI;
        // Comprueba si el ángulo está dentro del rango de visión del enemigo
        if (angleDegrees < verticalFOV / 2.0 && std::abs(angleDegrees) < horizontalFOV / 2.0) {
            //Raycast
            bool rayintercepted{false};
            // vec3d posenemy{};
            // if(getplayerpos(ectx).y() != 0.0){
            //     posenemy = {ectx.phy.position.x(),
            //     getplayerpos(ectx).y(),ectx.phy.position.z()};
            // }else{
            //     posenemy = ectx.phy.position;
            // }
            // Realiza un raycast en la dirección en la que el enemigo está mirando para detectar obstáculos
            RayCast ray = { ectx.phy.position, enemyDirection };
            //Compruebo si la caja de colisión de un obstaculo ha colisionado con el rayo
            for(Entity &ent : ectx.em.getEntities()){
                if(ent.hasComponent<ColliderComponent>()){
                    ColliderComponent& col = ectx.em.getComponent<ColliderComponent>(ent);
                    if(col.behaviorType & BehaviorType::STATIC){
                        auto& col = ectx.em.getComponent<ColliderComponent>(ent);
                        if(col.boundingBox.intersectsRay(ray.origin,ray.direction)){
                            // si el rayo intercepta un obstaculo
                            // calculo distancia del obstaculo al enemigo
                            auto& phyobs = ectx.em.getComponent<PhysicsComponent>(ent);
                            vec3d distanceobjene = phyobs.position - ectx.phy.position;
                            //calculo distancia del player al enemigo
                            vec3d distancetoplayer = getplayerpos(ectx) - ectx.phy.position;
                            //Si la distancia del enemigo al obstaculo es menor que al player 
                            //significa que esta entre ambos
                            // if(ectx.ent.hasTag<SnowmanTag>()){
                            //     std::cout << "disobj: " << distanceobjene.length() << "displa:" <<
                            //     distancetoplayer.length() << "\n";
                            //     if(distanceobjene.length() < distancetoplayer.length()){
                            //         std::cout << "objeto en medio \n";
                            //     }
                            // }
                            if(distanceobjene.length() < distancetoplayer.length()){
                                rayintercepted = true;
                                break;
                            }
                        }
                    }
                }
            }
            if(rayintercepted){ // si el obstaculo quita la vision al enemigo
                ectx.ai.playerdetected = false;
            }else{ // si no hay obstaculos entre player y enemigo lo detecta
                ectx.ai.playerdetected = true;
                return BTNodeStatus_t::success;
            }
        }
        
        //#### PERCEPCION SENSORIAL - OIDO ################################
        //Calculo la distancia del player al enemigo
        auto const distance = (ectx.phy.position - getplayerpos(ectx)).lengthSQ();
        //Compruebo si esta dentro del radio de detección por oido
        //podriamos hacer que en un radio x el enemigo entre en un estado
        // de alerta
        // si te acercas mucho superando la mitad de ese radio, ya te escucha detectandote
        // if(distance < ectx.ai.detect_radius){
        //     //estaado de alerta
        // }

        if( distance < (ectx.ai.detect_radius * ectx.ai.detect_radius) / 2){
            //te escucha
            if(!ectx.ent.hasTag<BossFinalTag>()){
                ectx.ai.path_initialized = false;
            }
            ectx.ai.playerdetected = true;
            return BTNodeStatus_t::success;
        }else{
            ectx.ai.playerdetected = false;
        }
        
        return BTNodeStatus_t::fail;
    }

private:
    vec3d getplayerpos(EntityContext_t& ectx){
        auto& li = ectx.em.getSingleton<LevelInfo>();
        auto* playerEn = ectx.em.getEntityByID(li.playerID);
        if (not playerEn) return vec3d{}; // No hay player
        // Si hay player
        auto& plphy = ectx.em.getComponent<PhysicsComponent>(*playerEn);
        return plphy.position;
    };
};
