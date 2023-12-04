#include "life_system.hpp"

void LifeSystem::update(EntityManager& em) {
    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, LifeComponent& lif)
    {
        lif.decreaseCountdown();

        if (lif.life == 0)
        {
            // Eliminamos la entidad
            em.destroyEntity(e.getID());
        }

        // Esto sirve pa que el entity manager no se raye si dos entidades se destruyen a la vez
        if (lif.decreaseLifeNextFrame)
        {
            lif.decreaseLife();
            lif.decreaseLifeNextFrame = false;
        }
    });
}