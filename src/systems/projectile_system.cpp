#include "projectile_system.hpp"

void ProjectileSystem::update(EntityManager& em) {
    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, ProjectileComponent& pro)
    {
        if(pro.checkRange()){
            em.destroyEntity(e.getID());
        }
    });
}