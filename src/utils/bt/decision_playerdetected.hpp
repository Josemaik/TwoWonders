#pragma once
#include "node.hpp"
#include <utils/vec3D.hpp>


struct BTDecisionPlayerDetected : BTNode_t {
    // Constantes para ángulos de visión
    const double verticalFOV = 80.0; // Ángulo vertical de visión en grados
    const double horizontalFOV = 170.0; // Ángulo horizontal de visión en grados

    BTDecisionPlayerDetected() {}

    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        ectx.ai.bh = "detecting player";
        //Si el player se encuentra en un nivel superior no es detectado por
        //la IA
        //Diferencia de niveles
        // if(ectx.ent.hasTag<SpiderTag>()){
        //     if(getplayerpos(ectx).y() - ectx.phy.position.y() > 1.0){
        //         if(getplayerpos(ectx).y() > ectx.phy.position.y()){ 
        //             // std::cout << getplayerpos(ectx).y() << "jiji" <<
        //             // ectx.phy.position.y();
        //             ectx.ai.playerdetected = false;
        //             return BTNodeStatus_t::fail;
        // //         }
        // //     }
        // // }
        //#### PERCEPCION SENSORIAL - OIDO ################################
        //Calculo la distancia del player al enemigo
    //    auto const distance = (ectx.phy.position - getplayerpos(ectx)).lengthSQ();
    //     //     //Compruebo si esta dentro del radio de detección por oido
    //     //     //podriamos hacer que en un radio x el enemigo entre en un estado
    //     //     // de alerta

    //     //si te metes en el radio de escucha, pasa a estado de sospecha
    //     if(distance < (ectx.ai.detect_radius * ectx.ai.detect_radius) && !ectx.ai.playerdetected){
    //         //estado de alerta
    //         //dibujar icono alerta encima de enemigo
    //         ectx.ai.alert_state = true;
    //     }
        
    //     // Si superas la mitad de ese radio te detecta directamente
    //     if( distance < (ectx.ai.detect_radius * ectx.ai.detect_radius) / 4){
    //         ectx.ai.alert_state = false;
    //         //te escucha
    //         if(!ectx.ent.hasTag<BossFinalTag>()){
    //             ectx.ai.path_initialized = false;
    //         }
    //         ectx.ai.playerdetected = true;
    //         return BTNodeStatus_t::success;
    //     }else{
    //         ectx.ai.playerdetected = false;
    //     }

        //########## PERCEPCION SENSORIAL - VISTA ################
        //Calcula la dirección de visión del enemigo utilizando su orientación
        //double orientation_grados = ectx.phy.orientation * 180.0 / M_PI;
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
        // // std::cout << "Grados: " << angleDegrees << "\n";
        // // std::cout << "Vertical: " << verticalFOV/2.0 << "\n";
        // // Comprueba si el ángulo está dentro del rango de visión del enemigo
        if (angleDegrees < verticalFOV / 2.0 && std::abs(angleDegrees) < horizontalFOV / 2.0) {
            //Raycast
            // bool rayintercepted{false};
            vec3d intersection_wall{}, intersection_player{};
            // Realiza un raycast en la dirección en la que el enemigo está mirando para detectar obstáculos
            //std::sin(ectx.phy.orientation) *180.0 / PI,1.0, std::cos(ectx.phy.orientation) *180.0 / PI
            // std::cout << getplayerpos(ectx).normalized() << "a\n";
            // std::cout << getplayerpos(ectx).normalized().length() << "a\n";
            // vec3d dir = vec3d(std::sin(ectx.phy.orientation) * 180.0 / PI, getplayerpos(ectx).y(), std::cos(ectx.phy.orientation) * 180.0 / PI).normalize();
            vec3d dir = getplayerpos(ectx) - ectx.phy.position;
            RayCast ray = { ectx.phy.position,  dir };

        //     // dibujado de raycast
            auto& bb = ectx.em.getSingleton<BlackBoard_t>();
            bb.position_origin = ray.origin;
            bb.direction = ray.direction;
            bb.launched = true;

            auto& bbox = getplayercollider(ectx).boundingBox;

            //Calculo punto de intersección con player
            if (bbox.intersectsRay(ray.origin, ray.direction)) {
                bbox.intersectsRay(ray.origin, ray.direction,intersection_player);
            }

        //     //Compruebo si la caja de colisión de un obstaculo ha colisionado con el rayo
            for (Entity& ent : ectx.em.getEntities()) {
                if (ent.hasComponent<ColliderComponent>()) {
                    // ColliderComponent& col = ectx.em.getComponent<ColliderComponent>(ent);
                    //RenderComponent& ren = ectx.em.getComponent<RenderComponent>(ent);
                    // col.behaviorType & BehaviorType::STATIC 
                    if (ent.hasTag<WallTag>()) {
                        auto& col = ectx.em.getComponent<ColliderComponent>(ent);
                        if (col.boundingBox.intersectsRay(ray.origin, ray.direction)) {
                            col.boundingBox.intersectsRay(ray.origin, ray.direction,intersection_wall);
                            if(intersection_wall.distance(ectx.phy.position) < 10.0){
                                break;
                            }
                        }

        // //                 // if(intersection_wall.x() != 0 && intersection_wall.z() != 0){
        // //                 //     break;
        // //                 // }
        // //                     // si el rayo intercepta un obstaculo
        // //                     // calculo distancia del obstaculo al enemigo

        // //                 //     auto& phyobs = ectx.em.getComponent<PhysicsComponent>(ent);
        // //                 //     vec3d distanceobjene = phyobs.position - ectx.phy.position;
        // //                 //     //calculo distancia del player al enemigo
        // //                 //     vec3d distancetoplayer = getplayerpos(ectx) - ectx.phy.position;
        // //                 //     //Si la distancia del enemigo al obstaculo es menor que al player
        // //                 //     //significa que esta entre ambos
        // //                 //     // if(ectx.ent.hasTag<SnowmanTag>()){
        // //                 //     //     std::cout << "disobj: " << distanceobjene.length() << "displa:" <<
        // //                 //     //     distancetoplayer.length() << "\n";
        // //                 //     //     if(distanceobjene.length() < distancetoplayer.length()){
        // //                 //     //         std::cout << "objeto en medio \n";
        // //                 //     //     }
        // //                 //     // }
        // //                 //    // std::cout << ent.getID() << "\n";
        // //                 //     if(distanceobjene.length() < distancetoplayer.length()){
        // //                 //         rayintercepted = true;
        // //                 //         break;
        // //                 //     }
                    }
                }
            }
            //distancias a los puntos de intersección
            // vec3d distancetoplayer = intersection_player - ectx.phy.position;
            // vec3d distancetoobject = intersection_wall - ectx.phy.position;
            //comprobamos las distancias a los puntos de intersección
            std::cout << "disW: " << intersection_wall.distance(ectx.phy.position) << 
            "disP: " << intersection_player.distance(ectx.phy.position) << "\n";
            if (intersection_wall.distance(ectx.phy.position) < intersection_player.distance(ectx.phy.position)) {
                //Hay un obstáculo enmedio
                ectx.ai.playerdetected = false;
                return BTNodeStatus_t::fail;
            }
            else {
                // Si te sales del cono de visión
                if(intersection_player.distance(ectx.phy.position) > ectx.ai.detect_radius){
                    ectx.ai.playerdetected = false;
                    return BTNodeStatus_t::fail;
                }else{
                    ectx.ai.alert_state = false;
                    ectx.ai.playerdetected = true;
                    return BTNodeStatus_t::success;
                }
            }
        //     // if(rayintercepted){ // si el obstaculo quita la vision al enemigo
        //     //     ectx.ai.playerdetected = false;
        //     // }else{ // si no hay obstaculos entre player y enemigo lo detecta
        //     //     ectx.ai.playerdetected = true;
        //     //     return BTNodeStatus_t::success;
        // //     // }
        }

        

        // ectx.ai.alert_state = false;
        ectx.ai.playerdetected = false;
        return BTNodeStatus_t::fail;
    }

private:
    vec3d getplayerpos(EntityContext_t& ectx) {
        auto& li = ectx.em.getSingleton<LevelInfo>();
        auto* playerEn = ectx.em.getEntityByID(li.playerID);
        if (not playerEn) return vec3d{}; // No hay player
        // Si hay player
        auto& plphy = ectx.em.getComponent<PhysicsComponent>(*playerEn);
        return plphy.position;
    };

    ColliderComponent& getplayercollider(EntityContext_t& ectx) {
        auto& li = ectx.em.getSingleton<LevelInfo>();
        auto* playerEn = ectx.em.getEntityByID(li.playerID);
        //if (not playerEn) return ColliderComponent{}; // No hay player
        // Si hay player
        auto& col = ectx.em.getComponent<ColliderComponent>(*playerEn);
        return col;
    };
};
