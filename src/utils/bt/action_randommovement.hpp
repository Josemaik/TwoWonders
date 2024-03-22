#pragma once
#include "node.hpp"
#include <random>
#include <utils/types.hpp>

struct BTActionRandomMovement : BTNode_t{
    // BTActionRandomShoot() = default;
    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        vec3d direction{};
        //check change direction when not shooting
            if (ectx.ai->elapsed_change_dir >= ectx.ai->countdown_change_dir) {
                //set random dir
                direction = ectx.ai->getRandomDir();
                ectx.ai->oldvel = direction;
                ectx.ai->elapsed_change_dir = 0;
            }
            ectx.ai->plusdeltatime(ectx.deltatime,ectx.ai->elapsed_change_dir);
        //check if ai have to stops
            if (ectx.ai->elapsed_stop >= ectx.ai->countdown_stop) {
                ectx.ai->elapsed_stop = 0;
                ectx.phy.velocity = {};
                return BTNodeStatus_t::success;
            }
            ectx.ai->plusdeltatime(ectx.deltatime,ectx.ai->elapsed_stop);

            if (!ectx.ai->isInDesiredRange(ectx.phy.position + ectx.ai->oldvel, ectx.ai->Xmin, ectx.ai->Xmax, ectx.ai->Zmin, ectx.ai->Zmax)) {
                ectx.ai->oldvel *= -1.0f;
            }
            ectx.phy.velocity = ectx.ai->oldvel;
            return BTNodeStatus_t::running;
    }
};
