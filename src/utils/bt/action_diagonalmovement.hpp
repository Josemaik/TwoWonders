#pragma once
#include "node.hpp"
#include <random>
#include <utils/types.hpp>


struct BTActionDiagonalMovement : BTNode_t {
    // BTActionPatrol() = default;


    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        vec3d direction{};
        //check change direction when not shooting
        if (ectx.ai->elapsed_change_dir >= ectx.ai->countdown_change_dir) {
            //set random dir
            direction = ectx.ai->getRandomDirwithDiagonals();
            ectx.ai->oldvel = direction;
            ectx.ai->elapsed_change_dir = 0;
        }
        ectx.ai->plusDeltatime(ectx.deltaTime, ectx.ai->elapsed_change_dir);
        //check if ai have to stops
        if (ectx.ai->elapsed_stop >= ectx.ai->countdown_stop) {
            ectx.ai->elapsed_stop = 0;
            ectx.phy.velocity = {};
        }
        ectx.ai->plusDeltatime(ectx.deltaTime, ectx.ai->elapsed_stop);

        if (!ectx.ai->isInDesiredRange(ectx.phy.position + ectx.ai->oldvel, ectx.ai->Xmin, ectx.ai->Xmax, ectx.ai->Zmin, ectx.ai->Zmax)) {
            ectx.ai->oldvel *= -1.0f;
        }
        ectx.phy.velocity = ectx.ai->oldvel;
        return BTNodeStatus_t::success;
    }
};
