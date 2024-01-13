#pragma once
#include "node.hpp"
#include <utils/types.hpp>


struct BTActionShoot : BTNode_t{
    using type_value = AIComponent::TypeShoot;
    BTActionShoot(type_value t) : shoot{t} {}
    //   BTDecisionAlternative( a = false) : alternative{a}  {}
    // Obtener la distancia del enemigo con respecto al player
    [[nodiscard]]vec3d getPlayerDistance(EntityContext_t& ectx) const noexcept {
            auto& li = ectx.em.getSingleton<LevelInfo>();
            auto* playerEn = ectx.em.getEntityByID(li.playerID);
            if (not playerEn) { ectx.ai.playerdetected = false; return vec3d{}; };
            auto& plphy = ectx.em.getComponent<PhysicsComponent>(*playerEn);
            auto const distance = plphy.position - ectx.phy.position;
            return  distance;
    }
    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        auto& att = ectx.em.getComponent<AttackComponent>(ectx.ent);
        // if (ectx.ai.elapsed_shoot >= ectx.ai.countdown_shoot) {
            // if(ectx.ai.ghost){
            //     //rend a no visibles
            //      auto& rend1 = ectx.em.getComponent<RenderComponent>(ectx.ent);
            //      rend1.visible = false;
            // }
            // ectx.ai.elapsed_shoot = 0;
            if(ectx.ai.ready_attack){
                 //std::cout << "HE DISPARADO \n";
                ectx.ai.ready_attack = false;
                    switch (shoot)
                {
                case AIComponent::TypeShoot::OneShootonDir : {
                    // shoot one time
                    att.vel = ectx.ai.oldvel;
                    att.attack(AttackType::Ranged);
                    return BTNodeStatus_t::success;
                }
                    break;
                case  AIComponent::TypeShoot::OneShoottoPlayer : {
                    att.vel = (getPlayerDistance(ectx)).normalized() * ectx.ai.SPEED_AI;
                    att.attack(AttackType::Ranged);
                    return BTNodeStatus_t::success;
                }
                    break;
                case AIComponent::TypeShoot::TripleShoot: {
                    //shoot three time
                    att.vel = (getPlayerDistance(ectx)).normalized() * ectx.ai.SPEED_AI;
                    att.attack(AttackType::TripleShot);
                    return BTNodeStatus_t::success;
                }
                case AIComponent::TypeShoot::Melee: {
                    //shoot three time
                    att.vel = (getPlayerDistance(ectx)).normalized() * ectx.ai.SPEED_AI;
                    att.attack(AttackType::Melee);
                    return BTNodeStatus_t::success;
                }
                    break;
                default:
                    break;
                }
            }
          std::cout << "NO HE DISPARADO \n";
        // }
        // Si no puedo atacar por culldown pero sigo en radio de ataque
        // if(ectx.ai.on_attack_radius){
        //     ectx.ai.on_attack_radius = false;
            return BTNodeStatus_t::success;
        // }else{
        //     return BTNodeStatus_t::fail;
        // }


    }
private:
   type_value shoot;
};
