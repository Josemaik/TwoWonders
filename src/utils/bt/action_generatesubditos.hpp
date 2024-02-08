#pragma once
#include "node.hpp"
#include <utils/types.hpp>


struct BTDAction_GenerateSubditos : BTNode_t {
    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        ectx.ai.bh = "generating subditos";
        // comprobar radio de cura
        //obtenemos blackboard
        auto& bb = ectx.em.getSingleton<BlackBoard_t>();
        switch (bb.subditosData.size())
        {
        case 0:  bb.tam_subditos_tocreate = 2; return BTNodeStatus_t::fail;
            break;
        case 1: bb.tam_subditos_tocreate = 1; return BTNodeStatus_t::fail;
            break;
        default: return BTNodeStatus_t::fail;
            break;
        }
    }
};
