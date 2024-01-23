#pragma once
#include "node.hpp"
#include <utils/sb/steeringbehaviour.hpp>

struct BTAction_HealMate : BTNode_t{
    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        if( !ectx.ai.tactive ) return BTNodeStatus_t::fail;
        ectx.ai.bh = "Healing mate";
    }
};
