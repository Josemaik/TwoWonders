#pragma once
#include "node.hpp"
#include <random>
#include <utils/types.hpp>


struct BTActionBat : BTNode_t{
    // BTActionPatrol() = default;
    bool isInDesiredRange(const vec3f& direction, float xmin, float xmax, float zmin, float zmax) {
        return direction.x() >= xmin && direction.x() <= xmax &&
        direction.z() >= zmin && direction.z() <= zmax;
    }
    vec3f getRandomDirwithDiagonals() {
        // Genero direccion aleatoria
        switch (std::rand() % 8) {
        case 0:  return { 0.25f, 0.0f, 0.0f }; break; //derecha
        case 1:  return { -0.25f, 0.0f, 0.0f }; break; //izquieda
        case 2:  return { 0.0f, 0.0f, 0.25f }; break; //Abajo
        case 3:  return { 0.0f, 0.0f, -0.25f }; break; //Arriba
        case 4:  return { -0.25f, 0.0f, -0.25f}; break; //up-left
        case 5:  return { 0.25f, 0.0f, 0.25f }; break; //down-right
        case 6:  return { 0.25f, 0.0f, -0.25f }; break; //up-right
        case 7:  return { -0.25f, 0.0f, 0.25f }; break; //down-left
        default: return { -0.25f, 0.0f, 0.0f }; break;
        }
    }
    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        vec3f direction{};
        //check change direction
        if (!ectx.ai.stoped) {
            if (ectx.ai.elapsed_change_dir >= ectx.ai.countdown_change_dir) {
                //set random dir
                direction = getRandomDirwithDiagonals();
                ectx.ai.oldvel = direction;
                ectx.ai.elapsed_change_dir = 0;
            }
            ectx.ai.dec_countdown_change_dir(ectx.deltatime);
        }
        //check if ai have to stops
        if (!ectx.ai.moving) {
            if (ectx.ai.elapsed_stop >= ectx.ai.countdown_stop) {
                ectx.ai.stoped = true;
                ectx.ai.moving = true;
                ectx.ai.elapsed_stop = 0;
                ectx.ai.elapsed_change_dir = 0;
            }
            ectx.ai.dec_countdown_stop(ectx.deltatime);
        }
        //time to return moving
        if (ectx.ai.moving) {
            if (ectx.ai.elapsed_moving >= ectx.ai.countdown_moving) {
                //Shoot
                ectx.ai.moving = false;
                ectx.ai.stoped = false;
                ectx.ai.elapsed_moving = 0;
            }
            ectx.ai.dec_countdown_moving(ectx.deltatime);
        }
        //Set velocity
        // setVelocityinRange(p,ectx.ai);
        if (!ectx.ai.stoped) {
            //Range Control
            if (!isInDesiredRange(ectx.phy.position + ectx.ai.oldvel, ectx.ai.Xmin, ectx.ai.Xmax, ectx.ai.Zmin, ectx.ai.Zmax)) {
                ectx.ai.oldvel *= -1.0f;
            }
            ectx.phy.velocity = ectx.ai.oldvel;
        }
        else {
            ectx.phy.velocity = {};
        }
        return BTNodeStatus_t::success;
    }
};