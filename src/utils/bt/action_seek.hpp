#pragma once
#include "node.hpp"
#include <cmath>
#include <numbers>
#include <algorithm>
#include <cstring>
#include <utils/sb/steeringbehaviour.hpp>

static double threshold{ 20.0 };
static double radius_centro_masas{ 30.0 };
// Peso para la influencia de la dirección hacia el jugador
//double playerWeight = 0.8; 
// Peso para la influencia de la dirección hacia el centro de masas
static double centerOfMassWeight{ 0.05 };

struct BTAction_Seek : BTNode_t {
    // BTActionPatrol() = default;


    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        if (!ectx.ai->tactive) {

            return BTNodeStatus_t::fail;
        }
        ectx.ai->bh = "seeking";
        ectx.ai->seeking = true;
        ectx.ai->ispushed = false;
        // ectx.ai->ia_front_of_you = false;
        //Seek
        Steer_t steering = STBH::Seek(ectx.phy, { ectx.ai->tx,0.0,ectx.ai->tz });
        
        // Calculo de Flocking
        steering = calculateFlocking(ectx, steering);

        // Esquivar obstáculos
        steering = avoidObstacles(ectx, steering);
        
        // set velocity and orientation
        ectx.phy.velocity = vec3d{ steering.v_x, 0.0, steering.v_z };
        ectx.phy.orientation = steering.orientation;
        return BTNodeStatus_t::success;
    }

    Steer_t calculateFlocking(EntityContext_t& ectx, Steer_t steering) {
        double suma_cm_x{};
        double suma_cm_z{};
        double numenemigos_cm{};
        vec3d cm{};

        auto& bb = ectx.em.getSingleton<BlackBoard_t>();
        for (auto& pt : bb.potencial_targets) {
            if (ectx.ent.getID() != pt.first) {
                vec3d direction = pt.second - ectx.phy.position;
                double distance = direction.length();
                if (distance < threshold) {
                    double maxsepforce{ 0.0 };
                    if ((*ectx.em.getEntityByID(pt.first)).hasComponent<PhysicsComponent>() &&
                        (*ectx.em.getEntityByID(pt.first)).hasComponent<AIComponent>()) {
                        auto& phytoevade = ectx.em.getComponent<PhysicsComponent>(*ectx.em.getEntityByID(pt.first));
                        auto& aitoevade = ectx.em.getComponent<AIComponent>(*ectx.em.getEntityByID(pt.first));
                        if (phytoevade.velocity.x() != 0.0 && phytoevade.velocity.z() != 0.0 && !aitoevade.ispushed) {
                            maxsepforce = 1.0;
                            Steer_t steeringEvade = STBH::Evade(ectx.phy, phytoevade, maxsepforce);
                            steering.v_x += steeringEvade.v_x;
                            steering.v_z += steeringEvade.v_z;
                            steering.orientation += steeringEvade.orientation;
                        } else {
                            Steer_t steeringEvade = STBH::Evade(phytoevade, ectx.phy, 1.0);
                            phytoevade.velocity = vec3d{ steeringEvade.v_x * 0.8, 0.0, steeringEvade.v_z * 0.8 };
                            phytoevade.orientation += steeringEvade.orientation;
                            aitoevade.ispushed = true;
                        }
                    }
                }
                if (distance < radius_centro_masas) {
                    suma_cm_x += pt.second.x();
                    suma_cm_z += pt.second.z();
                    numenemigos_cm++;
                }
            }
        }

        if (numenemigos_cm >= 2) {
            cm = vec3d{ suma_cm_x / numenemigos_cm, 0.0, suma_cm_z / numenemigos_cm };
            vec3d dirToCenterOfMass = cm - ectx.phy.position;
            dirToCenterOfMass.normalize();
            steering.v_x += std::clamp(dirToCenterOfMass.x() * centerOfMassWeight, -ectx.phy.max_speed, ectx.phy.max_speed);
            steering.v_z += std::clamp(dirToCenterOfMass.z() * centerOfMassWeight, -ectx.phy.max_speed, ectx.phy.max_speed);
        }
        return steering;
    }

    Steer_t avoidObstacles(EntityContext_t& ectx, Steer_t steering) {
        using noCMP = MP::TypeList<PhysicsComponent, ColliderComponent>;
        using obstacleTag = MP::TypeList<ObstacleTag>;

        auto& pos = ectx.phy.position;
        double mindistance{ 100.0 };
        PhysicsComponent* ptrphy{};
        ColliderComponent* ptrcol{};
        ectx.em.forEach<noCMP, obstacleTag>([&](Entity& e, PhysicsComponent& phy, ColliderComponent& col) {
            e.getID();
            auto auxdis = pos.distance(phy.position);
            if (auxdis < mindistance) {
                mindistance = auxdis;
                ptrphy = &phy;
                ptrcol = &col;
            }
        });

        if (mindistance <= 15.0) {
            auto dirtoobstacle = ptrphy->position - pos;
            dirtoobstacle.normalize();
            RayCast ray{ pos, dirtoobstacle };
            auto& bboxobstacle = ptrcol->bbox;
            if (bboxobstacle.intersectsRay(ray.origin, ray.direction)) {
                double angle = ectx.phy.orientation + K_PI / 2;
                vec3d perpendicular(sin(angle), 0, cos(angle));

                auto maxseparate{ 0.1 };
                if (mindistance < 5.0) {
                    maxseparate = 1.7;
                } else if (mindistance < 10.0) {
                    maxseparate = 0.8;
                } else {
                    maxseparate = 0.5;
                }
                vec3d avoidanceForce = perpendicular * maxseparate;
                
                steering.v_x += steering.v_x * 0.5;
                steering.v_z += steering.v_z * 0.5;
                steering.v_x += avoidanceForce.x();
                steering.v_z += avoidanceForce.z();
            }
        }
        return steering;
    }
};
