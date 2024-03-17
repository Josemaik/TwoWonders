#include "physics_system.hpp"
#include <math.h>

void PhysicsSystem::update(EntityManager& em, float dt)
{
    em.forEach<SYSCMPs, SYSTAGs>([dt, &em](Entity& ent, PhysicsComponent& phy)
    {
        // Cuando el jugador se para por un tiempo determinado
        if (phy.stopped)
        {
            if (phy.elapsed_stopped >= phy.countdown_stopped)
            {
                phy.elapsed_stopped = 0;
                phy.stopped = false;
                return;
            }
            else
                phy.plusdeltatime(dt, phy.elapsed_stopped);
        }
        // Sacamos referencias a la posición y velocidad
        auto& pos = phy.position;
        auto& vel = phy.velocity;

        // Actualizamos el estado anterior
        phy.previousState.position = pos;
        phy.previousState.velocity = vel;
        phy.previousState.scale = phy.scale;
        phy.previousState.orientation = phy.orientation;

        // Aplicar gravedad y hacer Clamp
        vel.setY(vel.y() - phy.gravity);

        // Normalizamos la velocidad
        if (std::abs(vel.x()) > phy.max_speed || std::abs(vel.y()) > phy.max_speed || std::abs(vel.z()) > phy.max_speed)
        {
            vel.normalize();
        }

        //Stuneo al jugador durante un tiempo provocado por el golpe de un golem
        if (phy.dragActivatedTime) {
            phy.dragActivated = true;
            if (phy.elapsed_stunned >= phy.countdown_sttuned) {
                phy.elapsed_stunned = 0;
                phy.dragActivatedTime = false;
            }
            phy.plusdeltatime(dt, phy.elapsed_stunned);
        }

        //Stunear o Ralentizar al player
        if (ent.hasTag<PlayerTag>())
        {
            if (phy.dragActivated) {
                phy.dragActivated = false;
                // float dragFactor = 0.3f;
                // vel -= dragFactor;
                vel /= phy.kDrag;
            }
            else {
                // si el player no esta siendo ralentizado-> no esta siendo capturado por tearaña
                em.getSingleton<BlackBoard_t>().playerhunted = false;
            }
        }

        pos.setX((pos.x() + vel.x()));
        pos.setY((pos.y() + vel.y()));
        pos.setZ((pos.z() + vel.z()));

        if (!phy.stopped && (vel.x() != 0 || vel.z() != 0)) {
            phy.orientation = std::atan2(vel.x(), vel.z());
        }

        //Orientamos a enemigos hacia el player si están parados
        if (ent.hasTag<SpiderTag>() || ent.hasTag<SnowmanTag>()) {
            if (ent.hasComponent<AIComponent>())
            {
                auto& ia = em.getComponent<AIComponent>(ent);
                if (ia.playerdetected) {
                    auto& bb = em.getSingleton<BlackBoard_t>();
                    vec3d targetpos{ bb.tx,0.0,bb.tz };
                    vec3d direction = targetpos - phy.position;
                    phy.orientation = std::atan2(direction.x(), direction.z());
                }
            }
        }

        // comprobar si están en el suelo
        if (phy.alreadyGrounded)
            phy.alreadyGrounded = false;
    });
}
