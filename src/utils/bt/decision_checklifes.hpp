#pragma once
#include "node.hpp"
#include <utils/vec3D.hpp>


struct BTDecisionCheckLifes : BTNode_t{

    BTDecisionCheckLifes()  {}

    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        std::cout << "CHECK LIFES \n";
        if(ectx.lifec.life < 3){
            // if(ectx.lifec.life == 0){
            //  std::cout << "EXPLOTAR \n";
            //  auto& att = ectx.em.getComponent<AttackComponent>(ectx.ent);
            //  att.attack(AttackType::Bomb);
            // }
            return BTNodeStatus_t::success;
        }
        return BTNodeStatus_t::fail;
    }

// private:
//     bool alternative { false };
};
