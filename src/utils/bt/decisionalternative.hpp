#pragma once
#include "node.hpp"



struct BTDecisionAlternative : BTNode_t{

    BTDecisionAlternative(bool a = false) : alternative{a}  {}

    BTNodeStatus_t run(EntityContext_t&) noexcept final { // final es como override sin dejar sobreescribir
        alternative = !alternative;
        if ( alternative ) return BTNodeStatus_t::success;
        return BTNodeStatus_t::fail;
    }
    
private:
    bool alternative { false };
};