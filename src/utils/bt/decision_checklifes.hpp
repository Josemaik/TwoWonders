#pragma once
#include "node.hpp"
#include <utils/vec3D.hpp>

//comprobar si tu vida es inferior a 3
struct BTDecisionCheckLifes : BTNode_t{

    BTDecisionCheckLifes()  {}

    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
         ectx.ai->bh = "checking lifes";
        if(ectx.lifec->life < 3){
            return BTNodeStatus_t::success;
        }
        return BTNodeStatus_t::fail;
    }

};
