#pragma once
#include "node.hpp"
#include <utils/types.hpp>


struct BTActionShoot : BTNode_t{
    using type_value = AIComponent::TypeShoot;
    BTActionShoot(type_value t) : shoot{t} {}
    //   BTDecisionAlternative( a = false) : alternative{a}  {}
     // Obtener la distancia del enemigo con respecto al player
    [[nodiscard]] vec3f getPlayerDistance(EntityManager& EM, PhysicsComponent const& p, AIComponent& ai) const noexcept {
        auto& li = EM.getSingleton<LevelInfo>();
        auto* playerEn = EM.getEntityByID(li.playerID);
        if (not playerEn) { ai.playerdetected = false; return vec3f{}; };
        auto& plphy = EM.getComponent<PhysicsComponent>(*playerEn);
        auto const distance = plphy.position - p.position;
        return  distance;
    }

    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        auto& att = ectx.em.getComponent<AttackComponent>(ectx.ent);
        if (ectx.ai.elapsed_shoot >= ectx.ai.countdown_shoot) {
            if(ectx.ai.ghost){
                //rend a no visibles
                 auto& rend1 = ectx.em.getComponent<RenderComponent>(ectx.ent);
                 rend1.visible = false;
            }
            ectx.ai.elapsed_shoot = 0;
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
                att.vel = (getPlayerDistance(ectx.em, ectx.phy, ectx.ai)).normalized() * ectx.ai.SPEED_AI;
                att.attack(AttackType::Ranged);
                return BTNodeStatus_t::success;
            }
                break;
            case AIComponent::TypeShoot::TripleShoot: {
                //shoot three time
                att.attack(AttackType::TripleShot);
                return BTNodeStatus_t::success;
            }
                break;
            default:
                break;
            }
        }
        ectx.ai.dec_countdown_shoot(ectx.deltatime);
        return BTNodeStatus_t::running; 
    }
private:
   type_value shoot;
};