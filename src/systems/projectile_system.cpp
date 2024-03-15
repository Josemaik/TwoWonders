#include "projectile_system.hpp"

void ProjectileSystem::update(EntityManager& em, float deltaTime) {

    auto& li = em.getSingleton<LevelInfo>();
    auto& plfi = em.getSingleton<PlayerInfo>();

    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, ProjectileComponent& pro)
    {
        if (pro.checkRange(deltaTime)) {

            if (pro.startedFalling)
            {
                li.insertDeath(e.getID());
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

        if (plfi.currentSpell == Spells::WaterBomb || plfi.currentSpell == Spells::FireBall || plfi.currentSpell == Spells::FireMeteorites)
        {
            // Vemos si el hechizo está en la lista de destrucción
            auto& dead_ents = li.getDeath();
            if (dead_ents.find(e.getID()) != dead_ents.end())
            {
                auto& col = em.getComponent<ColliderComponent>(e);
                if (col.behaviorType & BehaviorType::ATK_PLAYER)
                {
                    auto& phy = em.getComponent<PhysicsComponent>(e);
                    if (phy.position.y() > -20.)
                    {
                        if (plfi.currentSpell == Spells::FireBall || plfi.currentSpell == Spells::FireMeteorites)
                            em.addComponent<AttackComponent>(e, AttackComponent{ .type = AttackType::FireBall, .damage = plfi.currentSpell.damage, .createAttack = true });
                        else
                            em.addComponent<AttackComponent>(e, AttackComponent{ .type = AttackType::WaterBomb, .damage = plfi.currentSpell.damage, .createAttack = true });
                    }
                }
            }
        }

    });
}
