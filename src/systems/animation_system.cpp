#include "animation_system.hpp"

void AnimationSystem::update(EntityManager& em, GameEngine& engine)
{
    auto& frti = em.getSingleton<FrustumInfo>();
    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, RenderComponent& rc, AnimationComponent& ac)
    {
        if (!frti.inFrustum(e.getID()))
            return;

        if (ac.animToPlay != ac.max)
        {
            ac.idCurrent = engine.playAnimation(ac.animationList[ac.animToPlay]);
            auto& meshes = rc.node->getEntity<Model>()->getMeshes();

            for (auto& mesh : meshes) {
                mesh->animID = ac.idCurrent;
            }

            if (e.hasTag<PlayerTag>())
            {
                switch (ac.animToPlay)
                {
                case 1:
                    ac.timer = 2.0f;
                    break;
                case 2:
                    ac.timer = 1.0f;
                    break;
                default:
                    break;
                }
            }

            if (e.hasTag<CrusherTag>() && e.hasComponent<AIComponent>())
                em.getComponent<AIComponent>(e).elapsed_shoot = 0;

            ac.currentAnimation = ac.animToPlay;
            ac.animToPlay = ac.max;
        }

        // if (ac.stopAnim)
        // {
        //     engine.stopAnimation(ac.currentAnimation);
        //     ac.currentAnimation = ac.max;
        //     ac.stopAnim = false;
        //     ac.idCurrent = ac.max;
        // }

        if (ac.currentAnimation != ac.max)
            engine.updateAnimations(ac.multiplier, ac.idCurrent);

        if (ac.timer > 0.1f)
        {
            ac.timer -= engine.getFrameTime();

            if (ac.timer < 0.1f)
            {
                ac.animToPlay = ac.animationList.size() - 1;
                ac.timer = 0.f;
            }
        }
    });
}