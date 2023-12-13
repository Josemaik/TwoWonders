#include "attack_system.hpp"

void AttackSystem::update(EntityManager& em, float deltaTime) {

    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& ent, AttackComponent& att)
    {
        if (att.createAttack)
            createAttack(em, ent, att);

        att.decreaseCountdown(deltaTime);
    });
}

void AttackSystem::createAttack(EntityManager& em, Entity& ent, AttackComponent& att) {
    att.vel != vec3f{ 0, 0, 0 } ? att.vel : att.vel = { 0, 0, -0.5f };

    // Se pone la direccion en la que este mirando el player
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

    // Comprobar si la vida de la entidad es la maxima y elegir que tipo de ataque usa
    if (att.type == AttackType::AttackPlayer) {
        if (ent.hasComponent<LifeComponent>() && em.getComponent<LifeComponent>(ent).vidaMax())
            att.type = AttackType::Ranged;
        else
            att.type = AttackType::Melee;
    }


    // Tipo de ataque
    switch (att.type)
    {
    case AttackType::Ranged:
        createAttackRanged(em, ent, att);
        break;

    case AttackType::Melee:
        createAttackMelee(em, ent, att);
        break;

    case AttackType::Bomb:
        if (ent.hasComponent<InformationComponent>())
        {
            auto& inf = em.getComponent<InformationComponent>(ent);
            if (inf.bombs > 0) {
                auto& e{ em.newEntity() };
                em.addComponent<RenderComponent>(e, RenderComponent{ .position = em.getComponent<PhysicsComponent>(ent).position + att.vel * 2, .scale = { 1.0f, 1.0f, 1.0f }, .color = BLACK });
                em.addComponent<ObjectComponent>(e, ObjectComponent{ .type = Object_type::BombExplode, .life_time = 2.0f });
                inf.decreaseBomb();
            }
        }
        break;

    case AttackType::AttackPlayer:
        break;

    default:
        break;
    }

    att.createAttack = false;
}

void AttackSystem::createAttackRanged(EntityManager& em, Entity& ent, AttackComponent& att) {
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
}

void AttackSystem::createAttackMelee(EntityManager& em, Entity& ent, AttackComponent& att) {
    auto& e{ em.newEntity() };
    em.addTag<HitPlayer>(e);
    auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = em.getComponent<PhysicsComponent>(ent).position + att.vel * 2, .scale = { 1.0f, 1.0f, 1.0f }, .color = BLACK });
    auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position{ r.position }, .gravity = 0 });
    em.addComponent<LifeComponent>(e, LifeComponent{ .life = 1 });
    em.addComponent<ProjectileComponent>(e, ProjectileComponent{ .range = 0.2f });
    if (ent.hasTag<PlayerTag>())
        em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::ATK_PLAYER });
    else
        em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::ATK_ENEMY });
}