#include "animation_system.hpp"
#include <omp.h>

void AnimationSystem::update(EntityManager& em, GameEngine& engine)
{
    auto& frti = em.getSingleton<FrustumInfo>();
    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, RenderComponent& rc, AnimationComponent& ac)
    {
        if (!frti.inFrustum(e.getID()) || ac.animationList.empty())
            return;

        if (ac.animEnded)
        {
            if (e.hasTag<PlayerTag>())
            {
                if (ac.currentAnimation != 10 && ac.currentAnimation != 0
                    && ac.currentAnimation != 9 && ac.currentAnimation != 5)
                {
                    ac.reset();
                    ac.animToPlay = static_cast<std::size_t>(PlayerAnimations::IDLE);
                }
            }
            else if (e.hasTag<ChestTag>())
            {
                engine.stopAnimation(ac.idCurrent);
                rc.node->getEntity<Model>()->animID = ac.max;

                ac.reset();
                ac.idCurrent = ac.max;
            }
            else if (e.hasTag<SnowmanTag>())
            {
                auto& aic = em.getComponent<AIComponent>(e);
                if ((!aic.playerdetected || aic.elapsed_stop < aic.countdown_stop) && (ac.currentAnimation == static_cast<std::size_t>(SnowmanAnimations::ATTACK) ||
                    ac.currentAnimation == static_cast<std::size_t>(SnowmanAnimations::GOT_HIT)))
                {
                    ac.reset();
                    ac.animToPlay = static_cast<std::size_t>(SnowmanAnimations::IDLE);
                }
            }
        }

        if (ac.stopAnim)
        {
            engine.dmeg.ChangeAnimation(ac.idCurrent, ac.animationList[ac.animToPlay]);
            auto aux = ac.idCurrent;
            auto aux2 = ac.animToPlay;

            ac.reset();
            ac.idCurrent = aux;
            ac.currentAnimation = aux2;
            ac.stopAnim = false;
        }

        if (ac.animToPlay != ac.max)
        {
            if (e.hasTag<PlayerTag>())
            {
                if (ac.currentAnimation == static_cast<std::size_t>(PlayerAnimations::MELEE_ATTACK) && !ac.aboutToEnd)
                {
                    ac.animToPlay = ac.max;
                    return;
                }

                ac.multiplier = playerSpeeds[ac.animToPlay];
            }

            else if (e.hasTag<SnowmanTag>())
                ac.multiplier = snowmanSpeeds[ac.animToPlay];

            // Seteamos la animación o la interpolamos con la anterior por fluidez
            if (ac.idCurrent != ac.max)
                engine.dmeg.InterpolateAnimation(ac.idCurrent, ac.animationList[ac.animToPlay]);
            else
            {
                ac.idCurrent = engine.playAnimation(ac.animationList[ac.animToPlay]);
                rc.node->getEntity<Model>()->animID = ac.idCurrent;
            }

            // Reseteamos valores
            ac.currentAnimation = ac.animToPlay;
            ac.animToPlay = ac.max;
            ac.currentTime = 0.0f;
            ac.animEnded = false;
            ac.aboutToEnd = false;
        }

        if (ac.currentAnimation != ac.max)
        {
            engine.updateAnimation(timeStep, ac.multiplier, ac.idCurrent);

            if (!engine.dmeg.IsInterpolating(ac.idCurrent))
            {
                float currentTime = engine.getAnimationTime(ac.idCurrent);
                if (currentTime < ac.currentTime)
                    ac.animEnded = true;

                ac.currentTime = currentTime;

                float offsetTime = 0.5f;
                if (e.hasTag<SnowmanTag>())
                    offsetTime = 1.3f;

                if (ac.currentTime > ac.animationList[ac.currentAnimation]->getSekDuration() - offsetTime)
                    ac.aboutToEnd = true;

                if (e.hasTag<PlayerTag>() && ac.currentAnimation == 1 && ac.currentTime > 2.0f)
                    em.getComponent<AttackerComponent>(e).attack(AttackType::MeleePlayer);
            }
        }

        if (ac.aboutToEnd)
        {
            if (e.hasTag<DoorTag>())
            {
                auto& li = em.getSingleton<LevelInfo>();
                li.insertDeath(e.getID());
                engine.stopAnimation(ac.idCurrent);
            }
        }
    });
}