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
    att.vel += vec3d{ 0, 0, -0.5f } *(att.vel == vec3d{ 0, 0, 0 });

    // Se pone la direccion en la que este mirando el player
    if (ent.hasTag<PlayerTag>() && ent.hasComponent<InputComponent>()) {
        auto& phy = em.getComponent<PhysicsComponent>(ent);

        static const double ATTACK_SPEED = 0.5f;

        // Calculamos la velocidad basada en la orientación del jugador
        double velX = sin(phy.orientation) * ATTACK_SPEED;
        double velZ = cos(phy.orientation) * ATTACK_SPEED;

        // Asignamos la velocidad al ataque
        att.vel = { velX , 0 , velZ };
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
        if (ent.hasTag<SpiderTag>()) {
            createAttackRangedOrMelee(em, ent, att, true, att.scale_to_respawn_attack, 1.0);
        }
        else
            createAttackRangedOrMelee(em, ent, att, true, att.scale_to_respawn_attack, 3.0);
        break;

    case AttackType::Melee:
        createAttackRangedOrMelee(em, ent, att, false, att.scale_to_respawn_attack, 3.0);
        break;

    case AttackType::Bomb:
    {
        auto& li = em.getSingleton<LevelInfo>();
        if (li.playerID == ent.getID())
        {
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
        }
        else
        {
            auto& e{ em.newEntity() };
            em.addComponent<RenderComponent>(e, RenderComponent{ .position = em.getComponent<PhysicsComponent>(ent).position + att.vel * 2, .scale = { 1.0f, 1.0f, 1.0f }, .color = BLACK });
            em.addComponent<ObjectComponent>(e, ObjectComponent{ .type = Object_type::BombExplode, .life_time = 2.0f });
        }
        break;
    }
    case AttackType::HealSpell:
    {
        auto& e{ em.newEntity() };
        em.addComponent<RenderComponent>(e, RenderComponent{ .position = em.getComponent<PhysicsComponent>(ent).position + att.vel * 2, .scale = { 1.0f, 1.0f, 1.0f }, .color = GREEN });
        em.addComponent<ObjectComponent>(e, ObjectComponent{ .type = Object_type::Heal_Spell, .life_time = 1.0f });
        break;
    }
    case AttackType::AttackPlayer:
        break;
    case AttackType::TripleShot: createAttackMultipleShot(em, ent, att, 3);
        break;
    case AttackType::AreaAttack: {
        // auto& li = em.getSingleton<LevelInfo>();
        if (ent.hasTag<GolemTag>()) {
            auto& e{ em.newEntity() };
            em.addTag<HitPlayerTag>(e);
            auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = em.getComponent<PhysicsComponent>(ent).position + att.vel * 2, .scale = { 4.0f, 0.1f, 4.0f }, .color = GREEN });
            auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position{ r.position }, .gravity = 0.01 });
            em.addComponent<ObjectComponent>(e, ObjectComponent{ .type = Object_type::AreaAttack, .life_time = 7.0f });
            ElementalType tipoElemental;
            if (ent.hasComponent<TypeComponent>())
                tipoElemental = em.getComponent<TypeComponent>(ent).type;
            else
                tipoElemental = ElementalType::Neutral;
            em.addComponent<TypeComponent>(e, TypeComponent{ .type = tipoElemental });
            //em.addComponent<ProjectileComponent>(e, ProjectileComponent{ .range = 0.2f });
            em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::AREADAMAGE });
        }
    }
                               break;
    case AttackType::Spiderweb: {
        //createAttackRangedOrMelee(em, ent, att, true, att.scale_to_respawn_attack,1.0);
        auto& e{ em.newEntity() };
        auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = em.getComponent<PhysicsComponent>(ent).position + att.vel * 2, .scale = { 3.0f, 0.1f, 3.0f }, .color = GREEN });
        auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position{ r.position }, .gravity = 0.01 });
        em.addComponent<ObjectComponent>(e, ObjectComponent{ .type = Object_type::Spiderweb, .life_time = 4.0f });
        ElementalType tipoElemental;
        if (ent.hasComponent<TypeComponent>())
            tipoElemental = em.getComponent<TypeComponent>(ent).type;
        else
            tipoElemental = ElementalType::Neutral;
        em.addComponent<TypeComponent>(e, TypeComponent{ .type = tipoElemental });
        em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::SPIDERWEB });
    }
                              break;
    default:
        break;
    }

    att.createAttack = false;
}

void AttackSystem::createAttackMultipleShot(EntityManager& em, Entity& ent, AttackComponent& att, int numShots) {
    float spread = 0.15f; // Ángulo de dispersión entre los disparos
    vec3d vel = att.vel;

    // Disparo hacia el jugador
    createAttackRangedOrMelee(em, ent, att, true, att.scale_to_respawn_attack, 3.0);

    for (int i = 1; i <= numShots; ++i) {
        float offset = spread * (static_cast<float>(i) - 0.5f - static_cast<float>(numShots) / 2.f);

        // Restaura la velocidad original
        att.vel = vel;

        // Aplica el offset en la dirección z
        att.vel = { att.vel.x(), att.vel.y(), att.vel.z() + offset };

        // Crea el disparo
        createAttackRangedOrMelee(em, ent, att, true, att.scale_to_respawn_attack, 3.0);
    }
}

void AttackSystem::createAttackRangedOrMelee(EntityManager& em, Entity& ent, AttackComponent& att, bool isRanged, double const scale_to_respawn_attack, double const ranged) {
    //std::cout << "CREO LA BALA";
    auto const& phy = em.getComponent<PhysicsComponent>(ent);

    // Comprobar el tipo del ataque
    ElementalType tipoElemental;
    if (ent.hasComponent<TypeComponent>())
        tipoElemental = em.getComponent<TypeComponent>(ent).type;
    else
        tipoElemental = ElementalType::Neutral;

    // Crear la entidad ataque
    auto& e{ em.newEntity() };
    em.addTag<HitPlayerTag>(e);
    auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = em.getComponent<PhysicsComponent>(ent).position + (isRanged ? vec3d{0, 0, 0} : att.vel * scale_to_respawn_attack), .scale = { isRanged ? 0.5 : 2.0, isRanged ? 0.5 : 1.0, isRanged ? 0.5 : 2.0 }, .color = BLACK });
    auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position{ r.position }, .velocity = isRanged ? att.vel : vec3d{0, 0, 0}, .gravity = 0, .orientation = phy.orientation });
    em.addComponent<LifeComponent>(e, LifeComponent{ .life = 1 });
    em.addComponent<ProjectileComponent>(e, ProjectileComponent{ .range = static_cast<float>(isRanged ? ranged : 0.2) });
    em.addComponent<TypeComponent>(e, TypeComponent{ .type = tipoElemental });
    if (ent.hasTag<PlayerTag>())
        em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::ATK_PLAYER });
    else {
        auto& c = em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::ATK_ENEMY });
        if (ent.hasTag<GolemTag>()) {
            c.attackType = AttackType::GollemAttack;
        }
        if (ent.hasTag<SpiderTag>()) {
            em.addComponent<AttackComponent>(e);
            c.attackType = AttackType::Spiderweb;
        }
    }
}
