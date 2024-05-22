#pragma once
#include "node.hpp"
#include <utils/vec3D.hpp>

//Comprobar si estás en rango para atacar
struct BTDecisionOnAttackRadius : BTNode_t{

    BTDecisionOnAttackRadius()  {}

    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
         ectx.ai->bh = "check attack radius";
        if(ectx.ai->on_attack_radius){
            ectx.ai->on_attack_radius = false;
            return BTNodeStatus_t::success;
        }else{
            return BTNodeStatus_t::fail;
        }
    }


};
