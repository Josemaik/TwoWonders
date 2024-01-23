#pragma once
#include "node.hpp"
#include <utils/vec3D.hpp>


struct BTDecisionOnAttackRadius : BTNode_t{

    BTDecisionOnAttackRadius()  {}

    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
         ectx.ai.bh = "check ready heal mate";
        // comprobar si la vida del compañero es < 3
        // comprobar radio de cura
        // si se cumple success
        // sino fail
    }
};
