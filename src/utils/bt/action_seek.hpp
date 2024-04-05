#pragma once
#include "node.hpp"
#include <cmath>
#include <numbers>
#include <algorithm>
#include <utils/sb/steeringbehaviour.hpp>

static double threshold { 15.0 };
// static double radius_centro_masas { 20.0 };
// Peso para la influencia de la dirección hacia el jugador
//double playerWeight = 0.8; 
// Peso para la influencia de la dirección hacia el centro de masas
// static double centerOfMassWeight = 0.01;

struct BTAction_Seek : BTNode_t {
    // BTActionPatrol() = default;
   

    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        if (!ectx.ai->tactive) {

            return BTNodeStatus_t::fail;
        }
        ectx.ai->bh = "seeking";
        ectx.ai->ia_front_of_you = false;
        //Seek
        Steer_t steering = STBH::Seek(ectx.phy, { ectx.ai->tx,0.0,ectx.ai->tz });
        steering.v_x = steering.v_x * 0.7;
        steering.v_z = steering.v_z * 0.7;
        //Calcule Flocking
        //Comprobamos si tiene target en un radio cercano
        // double suma_cm_x{};
        // double suma_cm_z{};
        // double numenemigos_cm{};
        // vec3d cm{};
        //int num_forces_aplicated{0};
        //bool force_aplicated{false};
        int num_stopped{0};
        double min_distance{100.0};
        Steer_t steering_closest{};
        auto& bb = ectx.em.getSingleton<BlackBoard_t>();
        //Check closest  targets
        for(auto& pt : bb.potencial_targets){
            //Discart the current ia that is processing the node
            if(ectx.ent.getID() != pt.first){
                //Check if the targte is close
                vec3d direction = pt.second - ectx.phy.position;
                double distance = direction.length();
                //Si esta dentro del límite
                if(distance < threshold){
                    //bool closerEnemyExists = false;
                    
                    // // Verificar si hay otro enemigo más cerca del objetivo dentro del umbral
                    // for (auto& other_pt : bb.potencial_targets) {
                    //     if (ectx.ent.getID() != other_pt.first) {
                    //         vec3d otherDirection = other_pt.second - ectx.phy.position;
                    //         double otherDistance = otherDirection.length();

                    //         // Si hay otro enemigo más cerca del objetivo dentro del umbral, marcar la bandera y salir del bucle
                    //         if (otherDistance <= distance) {
                    //             closerEnemyExists = true;
                    //             break;
                    //         }
                    //     }
                    // }
                    // // Si no hay otro enemigo más cerca del objetivo dentro del umbral, aplicar la lógica de evasión
                    if ((*ectx.em.getEntityByID(pt.first)).hasComponent<PhysicsComponent>() &&
                    (*ectx.em.getEntityByID(pt.first)).hasComponent<ColliderComponent>()) {
                        auto& phytoevade = ectx.em.getComponent<PhysicsComponent>(*ectx.em.getEntityByID(pt.first));
                        
                        if(distance < min_distance){
                            min_distance = distance;
                            steering_closest = STBH::Evade(ectx.phy, phytoevade);
                        }
                        
                        //Mientras se muevan se mantiene una separación
                        if(phytoevade.velocity.x() != 0.0 && phytoevade.velocity.z() != 0.0){
                            Steer_t steeringEvade = STBH::Evade(ectx.phy, phytoevade);
                            steering.v_x += steeringEvade.v_x;
                            steering.v_z += steeringEvade.v_z;
                            steering.orientation += steeringEvade.orientation;
                        }else{
                            //están parados
                            num_stopped++;
                        }
                        // auto& colltoevade = ectx.em.getComponent<ColliderComponent>(*ectx.em.getEntityByID(pt.first));
                        // if(phytoevade.velocity.x() == 0.0 && phytoevade.velocity.z() == 0.0){
                        //     //Acelereamos la velocidad hacia el player
                        // steering.v_x *= 2.3;
                        // steering.v_z *= 2.3;
                        //     //Esquivamos enemigos
                        //     vec3d enemyDirection = vec3d(std::sin(ectx.phy.orientation), 0.0, std::cos(ectx.phy.orientation));
                        //     RayCast ray = { ectx.phy.position, enemyDirection};
                        //     //auto& bboxevader = ectx.em.getComponent<ColliderComponent>(ectx.ent).boundingBox;
                        //     auto& bboxtoevade = colltoevade.boundingBox;
                        //     if (bboxtoevade.intersectsRay(ray.origin, ray.direction) && !force_aplicated) {
                        //        ectx.ai->ia_front_of_you = true;
                        //        Steer_t stearing_avoid = STBH::Avoid(phytoevade,ectx.phy);
                        //        steering.v_x += stearing_avoid.v_x * 2.0;
                        //        steering.v_z += stearing_avoid.v_z * 2.0;
                        //        force_aplicated = true;
                        //         // double evadeSpeed = 0.5; // Ajusta la velocidad de esquive según sea necesario
                        //         // vec3d evadeVector = vec3d(-enemyDirection.z(), 0.0, enemyDirection.x()); // Girar 90 grados
                        //         // steering.v_x += evadeVector.x() * evadeSpeed;
                        //         // steering.v_z += evadeVector.z() * evadeSpeed;
                        //     }
                    }
                        // else{
                        //     //Mientras no se mueven se esquivan
                        //     vec3d enemyDirection = vec3d(std::sin(ectx.phy.orientation), 0.0, std::cos(ectx.phy.orientation));
                        //     RayCast ray = { ectx.phy.position, enemyDirection};
                        //     //auto& bboxevader = ectx.em.getComponent<ColliderComponent>(ectx.ent).boundingBox;
                        //     auto& bboxtoevade = colltoevade.boundingBox;
                        //     if (bboxtoevade.intersectsRay(ray.origin, ray.direction)) {
                        //         ectx.ai->ia_front_of_you = true;
                        //         double evadeSpeed = 0.5; // Ajusta la velocidad de esquive según sea necesario
                        //         vec3d evadeVector = vec3d(-enemyDirection.z(), 0.0, enemyDirection.x()); // Girar 90 grados
                        //         steering.v_x += evadeVector.x() * evadeSpeed;
                        //         steering.v_z += evadeVector.z() * evadeSpeed;
                        //     }
                        // }
                        //steering.orientation += steeringEvade.orientation;
                    // }
                }
                //     //Separate from target
                //     Steer_t steeringEvade = STBH::Evade(ectx.phy,pt.second);
                //     steering.v_x += steeringEvade.v_x;
                //     steering.v_z += steeringEvade.v_z;
                //     //Calculate the strength of repulsion
                //     // double strength = std::min(0.5 * distance * distance,ectx.phy.max_speed);
                //     // direction.normalize();
                //     // steering.v_x += strength * direction.x();
                //     // steering.v_z += strength * direction.z();
                // }
            }

            // if(num_stopped == static_cast<int>(bb.potencial_targets.size() - 1)){
            //     steering.v_x += steering_closest.v_x;
            //     steering.v_z += steering_closest.v_z;
            // }
            //     auto distbetweentpoints = ectx.phy.position.distance(pos.second);
            //     if(distbetweentpoints < radius_evade){
                    // // Evade
                    // //calculate directtion postoevade since player
                    // vec3d evadetoplayer = vec3d{ectx.ai->tx,0.0,ectx.ai->tz} - pos.second;
                    // vec3d evadeVector = pos.second - ectx.phy.position;
                    // evadetoplayer.normalize();
                    // evadeVector.normalize();
                    // // Calcula el ángulo entre las dos direcciones en radianes
                    // double angle = acos(evadetoplayer.dotProduct(evadeVector));
                    // // Convierte el ángulo de radianes a grados
                    // double angleDegrees = angle * (180.0 / PI);
                    // std::cout <<"grados:"<< angleDegrees << "\n";
                    // if(angleDegrees > 20.0 && angleDegrees < 70.0){
                    //     Steer_t steeringEvade = STBH::Evade(ectx.phy,pos.second);
                    //     steering.v_x += steeringEvade.v_x;
                    //     steering.v_z += steeringEvade.v_z;
                    // }
                    //contador++;
            //     }
        }
            //Calculate mass center
            // if(ectx.phy.position.calculatePointDistance(pos.second) < radius_centro_masas){
            //     suma_cm_x += pos.second.x();
            //     suma_cm_z += pos.second.z();
            //     numenemigos_cm++;
            // }
        // }
        //std::cout << "Contaor: " << contador << "\n";
        // add center of mass direction to toplayer direction
        // cm = vec3d{suma_cm_x/numenemigos_cm,0.0,suma_cm_z/numenemigos_cm};
        // vec3d dirToCenterOfMass = cm - ectx.phy.position;
        // dirToCenterOfMass.normalize();
        // // // Dirección final, combinando las direcciones hacia el jugador y hacia el centro de masas
        // // // Dirección final, combinando las direcciones hacia el jugador y hacia el centro de masas
        // steering.v_x += std::clamp(dirToCenterOfMass.x() * centerOfMassWeight, -ectx.phy.max_speed, ectx.phy.max_speed);
        // steering.v_z += std::clamp(dirToCenterOfMass.z() * centerOfMassWeight, -ectx.phy.max_speed, ectx.phy.max_speed);
        //Almacenar posiciones de eenemigos en un radio mas grande
        //Para radio pequeño-> evade de estos y restarselo al vector velocidad ( evitar)
        //Para radio grande -> calcular centro de masas y sumar a la velocidad (cohesionar)
        

        ectx.phy.velocity = vec3d{ steering.v_x, 0.0, steering.v_z };
        ectx.phy.orientation = steering.orientation;
        return BTNodeStatus_t::success;
    }
};
