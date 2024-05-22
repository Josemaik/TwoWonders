#pragma once
#include "node.hpp"
#include <random>
#include <cmath>
#include <numbers>
#include <algorithm>
#include <utils/sb/steeringbehaviour.hpp>

//Curarye a tí mismo
struct BTAction_Healing : BTNode_t {
    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        if (!ectx.ai->tactive) return BTNodeStatus_t::fail;
        ectx.ai->bh = "Healing";
        ectx.lifec->increaseLife();
        return BTNodeStatus_t::success;
    }
};
