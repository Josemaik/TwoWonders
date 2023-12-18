#pragma once
#include "node.hpp"
#include <random>
#include <utils/types.hpp>

struct BTActionRandomShoot : BTNode_t{
    // BTActionRandomShoot() = default;
    // // Devuelve una dirección aleatoria
    vec3f getRandomDir() {
        // Genero direccion aleatoria
        switch (std::rand() % 4) {
        case 0:  return { 0.25f, 0.0f, 0.0f }; break;//derecha
        case 1:  return { -0.25f, 0.0f, 0.0f }; break; //izquieda
        case 2:  return { 0.0f, 0.0f, 0.25f }; break; //Abajo
        case 3:  return { 0.0f, 0.0f, -0.25f }; break; //Arriba
        default: return { -0.25f, 0.0f, 0.0f }; break;
        }
    }
    bool isInDesiredRange(const vec3f& direction, float xmin, float xmax, float zmin, float zmax) {
        return direction.x() >= xmin && direction.x() <= xmax &&
        direction.z() >= zmin && direction.z() <= zmax;
    }
    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        vec3f direction{};
        //check change direction when not shooting
        if (!ectx.ai.stoped) {
            if (ectx.ai.elapsed_change_dir >= ectx.ai.countdown_change_dir) {
                //set random dir
                direction = getRandomDir();
                ectx.ai.oldvel = direction;
                ectx.ai.elapsed_change_dir = 0;
            }
            ectx.ai.dec_countdown_change_dir(ectx.deltatime);
        }
        //check if ai have to stops
        if (!ectx.ai.shoot) {
            if (ectx.ai.elapsed_stop >= ectx.ai.countdown_stop) {
                ectx.ai.stoped = true;
                ectx.ai.shoot = true;
                ectx.ai.elapsed_stop = 0;
                ectx.ai.elapsed_change_dir = 0;
                //shoot one time
                auto& att = ectx.em.getComponent<AttackComponent>(ectx.ent);
                att.vel = ectx.ai.oldvel;
                att.attack(AttackType::Ranged);
            }
            ectx.ai.dec_countdown_stop(ectx.deltatime);
        }
        //time while shooting
        if (ectx.ai.shoot) {
            if (ectx.ai.elapsed_shoot >= ectx.ai.countdown_shoot) {
                ectx.ai.shoot = false;
                ectx.ai.stoped = false;
                ectx.ai.elapsed_shoot = 0;
            }
            ectx.ai.dec_countdown_shoot(ectx.deltatime);
        }
        //Set velocity
        // setVelocityinRange(ectx.phy,ectx.ai);
        // void AISystem::setVelocityinRange(PhysicsComponent& p,CMP& cmp){
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