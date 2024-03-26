#pragma once
#include "node.hpp"
#include <utils/types.hpp>

struct BTActionChangePosition : BTNode_t {
    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        // if (!ectx.ai->shoot) {
        if (ectx.ai->elapsed_change_position >= ectx.ai->countdown_change_position) {
            //before change position go visible
            auto& rend = ectx.em.getComponent<RenderComponent>(ectx.ent);
            rend.visible = true;
            //Set random position
            vec3d randomPos = ectx.ai->getRandomPosinRange(ectx.ai->Xmin, ectx.ai->Xmax, ectx.ai->Zmin, ectx.ai->Zmax);
            ectx.phy.position.setX(randomPos.x());
            ectx.phy.position.setZ(randomPos.z());
            //Attack
            // ectx.ai->shoot = true;
            ectx.ai->elapsed_change_position = 0;
            return BTNodeStatus_t::success;
        }
        ectx.ai->plusDeltatime(ectx.deltaTime, ectx.ai->elapsed_change_position);
        return BTNodeStatus_t::running;
    }
};
