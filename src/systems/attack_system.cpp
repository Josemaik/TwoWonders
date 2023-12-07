#include "attack_system.hpp"

void AttackSystem::update(EntityManager& em, float deltaTime) {

    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& ent, AttackComponent& att)
    {
        if (att.createAttack) {
            att.vel != vec3f{ 0, 0, 0 } ? att.vel : att.vel = { 0, 0, -0.5f };
            if (ent.hasTag<PlayerTag>() && ent.hasComponent<InputComponent>()) {
                auto& i = em.getComponent<InputComponent>(ent);
                if (i.last_key == i.down)
                    att.vel = { 0 , 0 , 0.5f };
                else if (i.last_key == i.up)
                    att.vel = { 0 , 0 , -0.5f };
                else if (i.last_key == i.right)
                    att.vel = { 0.5f , 0 , 0 };
                else if (i.last_key == i.left)
                    att.vel = { -0.5f , 0 , 0 };
            }
            auto& e{ em.newEntity() };
            em.addTag<HitPlayer>(e);
            auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = em.getComponent<PhysicsComponent>(ent).position, .scale = { 0.5f, 0.5f, 0.5f }, .color = BLACK });
            auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position{ r.position }, .velocity = att.vel, .gravity = 0 });
            em.addComponent<LifeComponent>(e, LifeComponent{ .life = 1 });
            em.addComponent<ProjectileComponent>(e, ProjectileComponent{});
            if (ent.hasTag<PlayerTag>())
                em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::ATK_PLAYER });
            else
                em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::ATK_ENEMY });

            att.createAttack = false;
        }

        att.decreaseCountdown(deltaTime);
    });
}