#include "animation_system.hpp"

void AnimationSystem::update(EntityManager& em, GameEngine& engine)
{
    auto& frti = em.getSingleton<FrustumInfo>();
    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, RenderComponent& rc, AnimationComponent& ac)
    {
        if (!frti.inFrustum(e.getID()))
            return;

        if (ac.currentAnimation == std::numeric_limits<std::size_t>::max())
        {
            auto id = engine.playAnimation(ac.animationList[0]);
            auto& meshes = rc.node->getEntity<Model>()->getMeshes();

            for (auto& mesh : meshes) {
                mesh->animID = id;
            }
            ac.currentAnimation = 0;
        }
    });

    engine.updateAnimations();
}