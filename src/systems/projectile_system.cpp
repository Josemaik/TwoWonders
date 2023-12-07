#include "projectile_system.hpp"

void ProjectileSystem::update(EntityManager& em, float deltaTime) {
    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, ProjectileComponent& pro)
    {
        if (pro.checkRange(deltaTime)) {
            em.destroyEntity(e.getID());
        }
    });
}