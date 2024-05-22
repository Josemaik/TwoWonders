#pragma once
#include "node.hpp"
#include <utils/sb/steeringbehaviour.hpp>

//Movimiento de péndulo
struct BTAction_Pendulum : BTNode_t {

    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        if (!ectx.ai->tactive) return BTNodeStatus_t::fail;
        ectx.ai->bh = "penduling";
        if (ectx.ent.hasComponent<AngryBushComponent>()) {
            auto& abc = ectx.em.getComponent<AngryBushComponent>(ectx.ent);

            if (abc.chargeAttack && !abc.move) {
                auto& anc = ectx.em.getComponent<AnimationComponent>(ectx.ent);
                ectx.phy.velocity = vec3d{};
                if (anc.aboutToEnd) {
                    anc.aboutToEnd = false;
                    ectx.ai->elapsed_stop = 0;
                    abc.chargeAttack = false;
                    abc.move = true;
                    abc.vel = -abc.vel;
                    ectx.phy.max_speed = abc.max_speed;
                    abc.angrySoundOneTime = true;
                    ectx.em.getComponent<AnimationComponent>(ectx.ent).animToPlay = static_cast<std::size_t>(RockAnimations::ROLL);
                }
                else if (abc.angrySound)
                {
                    auto vel = (ectx.ai->path.m_waypoints[0] - ectx.phy.position).normalize();
                    ectx.phy.orientation = atan2(vel.x(), vel.z());

                    auto& li = ectx.em.getSingleton<LevelInfo>();
                    auto& playerPos = ectx.em.getComponent<PhysicsComponent>(*ectx.em.getEntityByID(li.playerID)).position;
                    if (playerPos.distance(ectx.phy.position) < 28.0)
                        ectx.em.getSingleton<SoundSystem>().sonido_piedras_golpe();
                    abc.angrySound = false;
                }
            }

            if (abc.move) {
                if (abc.dir == vec3d::zero() && abc.vel == vec3d::zero()) {
                    abc.dir = ectx.ai->path.m_waypoints[0];
                    abc.vel = (abc.dir - ectx.phy.position).normalize();
                }

                ectx.phy.velocity = { abc.vel.x() * ectx.phy.max_speed, 0.0, abc.vel.z() * ectx.phy.max_speed };

                if (ectx.phy.max_speed < 3.0)
                    ectx.phy.max_speed += 0.1;
                return BTNodeStatus_t::success;
            }

            return BTNodeStatus_t::success;
        }

        return BTNodeStatus_t::success;
    }
};