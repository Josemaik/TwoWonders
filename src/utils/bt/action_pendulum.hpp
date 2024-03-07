#pragma once
#include "node.hpp"
#include <utils/sb/steeringbehaviour.hpp>

struct BTAction_Pendulum : BTNode_t {

    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        if (!ectx.ai.tactive) return BTNodeStatus_t::fail;
        ectx.ai.bh = "penduling";
        if (ectx.ent.hasComponent<AngryBushComponent>()) {
            auto& abc = ectx.em.getComponent<AngryBushComponent>(ectx.ent);

            if (abc.chargeAttack && !abc.move) {
                ectx.phy.velocity = vec3d{};
                if (ectx.ai.elapsed_stop >= ectx.ai.countdown_stop) {
                    ectx.ai.elapsed_stop = 0;
                    abc.chargeAttack = false;
                    abc.move = true;
                    abc.vel = -abc.vel;
                    ectx.phy.max_speed = abc.max_speed;
                }
                else {
                    ectx.ai.plusdeltatime(ectx.deltatime, ectx.ai.elapsed_stop);
                }
            }

            if (abc.move) {
                if (abc.dir == vec3d::zero() && abc.vel == vec3d::zero()) {
                    abc.dir = ectx.ai.path.m_waypoints[0];
                    abc.vel = (abc.dir - ectx.phy.position).normalize();
                }

                ectx.phy.velocity = { abc.vel.x() * ectx.phy.max_speed, 0.0, abc.vel.z() * ectx.phy.max_speed };
                ectx.phy.max_speed += 0.1;
                return BTNodeStatus_t::success;
            }

            return BTNodeStatus_t::success;
        }

        return BTNodeStatus_t::success;
    }
};