#include "life_system.hpp"

void LifeSystem::update(EntityManager& em) {
    em.forEach<SYSCMPs, SYSTAGs>([](Entity&, LifeComponent& lif)
    {
        lif.decreaseCountdown();
    });

    // Destruir las entidades que tengan vida a 0
    size_t count = 0;
    for (auto const& e : em.getEntities())
        if (e.hasComponent<LifeComponent>() &&  em.getComponent<LifeComponent>(e).life == 0)
            em.destroyEntity(count);
}