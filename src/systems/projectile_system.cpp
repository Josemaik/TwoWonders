#include "projectile_system.hpp"

void ProjectileSystem::update(EntityManager& em, float deltaTime) {
    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, ProjectileComponent& pro)
    {
        if (pro.checkRange(deltaTime)) {
            if (e.hasComponent<LifeComponent>()){
                em.getComponent<LifeComponent>(e).markedForDeletion = true;
                if(e.hasComponent<ColliderComponent>()){
                    if(em.getComponent<ColliderComponent>(e).atackspider){
                        em.getComponent<LifeComponent>(e).life = 0;
                    }
                }
            }
            else
                dead_entities.insert(e.getID());
        }
    });

    if (!dead_entities.empty())
    {
        em.destroyEntities(dead_entities);
        dead_entities.clear();
    }
}
