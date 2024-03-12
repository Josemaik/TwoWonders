#pragma once
#include "node.hpp"
#include <utils/vec3D.hpp>


struct BTDecisionPlayerDetected : BTNode_t {
    // Constantes para ángulos de visión
    const double verticalFOV = 80.0; // Ángulo vertical de visión en grados
    const double horizontalFOV = 170.0; // Ángulo horizontal de visión en grados
    const double MinRayDistance = 10.0; // Ángulo horizontal de visión en grados

    BTDecisionPlayerDetected() {}

    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        ectx.ai.bh = "detecting player";
        //#### PERCEPCION SENSORIAL - OIDO ################################
        //Calculo la distancia del player al enemigo
       auto const distance = (ectx.phy.position - getplayerpos(ectx)).length();
        auto const radius = (ectx.ai.detect_radius * ectx.ai.detect_radius);
        //si te metes en el radio de escucha, pasa a estado de sospecha
        if(distance < radius && !ectx.ai.playerdetected){
            //estado de alerta
            //dibujar icono alerta encima de enemigo
            ectx.ai.alert_state = true;
        }else{
            ectx.ai.alert_state = false;
        }
        
        // Si superas 4 partes de ese radio te detecta directamente
        if( distance < (ectx.ai.detect_radius * ectx.ai.detect_radius) / 4){
            ectx.ai.alert_state = false;
            //te escucha
            if(!ectx.ent.hasTag<BossFinalTag>()){
                ectx.ai.path_initialized = false;
            }
            ectx.ai.playerdetected = true;
            return BTNodeStatus_t::success;
        }else{
            //No te escucha
            ectx.ai.playerdetected = false;
        }

        //########## PERCEPCION SENSORIAL - VISTA ################

        //Si el player se encuentra a una altura mayor o menor que el enemigo
        // que sea superior a 1 metro no puede ser visto por el enemigo
        if(getplayerpos(ectx).y() - ectx.phy.position.y() > 1.0){
            if(getplayerpos(ectx).y() > ectx.phy.position.y()){ 
                    ectx.ai.playerdetected = false;
                    return BTNodeStatus_t::fail;
            }
        }


        //Calcula la dirección de visión del enemigo utilizando su orientación
        vec3d enemyDirection = vec3d(std::sin(ectx.phy.orientation), 0.0, std::cos(ectx.phy.orientation));
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
        double angleDegrees = angle * 180 / PI;
        //Comprueba si el ángulo está dentro del rango de visión del enemigo
        if (angleDegrees < verticalFOV / 2.0 && std::abs(angleDegrees) < horizontalFOV / 2.0) {
            //Raycast
            vec3d intersection_wall{}, intersection_player{};
            // Realiza un raycast en la dirección en la que el enemigo está mirando para detectar obstáculos
            vec3d dir = getplayerpos(ectx) - ectx.phy.position;
            RayCast ray = { ectx.phy.position,  dir };

            // dibujado de raycast
            auto& bb = ectx.em.getSingleton<BlackBoard_t>();
            bb.position_origin = ray.origin;
            bb.direction = ray.direction;
            bb.launched = true;

            auto& bbox = getplayercollider(ectx).boundingBox;

            //Calculo punto de intersección con player
            if (bbox.intersectsRay(ray.origin, ray.direction)) {
                bbox.intersectsRay(ray.origin, ray.direction,intersection_player);
            }

            //Compruebo si la caja de colisión de un obstaculo ha colisionado con el rayo
            for (Entity& ent : ectx.em.getEntities()) {
                //Comprobamos entidades Pared y que tengan colisión component
                if (ent.hasComponent<ColliderComponent>()) {
                    if (ent.hasTag<WallTag>()) {
                        auto& col = ectx.em.getComponent<ColliderComponent>(ent);
                        if (col.boundingBox.intersectsRay(ray.origin, ray.direction)) {
                            col.boundingBox.intersectsRay(ray.origin, ray.direction,intersection_wall);
                            //Evitamos intersecciones de rayos que se han ido muy lejos
                            // Damos más importancia a los obstáculos cercanos
                            if(intersection_wall.distance(ectx.phy.position) < MinRayDistance){
                                //El primero que es encontrado, rompe el bucle
                                break;
                            }
                        }
                    }
                }
            }
            //comprobamos las distancias a los puntos de intersección
            if (intersection_wall.distance(ectx.phy.position) < intersection_player.distance(ectx.phy.position)) {
                //Hay un obstáculo delante
                ectx.ai.playerdetected = false;
                return BTNodeStatus_t::fail;
            }
            else {
                // Si te sales del cono de visión ( evitar que te vea hasta el infinito )
                if(intersection_player.distance(ectx.phy.position) > ectx.ai.detect_radius){
                    ectx.ai.playerdetected = false;
                    return BTNodeStatus_t::fail;
                }else{
                    // No hay obstáculo, eres detectado
                    ectx.ai.alert_state = false;
                    ectx.ai.playerdetected = true;
                    return BTNodeStatus_t::success;
                }
            }
        }

        //No ha sido detectado por oido ni vista
        ectx.ai.playerdetected = false;
        return BTNodeStatus_t::fail;
    }

private:
    //Obtengo la posición del player
    vec3d getplayerpos(EntityContext_t& ectx) {
        auto& li = ectx.em.getSingleton<LevelInfo>();
        auto* playerEn = ectx.em.getEntityByID(li.playerID);
        if (not playerEn) return vec3d{}; // No hay player
        // Si hay player
        auto& plphy = ectx.em.getComponent<PhysicsComponent>(*playerEn);
        return plphy.position;
    };
    //Obtengo el colision component del player
    ColliderComponent& getplayercollider(EntityContext_t& ectx) {
        auto& li = ectx.em.getSingleton<LevelInfo>();
        auto* playerEn = ectx.em.getEntityByID(li.playerID);
        //if (not playerEn) return ColliderComponent{}; // No hay player
        // Si hay player
        auto& col = ectx.em.getComponent<ColliderComponent>(*playerEn);
        return col;
    };
};
