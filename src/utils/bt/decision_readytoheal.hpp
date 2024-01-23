#pragma once
#include "node.hpp"
#include <utils/vec3D.hpp>


struct BTDecisionOnAttackRadius : BTNode_t{

    BTDecisionOnAttackRadius()  {}

    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
         ectx.ai.bh = "check ready heal mate";
        // comprobar radio de cura
        // recorrer unorderd map del bb , comprobar si tienen < de 3 vidas y si lo cumple, comprobar si esta en radio de cura. El primero que lo cumpla
        // sale del bucle
        // tx y tz la posicion de ese slime
    }
};
