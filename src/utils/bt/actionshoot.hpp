#pragma once
#include "node.hpp"
#include <utils/types.hpp>


struct BTActionShoot : BTNode_t{
    using type_value = AIComponent::TypeShoot;
    BTActionShoot(type_value t) : shoot{t} {}
    //   BTDecisionAlternative( a = false) : alternative{a}  {}

    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        auto& att = ectx.em.getComponent<AttackComponent>(ectx.ent);
        if (ectx.ai.elapsed_shoot >= ectx.ai.countdown_shoot) {
             ectx.ai.elapsed_shoot = 0;
            switch (shoot)
            {
            case AIComponent::TypeShoot::OneShoot : {
                // shoot one time
                att.vel = ectx.ai.oldvel;
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