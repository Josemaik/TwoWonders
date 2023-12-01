#include "life_system.hpp"

void LifeSystem::update(EntityManager& em) {
    em.forEach<SYSCMPs, SYSTAGs>([](Entity&, LifeComponent& lif)
    {
        lif.decreaseCountdown();
    });
}