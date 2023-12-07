#include "life_system.hpp"

void LifeSystem::update(EntityManager& em, float deltaTime) {
    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& ent, LifeComponent& lif)
    {
        lif.decreaseCountdown(deltaTime);

        if (lif.life == 0)
        {
            // Si es enemigo creamos un objeto que cura
            if (ent.hasTag<EnemyTag>()) {
                // Crera la vida solo la mitad de las veces
                int random_value = std::rand();
                if (random_value % 2 == 0) {
                    auto& e{ em.newEntity() };
                    em.addTag<ObjectLifeTag>(e);
                    auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = em.getComponent<PhysicsComponent>(ent).position, .scale = { 0.5f, 0.5f, 0.5f }, .color = RED });
                    auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position{ r.position }, .gravity = 0 });
                    em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::STATIC });
                }
            }

            // Eliminamos la entidad
            em.destroyEntity(ent.getID());
        }

        // Esto sirve pa que el entity manager no se raye si dos entidades se destruyen a la vez
        if (lif.decreaseLifeNextFrame)
        {
            lif.decreaseLife();
            lif.decreaseLifeNextFrame = false;
        }
    });
}