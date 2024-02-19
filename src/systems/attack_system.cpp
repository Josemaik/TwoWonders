#include "attack_system.hpp"

void AttackSystem::update(EntityManager& em, float deltaTime) {

    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& ent, AttackComponent& att)
    {
        if (att.createAttack)
            createAttack(em, ent, att, deltaTime);

        att.decreaseCountdown(deltaTime, att.elapsed);
    });
}

vec3d AttackSystem::getPosMeteorito(uint16_t fase, vec3d posplayer) {
    switch (fase)
    {
    case 3: return vec3d{ posplayer.x() - 1, posplayer.y() + 5, posplayer.z() };
          break;
    case 2: return vec3d{ posplayer.x() + 1, posplayer.y() + 5, posplayer.z() + 1 };
          break;
    case 1: return vec3d{ posplayer.x() + 1 ,posplayer.y() + 5, posplayer.z() - 1 };
          break;
    default: break;
    }
    return vec3d{};
}

void AttackSystem::createAttack(EntityManager& em, Entity& ent, AttackComponent& att, float dt) {
    // att.vel += vec3d{ 0, 0, -0.5f } *(att.vel == vec3d{ 0, 0, 0 });
    auto& phy = em.getComponent<PhysicsComponent>(ent);

    // Se pone la direccion en la que este mirando el player
    if (ent.hasTag<PlayerTag>() && ent.hasComponent<InputComponent>()) {
        auto& plfi = em.getSingleton<PlayerInfo>();
        if (plfi.mana <= MANA_CUT)
        {
            att.createAttack = false;
            return;
        }

        // Calculamos la velocidad basada en la orientación del jugador
        static const double ATTACK_SPEED = 1.5f;

        double velX = sin(phy.orientation) * ATTACK_SPEED;
        double velZ = cos(phy.orientation) * ATTACK_SPEED;

        // Asignamos la velocidad al ataque
        att.vel = { velX , 0 , velZ };

        // Comprobar si la vida de la entidad es la maxima y elegir que tipo de ataque usa
        if (att.type == AttackType::AttackPlayer)
        {
            auto& plfi = em.getSingleton<PlayerInfo>();

            if (plfi.currentSpell == Spells::None)
                att.type = AttackType::Melee;
            else
            {
                createSpellAttack(em, ent, att);
                att.createAttack = false;
                return;
            }

            // if (ent.hasComponent<TypeComponent>())
            //     att.type = em.getComponent<TypeComponent>(ent).type == ElementalType::Neutral ? AttackType::Melee : AttackType::Ranged;
            // else
            //     att.type = AttackType::Ranged;

            // if (ent.hasComponent<LifeComponent>() && em.getComponent<LifeComponent>(ent).vidaMax())
            //     att.type = AttackType::Ranged;
            // else
            //     att.type = AttackType::Melee;
        }
    }


    bool is_air_attack{ false };
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
            auto& plfi = em.getSingleton<PlayerInfo>();
            if (plfi.bombs > 0) {
                auto& e{ em.newEntity() };
                em.addComponent<RenderComponent>(e, RenderComponent{ .position = phy.position + att.vel * 2, .scale = { 1.0f, 1.0f, 1.0f }, .color = BLACK });
                em.addComponent<ObjectComponent>(e, ObjectComponent{ .type = ObjectType::BombExplode, .life_time = 2.0f });
                plfi.decreaseBomb();
            }
        }
        else
        {
            auto& e{ em.newEntity() };
            em.addComponent<RenderComponent>(e, RenderComponent{ .position = phy.position + att.vel * 2, .scale = { 1.0f, 1.0f, 1.0f }, .color = BLACK });
            em.addComponent<ObjectComponent>(e, ObjectComponent{ .type = ObjectType::BombExplode, .life_time = 2.0f });
        }
        break;
    }
    case AttackType::HealSpell:
    {
        auto& e{ em.newEntity() };
        em.addComponent<RenderComponent>(e, RenderComponent{ .position = phy.position + att.vel * 2, .scale = { 1.0f, 1.0f, 1.0f }, .color = GREEN });
        em.addComponent<ObjectComponent>(e, ObjectComponent{ .type = ObjectType::Heal_Spell, .life_time = 1.0f });
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
            auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = phy.position + att.vel * 2, .scale = { 18.0f, 0.1f, 18.0f }, .color = GREEN });
            auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position{ r.position }, .scale = r.scale, .gravity = 0.01 });
            em.addComponent<ObjectComponent>(e, ObjectComponent{ .type = ObjectType::AreaAttack, .life_time = 7.0f });
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
        auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = phy.position + att.vel * 2, .scale = { 3.0f, 0.1f, 3.0f }, .color = GREEN });
        auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position{ r.position }, .scale = r.scale, .gravity = 0.01 });
        em.addComponent<ObjectComponent>(e, ObjectComponent{ .type = ObjectType::Spiderweb, .life_time = 4.0f });
        ElementalType tipoElemental;
        if (ent.hasComponent<TypeComponent>())
            tipoElemental = em.getComponent<TypeComponent>(ent).type;
        else
            tipoElemental = ElementalType::Neutral;
        em.addComponent<TypeComponent>(e, TypeComponent{ .type = tipoElemental });
        em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::SPIDERWEB });
    }
                              break;
    case AttackType::AirAttack: {
        if (att.air_attack_fases == 0) { // si se acaban las fases, reseteo contador
            is_air_attack = false;
            att.air_attack_fases = 4;
            att.warning_created = false;

        }
        else {
            bool attk_available{ false };
            is_air_attack = true;
            switch (att.air_attack_fases)
            {
            case 4: { //primera fase : creo indicador de donde se lanzara attaque
                if (!att.warning_created) {
                    auto& e{ em.newEntity() };
                    auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = att.pos_respawn_air_attack, .scale = { 4.0f, 0.1f, 4.0f }, .color = ORANGE });
                    em.addComponent<ObjectComponent>(e, ObjectComponent{ .type = ObjectType::Meteorit, .life_time = 5.0f });
                    auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position{ r.position }, .scale = r.scale, .gravity = 0.01 });
                    em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::PLAYER });
                    att.warning_created = true;
                }
                if (att.elapsed_warning_airatk >= att.countdown_warning_airatk) {
                    att.elapsed_warning_airatk = 0;
                    attk_available = true;
                }
                att.decreaseCountdown(dt, att.elapsed_warning_airatk);
            }
                  break;
            case 3: case 2: case 1: { //creo 1 meteorito
                if (att.elapsed_air_attk >= att.countdown_air_attk) {
                    att.elapsed_air_attk = 0;
                    attk_available = true;
                    auto& e{ em.newEntity() };
                    auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = getPosMeteorito(att.air_attack_fases,att.pos_respawn_air_attack), .scale = { 1.0f, 1.0f, 1.0f }, .color = BROWN });
                    auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position{ r.position }, .scale = r.scale, .gravity = 0.01 });
                    em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::METEORITE });
                }
                att.decreaseCountdown(dt, att.elapsed_air_attk);
            }
            default:
                break;
            }
            if (attk_available) {
                att.air_attack_fases--;
            }
        }
    }
                              break;
    default:
        break;
    }
    //if air attack not running reset createattack
    if (!is_air_attack)
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
    auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = em.getComponent<PhysicsComponent>(ent).position + (isRanged ? vec3d{0, 0, 0} : att.vel * scale_to_respawn_attack), .scale = { isRanged ? 1.5 : 6.0, isRanged ? 1.5 : 3.0, isRanged ? 1.5 : 6.0 }, .color = BLACK });
    auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position{ r.position }, .velocity = isRanged ? att.vel : vec3d{0, 0, 0}, .scale = r.scale, .gravity = 0, .orientation = phy.orientation });
    em.addComponent<LifeComponent>(e, LifeComponent{ .life = 1 });
    em.addComponent<ProjectileComponent>(e, ProjectileComponent{ .range = static_cast<float>(isRanged ? ranged : 0.2) });
    em.addComponent<TypeComponent>(e, TypeComponent{ .type = tipoElemental });
    if (ent.hasTag<PlayerTag>())
        em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::ATK_PLAYER });

    else
    {
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

void AttackSystem::createSpellAttack(EntityManager& em, Entity& ent, AttackComponent& att)
{
    auto& plfi = em.getSingleton<PlayerInfo>();
    ElementalType tipoElemental;

    switch (plfi.currentSpell)
    {
    case Spells::Fire1:
        att.type = AttackType::Ranged;
        tipoElemental = ElementalType::Fire;
        break;
    case Spells::Ice1:
        att.type = AttackType::Ranged;
        tipoElemental = ElementalType::Ice;
        break;
    case Spells::Water1:
        att.type = AttackType::Ranged;
        tipoElemental = ElementalType::Water;
        break;
    case Spells::Fire2:
        att.type = AttackType::Ranged;
        tipoElemental = ElementalType::Fire;
        break;
    case Spells::Ice2:
        att.type = AttackType::Ranged;
        tipoElemental = ElementalType::Ice;
        break;
    case Spells::Water2:
        att.type = AttackType::Ranged;
        tipoElemental = ElementalType::Water;
        break;
    default:
        att.type = AttackType::Ranged;
        tipoElemental = ElementalType::Neutral;
        break;
    }
    // Crear la entidad ataque
    auto& e{ em.newEntity() };
    em.addTag<HitPlayerTag>(e);
    auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = em.getComponent<PhysicsComponent>(ent).position, .scale = { 1.5f, 1.5f, 1.5f }, .color = BLACK });
    auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position{ r.position }, .velocity = att.vel, .scale = r.scale, .gravity = 0 });
    em.addComponent<LifeComponent>(e, LifeComponent{ .life = 1 });
    em.addComponent<ProjectileComponent>(e, ProjectileComponent{ .range = 3.0f });
    em.addComponent<TypeComponent>(e, TypeComponent{ .type = tipoElemental });
    em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::ATK_PLAYER });

    for (auto& s : plfi.spells)
    {
        if (s.spell == plfi.currentSpell)
        {
            plfi.mana -= s.cost;

            if (plfi.mana < 0.0)
                plfi.mana = 0;
            break;
        }
    }
}
