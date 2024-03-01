#include "projectile_system.hpp"

void ProjectileSystem::update(EntityManager& em, float deltaTime) {

    auto& li = em.getSingleton<LevelInfo>();
    auto& plfi = em.getSingleton<PlayerInfo>();

    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, ProjectileComponent& pro)
    {
        if (pro.checkRange(deltaTime)) {

            if (pro.startedFalling)
            {
                li.dead_entities.insert(e.getID());
                return;
            }

            if (e.hasComponent<ColliderComponent>() && e.hasComponent<LifeComponent>()) {
                if (em.getComponent<ColliderComponent>(e).attackType == AttackType::Spiderweb) {
                    em.getComponent<LifeComponent>(e).life = 0;
                }
            }

            if (e.hasComponent<PhysicsComponent>())
            {
                auto& phy = em.getComponent<PhysicsComponent>(e);
                phy.gravity = .1f;
                pro.startedFalling = true;
                pro.range = 2.0f;
                pro.elapsed = 0.0f;
            }
        }

        if (plfi.currentSpell == Spells::Water1)
        {
            // Vemos si el hechizo está en la lista de destrucción
            if (li.dead_entities.find(e.getID()) != li.dead_entities.end())
            {
                auto& phy = em.getComponent<PhysicsComponent>(e);
                if (phy.position.y() > -20.)
                    em.addComponent<AttackComponent>(e, AttackComponent{ .type = AttackType::WaterBomb, .damage = 2, .createAttack = true });

            }
        }

    });
}
