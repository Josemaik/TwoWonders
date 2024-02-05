#pragma once
#include "node.hpp"
#include <utils/vec3D.hpp>
#include <cstdlib>
#include <ctime>

struct BTDecisionFleeorCurePartner : BTNode_t{

    BTDecisionFleeorCurePartner()  {}

    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        ectx.ai.bh = "flee or cure partner";
        // Semilla
        std::srand(static_cast<unsigned int>(std::time(0)));
        // Genera un número aleatorio entre 0 y 99
        int randomNum = std::rand() % 100;
        // Si el número es menor o igual a 25, go to heal mate
        if (randomNum <= 25) {
            return BTNodeStatus_t::fail;
        }
        // Si no, toma la decisión B (75%), go to heal itself
        else {
            return BTNodeStatus_t::success;
        }
    }
};
