#include "animation_system.hpp"

void AnimationSystem::update(EntityManager& em, GameEngine& engine) {
    em.forEach<SYSCMPs, SYSTAGs>([&](Entity&, RenderComponent& rc, AnimationComponent& ac)
    {
        if (ac.currentAnimation == std::numeric_limits<std::size_t>::max())
        {
            auto id = engine.playAnimation(ac.animationList[0]);
            auto& meshes = rc.node->getEntity<DarkMoon::Model>()->getMeshes();

            for (auto& mesh : meshes) {
                if (mesh->hasBones)
                    mesh->animID = id;
            }
            ac.currentAnimation = 0;
        }
    });

    engine.updateAnimations();
}