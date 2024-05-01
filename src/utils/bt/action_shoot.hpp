#pragma once
#include "node.hpp"
#include <utils/types.hpp>


struct BTActionShoot : BTNode_t {
    using type_value = AIComponent::TypeShoot;
    BTActionShoot(type_value t, vec3d p) : shoot{ t }, pos{ p } {}

    //   BTDecisionAlternative( a = false) : alternative{a}  {}
    // Obtener la distancia del enemigo con respecto al player
    [[nodiscard]] vec3d getPlayerDistance(EntityContext_t& ectx) const noexcept {
        auto& li = ectx.em.getSingleton<LevelInfo>();
        auto* playerEn = ectx.em.getEntityByID(li.playerID);
        if (not playerEn) { ectx.ai->playerdetected = false; return vec3d{}; };
        auto& plphy = ectx.em.getComponent<PhysicsComponent>(*playerEn);
        auto const distance = plphy.position - ectx.phy.position;
        return  distance;
    }
    PhysicsComponent& getplayerphy(EntityContext_t& ectx) {
        auto& li = ectx.em.getSingleton<LevelInfo>();
        auto* playerEn = ectx.em.getEntityByID(li.playerID);
        //if (not playerEn) return vec3d{}; // No hay player
        // Si hay player
        auto& plphy = ectx.em.getComponent<PhysicsComponent>(*playerEn);
        return plphy;
    };
    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        auto& att = ectx.em.getComponent<AttackComponent>(ectx.ent);
        ectx.ai->bh = "shooting";
        // if (ectx.ai->elapsed_shoot >= ectx.ai->countdown_shoot) {
            // if(ectx.ai->ghost){
            //     //rend a no visibles
            //      auto& rend1 = ectx.em.getComponent<RenderComponent>(ectx.ent);
            //      rend1.visible = false;
            // }
            // ectx.ai->elapsed_shoot = 0;
        if (ectx.ai->ready_attack) {
            ectx.ai->ready_attack = false;
            switch (shoot)
            {
            case AIComponent::TypeShoot::OneShootonDir: {
                // shoot one time
                att.vel = ectx.ai->oldvel;
                att.attack(AttackType::Ranged);
                return BTNodeStatus_t::success;
                break;
            }
            case  AIComponent::TypeShoot::OneShoottoPlayer: {
                auto& li = ectx.em.getSingleton<LevelInfo>();
                att.vel = (getPlayerDistance(ectx)).normalized() * ectx.ai->SPEED_AI;

                if (ectx.ent.hasTag<SnowmanTag>() && li.mapID == 2) {
                    att.attack(AttackType::FireBallShot);
                    //Se ajuste la velocidad y altura del atque segun a que distancia se encuentre
                    auto& plphy = getplayerphy(ectx);
                    auto distance = ectx.phy.position.distance(plphy.position);

                    if(plphy.velocity.x() != 0.0 && plphy.velocity.z() != 0.0){
                        Steer_t steering = STBH::Pursue(plphy,ectx.phy,50.0);
                        if(distance >= 11.0 && distance <= 14.1){
                            att.vel = vec3d{ steering.v_x,att.vel.y()+0.2, steering.v_z};
                        }else{
                            att.vel = vec3d{ steering.v_x,att.vel.y()+0.3, steering.v_z};
                        }
                    }else{
                        if(distance >= 11.0 && distance <= 14.1){
                            att.vel = { att.vel.x() * 1.0, att.vel.y() + 0.6, att.vel.z() * 1.0 };
                        }else{
                            att.vel = { att.vel.x() * 1.4, att.vel.y() + 0.7, att.vel.z() * 1.4 };
                        }
                    }

                    
                    //pursue en un futuro, arreglar que sea mas preciso
                    // else{
                    //     Steer_t steering = STBH::Pursue(plphy,ectx.phy,0.5);
                    //     att.vel = vec3d{ steering.v_x/2.0,att.vel.y() + 0.5, steering.v_z/2.0};
                    //     //att.vel = vec3d(std::sin(steering.orientation), 0.0, std::cos(steering.orientation));
                    // }
                }
                else
                    att.attack(AttackType::Ranged);

                if (ectx.ent.hasTag<SnowmanTag>()) {
                    ectx.em.getSingleton<SoundSystem>().sonido_munyeco_ataque();
                }
                return BTNodeStatus_t::success;
                break;
            }
            case AIComponent::TypeShoot::TripleShoot: {
                //shoot three time
                att.vel = (getPlayerDistance(ectx)).normalized() * ectx.ai->SPEED_AI;
                att.attack(AttackType::TripleShot);
                return BTNodeStatus_t::success;
            }
            case AIComponent::TypeShoot::Melee: {
                //shoot three time
                att.vel = (getPlayerDistance(ectx)).normalized() * ectx.ai->SPEED_AI;
                att.attack(AttackType::Melee);
                if (ectx.ent.hasTag<GolemTag>()) {
                    ectx.em.getSingleton<SoundSystem>().sonido_golem_ataque();
                }
                return BTNodeStatus_t::success;
                break;
            }
            case AIComponent::TypeShoot::Air_attack: {
                auto& li = ectx.em.getSingleton<LevelInfo>();
                auto* playerEn = ectx.em.getEntityByID(li.playerID);
                auto& plphy = ectx.em.getComponent<PhysicsComponent>(*playerEn);
                att.pos_respawn_air_attack = plphy.position;
                att.attack(AttackType::AirAttack);
                return BTNodeStatus_t::fail;
                break;
            }
            case AIComponent::TypeShoot::CrusherAttack: {
                att.pos_respawn_crush_attack = pos;
                if (ectx.ent.hasComponent<ColliderComponent>())
                {
                    auto& col = ectx.em.getComponent<ColliderComponent>(ectx.ent);
                    att.pos_respawn_crush_attack.setY(col.bbox.min.y() + 1);
                }
                att.attack(AttackType::CrusherAttack);
                return BTNodeStatus_t::fail;
            }
            default:
                break;
            }
        }
        // }
        // Si no puedo atacar por culldown pero sigo en radio de ataque
        // if(ectx.ai->on_attack_radius){
        //     ectx.ai->on_attack_radius = false;
        return BTNodeStatus_t::success;
        // }else{
        //     return BTNodeStatus_t::fail;
        // }


    }
private:
    type_value shoot;
    vec3d pos;
};
