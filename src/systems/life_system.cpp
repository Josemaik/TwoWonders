#include "life_system.hpp"

void LifeSystem::update(EntityManager& em, float deltaTime) {
    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& ent, LifeComponent& lif)
    {
        lif.decreaseCountdown(deltaTime);

        if (lif.life == 0)
        {
            // Si es enemigo creamos un objeto
            if (ent.hasTag<EnemyTag>()) {
                createObject(em, em.getComponent<PhysicsComponent>(ent).position);
            }

            lif.markedForDeletion = true;
        }

        if (lif.markedForDeletion)
            dead_entities.insert(ent.getID());
    });

    if (!dead_entities.empty())
    {
        em.destroyEntities(dead_entities);
        dead_entities.clear();
    }
}

// Se podra crear objetos: vida, bomba, moneda o nada
void LifeSystem::createObject(EntityManager& em, vec3f pos) {
    int random_value = std::rand();
    if (random_value % 4 > 0) {
        Object_type tipo_nuevo_objeto;
        Color color_nuevo_objeto;
        if (random_value % 4 == 1)
        {
            tipo_nuevo_objeto = Object_type::Bomb;
            color_nuevo_objeto = GRAY;
        }
        else if (random_value % 4 == 2)
        {
            tipo_nuevo_objeto = Object_type::Coin;
            color_nuevo_objeto = YELLOW;
        }
        else if (random_value % 4 == 3)
        {
            tipo_nuevo_objeto = Object_type::Life;
            color_nuevo_objeto = RED;
        }

        // Se crea el nuevo objeto
        auto& e{ em.newEntity() };
        em.addTag<ObjectTag>(e);
        auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = pos, .scale = { 0.5f, 0.5f, 0.5f }, .color = color_nuevo_objeto });
        auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position{ r.position }, .gravity = 0 });
        em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::STATIC });
        em.addComponent<ObjectComponent>(e, ObjectComponent{ .type = tipo_nuevo_objeto });
    }
}