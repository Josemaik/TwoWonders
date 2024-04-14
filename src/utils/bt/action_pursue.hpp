#pragma once
#include "node.hpp"
#include <cmath>
#include <numbers>
#include <algorithm>
#include <utils/sb/steeringbehaviour.hpp>

struct BTAction_Pursue : BTNode_t {
    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        if (!ectx.ai->tactive) return BTNodeStatus_t::fail;
        ectx.ai->bh = "pursuing";
        //get target entity by id
        auto const& e_opt{ ectx.em.getEntityByID(ectx.ai->teid) };
        if (!e_opt) { ectx.ai->tactive = false; return BTNodeStatus_t::running; };
        auto const& phyTarget = ectx.em.getComponent<PhysicsComponent>(*e_opt);
        Steer_t steering = STBH::Pursue(phyTarget, ectx.phy);
        // ectx.phy.a_linear = steering.linear;
        // ectx.phy.v_angular = steering.angular;

        using noCMP = MP::TypeList<PhysicsComponent,ColliderComponent>;
        using obstacleTag = MP::TypeList<ObstacleTag>;

        auto& pos = ectx.phy.position;
        ectx.em.forEach<noCMP, obstacleTag>([&](Entity& e, PhysicsComponent& phy,ColliderComponent& col) {
                e.getID();
                auto distance = pos.distance(phy.position);
                if(distance <= 15.0){
                    auto dirtoobstacle = phy.position - pos;
                    dirtoobstacle.normalize();
                    RayCast ray {pos,dirtoobstacle};
                    auto& bboxobstacle = col.boundingBox;
                    if(bboxobstacle.intersectsRay(ray.origin,ray.direction)){
                        double angle = ectx.phy.orientation + M_PI / 2; // Add 90 degrees to the orientation
                        vec3d perpendicular(cos(angle), 0, sin(angle));

                        auto maxseparate = 0.5;
                        if(distance <= 15.0){
                            maxseparate = 1.5;
                        }else if(distance < 10.0){
                            maxseparate = 2.5;
                        }else if( distance < 5.0){
                            maxseparate = 3.5;
                        }
                        vec3d avoidanceForce = perpendicular * maxseparate;
                        // Steer_t steeringEvade = STBH::Evade(ectx.phy,phy,maxseparate);
                        steering.v_x += avoidanceForce.x();
                        steering.v_z += avoidanceForce.z();
                    }
                // // Calculate a vector perpendicular to the orientation
                // double angle = phy.orientation + M_PI / 2; // Add 90 degrees to the orientation
                // vec3d perpendicular(cos(angle), 0, sin(angle));

                // // Scale the perpendicular vector by a factor that depends on the distance to the obstacle
                // float scale = 1.0 - (pos.distance(phy.position) / 15.0);
                // vec3d avoidanceForce = perpendicular * scale;

                // // Add the avoidance force to the steering velocity
                // steering.v_x += avoidanceForce.x();
                // steering.v_z += avoidanceForce.z();
                }
        });


        ectx.phy.velocity = vec3d{ steering.v_x,0.0,steering.v_z };
        return BTNodeStatus_t::success;
    }
};
