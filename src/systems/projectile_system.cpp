#include "projectile_system.hpp"

void ProjectileSystem::update(EntityManager& em, float deltaTime) {

    auto& li = em.getSingleton<LevelInfo>();

    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, ProjectileComponent& pro)
    {
        if (pro.checkRange(deltaTime)) {
            if (e.hasComponent<LifeComponent>()) {
                em.getComponent<LifeComponent>(e).markedForDeletion = true;

                if (e.hasComponent<ColliderComponent>()) {
                    if (em.getComponent<ColliderComponent>(e).attackType == AttackType::Spiderweb) {
                        em.getComponent<LifeComponent>(e).life = 0;
                    }
                }
                else
                    li.dead_entities.insert(e.getID());
            }
        }
    });
}
