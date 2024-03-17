#pragma once
#include "node.hpp"
#include <utils/vec3D.hpp>


struct BTDecisionPlayerDetected : BTNode_t {
    // Constantes para ángulos de visión
    const double verticalFOV = 60.0; // Ángulo vertical de visión en grados
    const double horizontalFOV = 170.0; // Ángulo horizontal de visión en grados
    const double MinRayDistance = 10.0; // Mínima distancia de detección por vista

    BTDecisionPlayerDetected() {}

    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        ectx.ai.bh = "detecting player";

        //Pongo player detected a false

        //#### PERCEPCION SENSORIAL - OIDO ################################
        //Calculo la distancia del player al enemigo
       auto const distance = (ectx.phy.position - getplayerphy(ectx).position).lengthSQ();
       auto const radius_listen_1 = (ectx.ai.detect_radius * ectx.ai.detect_radius);
       auto const radius_listen_2 = radius_listen_1 / 4.0;
        
        if(!ectx.ai.playerdetected){
            bool in_alert_radius{false};
            ectx.ai.listen_steps = true;
            //Si estas dentro de su rango de alerta
            if(distance < radius_listen_1){
                //Aqui se escuchan los pasos un poco
                in_alert_radius = true;
                //Compruebo si se escuchan los pasos del player
                if(getplayerphy(ectx).velocity.x() == 0.0 && getplayerphy(ectx).velocity.z() == 0.0){
                    //No se escuchan
                    ectx.ai.listen_steps = false;
                }
                //Nivel de escucha de los pasos
                ectx.ai.increase_angle = 0.5f;
            }

            if(distance < radius_listen_2){
                //Aqui se escuchan los pasos más fuerte
                in_alert_radius = true;
                //Compruebo si se escuchan los pasos del player
                if(getplayerphy(ectx).velocity.x() == 0.0 && getplayerphy(ectx).velocity.z() == 0.0){
                    ////No se escuchan
                    ectx.ai.listen_steps = false;
                }
                //Nivel de escucha de los pasos
                ectx.ai.increase_angle = 1.0f;
            }
            // Si no entro en ninguno de los radios de escucha no entra en estado de alerta
            if(!in_alert_radius){
                ectx.ai.alert_state = false;
                //Además no me detecta
                ectx.ai.playerdetected = false;
            }else{
                ectx.ai.alert_state = true;
            }
            //Si se completa la barra de escucha , me detectan
            //dibujar icono alerta encima de enemigo
            if(ectx.ai.endangle <= -360.0f){
                ectx.ai.playerdetected = true;
                ectx.ai.alert_state = false;
                ectx.ai.endangle = 0.0f;
                return BTNodeStatus_t::success;
            }
        }else{
            ectx.ai.alert_state = false;
            ectx.ai.endangle = 0.0f;
        }

        //########## PERCEPCION SENSORIAL - VISTA ################

        // Si el player se encuentra a una altura mayor o menor que el enemigo
        // que sea superior a 1 metro no puede ser visto por el enemigo
        if(getplayerphy(ectx).position.y() - ectx.phy.position.y() > 1.0){
            if(getplayerphy(ectx).position.y() > ectx.phy.position.y()){
                    ectx.ai.playerdetected = false;
                    return BTNodeStatus_t::fail;
            }
        }


        //Calcula la dirección de visión del enemigo utilizando su orientación
        vec3d enemyDirection = vec3d(std::sin(ectx.phy.orientation), 0.0, std::cos(ectx.phy.orientation));
        //Vector que apunta desde el enemigo al jugador
        vec3d toPlayer = getplayerphy(ectx).position - ectx.phy.position;
        //normalizamos vectores
        enemyDirection.normalize();
        toPlayer.normalize();
        //producto escalar
        double dotProduct = enemyDirection.dotProduct(toPlayer);
        //Calcula el ángulo entre la dirección de visión del enemigo y la dirección hacia el jugador
        double angle = std::acos(dotProduct);
        // Convierte el ángulo de radianes a grados
        double angleDegrees = angle * 180 / K_PI;

        //Comprueba si el ángulo está dentro del rango de visión del enemigo
        if (angleDegrees < verticalFOV / 2.0 && std::abs(angleDegrees) < horizontalFOV / 2.0) {
           // Raycast
            vec3d intersection_wall{}, intersection_player{};
            // Realiza un raycast en la dirección en la que el enemigo está mirando para detectar obstáculos
            vec3d dir = getplayerphy(ectx).position - ectx.phy.position;
            RayCast ray = { ectx.phy.position,  dir };

            // dibujado de raycast
            auto& bb = ectx.em.getSingleton<BlackBoard_t>();
            bb.position_origin = ray.origin;
            bb.direction = ray.direction;
            bb.launched = true;

            auto& bbox = getplayercollider(ectx).boundingBox;

            // Calculo punto de intersección con player
            if (bbox.intersectsRay(ray.origin, ray.direction)) {
                bbox.intersectsRay(ray.origin, ray.direction,intersection_player);
            }

            // Compruebo si la caja de colisión de un obstaculo ha colisionado con el rayo
            for (Entity& ent : ectx.em.getEntities()) {
                // Comprobamos entidades Pared y que tengan colisión component
                if (ent.hasComponent<ColliderComponent>()) {
                    if (ent.hasTag<WallTag>()) {
                        auto& col = ectx.em.getComponent<ColliderComponent>(ent);
                        if (col.boundingBox.intersectsRay(ray.origin, ray.direction)) {
                            col.boundingBox.intersectsRay(ray.origin, ray.direction,intersection_wall);
                            // Evitamos intersecciones de rayos que se han ido muy lejos
                            // Damos más importancia a los obstáculos cercanos
                            if(intersection_wall.distance(ectx.phy.position) < MinRayDistance){
                                // El primero que es encontrado, rompe el bucle
                                break;
                            }
                        }
                    }
                }
            }
            // comprobamos las distancias a los puntos de intersección
            if (intersection_wall.distance(ectx.phy.position) < intersection_player.distance(ectx.phy.position)) {
                // Hay un obstáculo delante
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
        if(ectx.ai.playerdetected){
            return BTNodeStatus_t::success;
        }else{
             return BTNodeStatus_t::fail;
        }
    }

private:
    // Obtengo la posición del player
    PhysicsComponent& getplayerphy(EntityContext_t& ectx) {
        auto& li = ectx.em.getSingleton<LevelInfo>();
        auto* playerEn = ectx.em.getEntityByID(li.playerID);
        //if (not playerEn) return vec3d{}; // No hay player
        // Si hay player
        auto& plphy = ectx.em.getComponent<PhysicsComponent>(*playerEn);
        return plphy;
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
