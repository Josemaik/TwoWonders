#include "physics_system.hpp"
#include <math.h>

void PhysicsSystem::update(EntityManager& em)
{
    auto& frti = em.getSingleton<FrustumInfo>();
    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, PhysicsComponent& phy, ColliderComponent& col)
    {
        if ((!e.hasTags(FrustOut{}) && frti.bboxIn(col.bbox) == FrustPos::OUTSIDE) || e.hasTag<EnemyDeathTag>())
            return;

        if (phy.notMove)
        {
            if (phy.velocity != vec3d::zero() && phy.target != vec3d::zero())
            {
                if (phy.position.distance(phy.target) < 8.0)
                    phy.velocity = vec3d::zero();
                else
                {
                    phy.position += phy.velocity;
                }
            }
            return;
        }
        // Cuando el jugador se para por un tiempo determinado
        if (phy.elapsed_afterStop < phy.countdown_afterStop)
        {
            phy.plusDeltatime(timeStep30, phy.elapsed_afterStop);
            if (phy.stopped)
                phy.stopped = false;
        }
        else if (phy.stopped)
        {
            if (phy.elapsed_stopped >= phy.countdown_stopped)
            {
                phy.elapsed_stopped = 0;
                phy.stopped = false;
                phy.gravity = phy.KGravity;
                phy.elapsed_afterStop = 0.0f;
                return;
            }
            else
                phy.plusDeltatime(timeStep30, phy.elapsed_stopped);
        }

        // Sacamos referencias a la posición y velocidad
        auto& pos = phy.position;
        auto& vel = phy.velocity;

        // Aplicar gravedad y hacer Clamp
        vel.setY(vel.y() - phy.gravity);

        // Normalizamos la velocidad
        if (std::abs(vel.x()) > phy.max_speed || std::abs(vel.y()) > phy.max_speed || std::abs(vel.z()) > phy.max_speed)
            vel.normalize();

        //Stuneo al jugador durante un tiempo provocado por el golpe de un golem
        if (phy.dragActivatedTime) {
            phy.dragActivated = true;
            if (phy.elapsed_stunned >= phy.countdown_sttuned) {
                phy.elapsed_stunned = 0;
                phy.dragActivatedTime = false;
            }
            phy.plusDeltatime(timeStep, phy.elapsed_stunned);
        }

        //Stunear o Ralentizar al player
        if (e.hasTag<PlayerTag>())
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

            // Normalizamos la velocidad del jugador siempre
            auto& plfi = em.getSingleton<PlayerInfo>();
            if (!plfi.onLadder)
                vel.normalize();

            if (plfi.hasBoots && !phy.stopped && !plfi.onLadder)
            {
                auto multiplier = 1.4;
                vel *= {multiplier, 1.0, multiplier};
            }
        }

        // Si estamos en una rampa queremos ir un poco más lento
        if (phy.onRamp)
        {
            vel.setX(vel.x() / 1.5);
            vel.setZ(vel.z() / 1.5);
        }

        // Colocamos la posición
        pos.setX((pos.x() + vel.x()));
        pos.setY((pos.y() + vel.y()));
        pos.setZ((pos.z() + vel.z()));

        if (!phy.stopped && (vel.x() != 0 || vel.z() != 0)) {
            phy.orientation = std::atan2(vel.x(), vel.z());
        }

        //Orientamos a enemigos hacia el player si están parados
        if (e.hasTag<SpiderTag>() || e.hasTag<SnowmanTag>() || e.hasTag<GolemTag>()) {
            if (e.hasComponent<AIComponent>())
            {
                auto& ia = em.getComponent<AIComponent>(e);
                if (ia.playerdetected) {
                    auto& bb = em.getSingleton<BlackBoard_t>();
                    vec3d targetpos{ bb.tx,0.0,bb.tz };
                    vec3d direction = targetpos - phy.position;
                    phy.orientation = std::atan2(direction.x(), direction.z());
                }
            }
        }
        if (e.hasTag<PlayerTag>()){

            auto& ss = em.getSingleton<SoundSystem>();
            if ((phy.velocity.x() != 0 || phy.velocity.z() != 0) && !playerWalking) {
                auto& li = em.getSingleton<LevelInfo>();
                ss.play_pasos();
                playerWalking = true;
                
            }
            else if ((phy.velocity.x() == 0 && phy.velocity.z() == 0) &&  playerWalking)
            {
                playerWalking = false;
                ss.SFX_pasos_stop();
            }
        }
        auto& li = em.getSingleton<LevelInfo>();
        if (e.hasTag<GolemTag>() ){
            
            auto& player = *em.getEntityByID(li.playerID);
            auto& playerPhy = em.getComponent<PhysicsComponent>(player);
            auto& playerPos = playerPhy.position;
            //phy.position
            //phy.position.distance(playerPos);
            auto& ss = em.getSingleton<SoundSystem>();
            if (phy.velocity.x() != 0 || phy.velocity.z() != 0) {
                auto& li = em.getSingleton<LevelInfo>();
                //ss.sonido_golem_mov();
                //playerWalking = true;
                
            }
        }
        
        // }
    });
}
