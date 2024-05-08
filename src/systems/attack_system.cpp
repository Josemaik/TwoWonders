#include "attack_system.hpp"

void AttackSystem::update(EntityManager& em) {

    auto& frti = em.getSingleton<FrustumInfo>();
    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, AttackComponent& att)
    {
        if (!frti.inFrustum(e.getID()))
            return;

        if (att.createAttack)
            createAttack(em, e, att);

        att.decreaseCountdown(timeStep, att.elapsed);
    });
}

vec3d AttackSystem::getPosMeteorito(uint16_t fase, vec3d posplayer) {
    switch (fase)
    {
    case 3: return vec3d{ posplayer.x() - 5, posplayer.y() + 20, posplayer.z() };
          break;
    case 2: return vec3d{ posplayer.x() + 5, posplayer.y() + 20, posplayer.z() + 5 };
          break;
    case 1: return vec3d{ posplayer.x() + 5 ,posplayer.y() + 20, posplayer.z() - 5 };
          break;
    default: break;
    }
    return vec3d{};
}

void AttackSystem::createAttack(EntityManager& em, Entity& ent, AttackComponent& att) {
    // att.vel += vec3d{ 0, 0, -0.5f } *(att.vel == vec3d{ 0, 0, 0 });


    // Se pone la direccion en la que este mirando el player
    if (ent.hasTag<PlayerTag>() && ent.hasComponent<InputComponent>()) {
        auto& plfi = em.getSingleton<PlayerInfo>();
        auto& phy = em.getComponent<PhysicsComponent>(ent);
        if (plfi.mana <= MANA_CUT)
        {
            att.createAttack = false;
            return;
        }

        // Calculamos la velocidad basada en la orientación del jugador
        static const double ATTACK_SPEED = 1.5f;

        double velX = std::sin(phy.orientation) * ATTACK_SPEED;
        double velZ = std::cos(phy.orientation) * ATTACK_SPEED;

        // Asignamos la velocidad al ataque
        att.vel = { velX , 0 , velZ };

        // Comprobar si la vida de la entidad es la maxima y elegir que tipo de ataque usa
        if (att.type == AttackType::AttackPlayer)
        {
            auto& plfi = em.getSingleton<PlayerInfo>();
            auto& inpi = em.getSingleton<InputInfo>();

            if (inpi.spell1)
                plfi.currentSpell = plfi.spells[0];
            else if (inpi.spell2)
                plfi.currentSpell = plfi.spells[1];
            else if (inpi.spell3)
                plfi.currentSpell = plfi.spells[2];
            else if (inpi.melee)
                plfi.currentSpell = plfi.noSpell;

            if (plfi.currentSpell == Spells::None)
            {
                att.type = AttackType::Melee;
                em.getSingleton<SoundSystem>().sonido_melee();
            }
            else
            {
                createSpellAttack(em, ent, att);
                att.createAttack = false;
                return;
            }

            inpi.setAttackFalse();
        }
    }

    createAttackType(em, ent, att);
}

void AttackSystem::createAttackType(EntityManager& em, Entity& ent, AttackComponent& att)
{
    auto& phy = em.getComponent<PhysicsComponent>(ent);
    bool is_air_attack{ false };
    // Tipo de ataque
    switch (att.type)
    {
    case AttackType::Ranged:
        if (ent.hasTag<SpiderTag>()) {
            createAttackRangedOrMelee(em, ent, att, true, att.scale_to_respawn_attack, 0.5);
        }
        else
            createAttackRangedOrMelee(em, ent, att, true, att.scale_to_respawn_attack, 1.5);
        break;

    case AttackType::Melee:
        createAttackRangedOrMelee(em, ent, att, false, att.scale_to_respawn_attack, 1.5);
        break;

    case AttackType::Bomb:
    {
        auto& li = em.getSingleton<LevelInfo>();
        if (li.playerID == ent.getID())
        {
            auto& plfi = em.getSingleton<PlayerInfo>();
            if (plfi.bombs > 0) {
                auto& e{ em.newEntity() };
                em.addComponent<RenderComponent>(e, RenderComponent{ .position = phy.position + att.vel * 2, .scale = { 1.0f, 1.0f, 1.0f }, .color = D_BLACK });
                em.addComponent<ObjectComponent>(e, ObjectComponent{ .type = ObjectType::BombExplode, .life_time = 1.0f });
                plfi.decreaseBomb();
            }
        }
        else
        {
            auto& e{ em.newEntity() };
            em.addComponent<RenderComponent>(e, RenderComponent{ .position = phy.position + att.vel * 2, .scale = { 1.0f, 1.0f, 1.0f }, .color = D_BLACK });
            em.addComponent<ObjectComponent>(e, ObjectComponent{ .type = ObjectType::BombExplode, .life_time = 1.0f });
        }
        break;
    }
    case AttackType::HealSpell:
    {
        auto& e{ em.newEntity() };
        em.addComponent<RenderComponent>(e, RenderComponent{ .position = phy.position + att.vel * 2, .scale = { 1.0f, 1.0f, 1.0f }, .color = D_GREEN });
        em.addComponent<ObjectComponent>(e, ObjectComponent{ .type = ObjectType::Heal_Spell, .life_time = 0.5f });
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
            auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = phy.position + att.vel * 2, .scale = { 18.0f, 0.5f, 18.0f }, .color = D_GREEN });
            auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position{ r.position }, .scale = r.scale, .gravity = 0.01 });
            em.addComponent<ObjectComponent>(e, ObjectComponent{ .type = ObjectType::AreaAttack, .life_time = 3.5f });
            ElementalType tipoElemental;
            if (ent.hasComponent<TypeComponent>())
                tipoElemental = em.getComponent<TypeComponent>(ent).type;
            else
                tipoElemental = ElementalType::Fire;
            em.addComponent<TypeComponent>(e, TypeComponent{ .type = tipoElemental });
            //em.addComponent<ProjectileComponent>(e, ProjectileComponent{ .range = 0.2f });
            em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::AREADAMAGE });
        }
        break;
    }
    case AttackType::Spiderweb: {
        //createAttackRangedOrMelee(em, ent, att, true, att.scale_to_respawn_attack,1.0);
        auto& e{ em.newEntity() };
        auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = phy.position + att.vel * 2, .scale = { 6.0f, 0.1f, 6.0f }, .color = D_GREEN });
        auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position{ r.position }, .scale = r.scale, .gravity = 0.01 });
        em.addComponent<ObjectComponent>(e, ObjectComponent{ .type = ObjectType::Spiderweb, .life_time = 2.0f });
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
                    auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = att.pos_respawn_air_attack, .scale = { 4.0f, 0.1f, 4.0f }, .color = D_ORANGE });
                    em.addComponent<ObjectComponent>(e, ObjectComponent{ .type = ObjectType::Meteorit, .life_time = 2.5f });
                    auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position{ r.position }, .gravity = 0.01 });
                    em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::WARNINGZONE });

                    att.warning_created = true;
                }
                if (att.elapsed_warning_airatk >= att.countdown_warning_airatk) {
                    att.elapsed_warning_airatk = 0;
                    attk_available = true;
                }
                att.decreaseCountdown(timeStep, att.elapsed_warning_airatk);
            }
                  break;
            case 3: case 2: case 1: { //creo 1 meteorito
                if (att.elapsed_air_attk >= att.countdown_air_attk) {
                    att.elapsed_air_attk = 0;
                    attk_available = true;
                    auto& e{ em.newEntity() };
                    auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = getPosMeteorito(att.air_attack_fases, att.pos_respawn_air_attack), .scale = { 3.0f, 3.0f, 3.0f }, .color = D_ORANGE_DARK });
                    auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position{ r.position }, .gravity = 0.01 });

                    em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::METEORITE });
                }
                att.decreaseCountdown(timeStep, att.elapsed_air_attk);
            }
            default:
                break;
            }
            if (attk_available) {
                att.air_attack_fases--;
            }
        }
        break;
    }
    case AttackType::CrusherAttack:
    {
        auto& e{ em.newEntity() };
        auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = vec3d{att.pos_respawn_crush_attack.x(),
        att.pos_respawn_crush_attack.y(),att.pos_respawn_crush_attack.z()}, .scale = { 20.0f, 0.1f, 20.0f }, .color = D_GREEN });
        auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position{ r.position }, .scale = r.scale, .gravity = 0.0 });
        em.addComponent<ObjectComponent>(e, ObjectComponent{ .type = ObjectType::None, .life_time = 0.2f });
        ElementalType tipoElemental;
        if (ent.hasComponent<TypeComponent>())
            tipoElemental = em.getComponent<TypeComponent>(ent).type;
        else
            tipoElemental = ElementalType::Neutral;
        em.addComponent<TypeComponent>(e, TypeComponent{ .type = tipoElemental });
        em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::AREADAMAGECRUSHER });
        em.getSingleton<SoundSystem>().sonido_apisonadora_ataque();
        break;
    }
    case AttackType::WaterBomb:
    {
        auto& e{ em.newEntity() };
        // auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = phy.position, .scale = { 10.0f, 0.1f, 10.0f }, .color = SKYBLUE });
        auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position{ phy.position }, .scale = { 10.0f, 0.1f, 10.0f }, .gravity = 0.001 });
        em.addComponent<ObjectComponent>(e, ObjectComponent{ .type = ObjectType::None, .life_time = 0.4f });
        em.addComponent<TypeComponent>(e, TypeComponent{ .type = ElementalType::Water });
        em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, p.scale, BehaviorType::ATK_PLAYER });
        em.addComponent<ParticleMakerComponent>(e, ParticleMakerComponent{ .active = true, .effect = Effects::WATERSPLASH, .maxParticles = 25, .spawnRate = 0.01f, .lifeTime = 0.3f, });
        break;
    }
    case AttackType::FireBall:
    {
        auto& e{ em.newEntity() };
        // auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = phy.position, .scale = { 10.0f, 0.1f, 10.0f }, .color = BLUE });
        auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position{ phy.position }, .scale = { 10.0f, 0.1f, 10.0f }, .gravity = 0.01 });
        em.addComponent<ObjectComponent>(e, ObjectComponent{ .type = ObjectType::None, .life_time = 1.0f });
        em.addComponent<TypeComponent>(e, TypeComponent{ .type = ElementalType::Fire });
        em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, p.scale, BehaviorType::ATK_PLAYER });
        em.addComponent<ParticleMakerComponent>(e, ParticleMakerComponent{ .active = true, .effect = Effects::FIRESPLASH, .maxParticles = 25, .spawnRate = 0.01f, .lifeTime = 0.3f, });
        break;
    }
    case AttackType::FireBallShot:
    {
        // Creamos el hechizo
        auto& e{ em.newEntity() };
        em.addTag<HitPlayerTag>(e);

        BehaviorType type = BehaviorType::ATK_ENEMY;
        if (ent.hasTag<PlayerTag>())
        {
            em.addTag<FireBallTag>(e);
            setPlayerAtkVel(em, ent, att);
            type = BehaviorType::ATK_PLAYER;
            em.getSingleton<SoundSystem>().sonido_h_bola_fuego();

        }
        else{
            em.addTag<MagmaBallTag>(e);
            em.getSingleton<SoundSystem>().sonido_munyeco_ataque();

        }

        auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = em.getComponent<PhysicsComponent>(ent).position, .scale = { 1.5f, 1.5f, 1.5f }, .color = D_BLACK });
        auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position{ r.position }, .velocity = att.vel, .scale = r.scale, .gravity = 0 });
        em.addComponent<LifeComponent>(e, LifeComponent{ .life = 1 });
        em.addComponent<ProjectileComponent>(e, ProjectileComponent{ .range = 0.07f });
        em.addComponent<TypeComponent>(e, TypeComponent{ .type = ElementalType::Fire });
        em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, type });
        em.addComponent<ParticleMakerComponent>(e, ParticleMakerComponent{ .active = true, .effect = Effects::FIREBALL, .maxParticles = 20, .spawnRate = 0.01f, .lifeTime = 0.2f, });
        break;

    }
    case AttackType::WaterBombShot:
    {
        // Creamos el hechizo
        auto& e{ em.newEntity() };
        em.addTag<HitPlayerTag>(e);
        em.addTag<WaterBombTag>(e);

        BehaviorType type = BehaviorType::ATK_ENEMY;
        if (ent.hasTag<PlayerTag>())
        {
            setPlayerAtkVel(em, ent, att);
            type = BehaviorType::ATK_PLAYER;
        }

        auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = em.getComponent<PhysicsComponent>(ent).position, .scale = { 1.5f, 1.5f, 1.5f }, .color = D_BLACK });
        auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position{ r.position }, .velocity = att.vel, .scale = r.scale, .gravity = 0 });
        em.addComponent<LifeComponent>(e, LifeComponent{ .life = 1 });
        em.addComponent<ProjectileComponent>(e, ProjectileComponent{ .range = 0.07f });
        em.addComponent<TypeComponent>(e, TypeComponent{ .type = ElementalType::Water });
        em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, type });
        em.addComponent<ParticleMakerComponent>(e, ParticleMakerComponent{ .active = true, .effect = Effects::WATER, .maxParticles = 10, .spawnRate = 0.01f, .lifeTime = 0.5f, });
        break;
    }
    default:
        break;
    }
    //if air attack not running reset createattack
    if (!is_air_attack)
        att.createAttack = false;
}

void AttackSystem::createAttackMultipleShot(EntityManager& em, Entity& ent, AttackComponent& att, int numShots) {
    float spread = 0.5f; // Ángulo de dispersión entre los disparos
    vec3d vel = att.vel;
    int i = 0;

    // Crear los disparos
    if (numShots % 2 != 0)
    {
        // Creamos disparo central
        createAttackRangedOrMelee(em, ent, att, true, att.scale_to_respawn_attack, 0.5);
        i = 1;
    }

    for (; i < numShots; i++) {
        // Calcular el ángulo de rotación
        float rotationAngle = spread * (static_cast<float>(i) - static_cast<float>(numShots) / 2.0f);

        // Rotar el vector de velocidad
        vec3d rotatedVel{};
        rotatedVel.setX(vel.x() * std::cos(rotationAngle) - vel.z() * std::sin(rotationAngle));
        rotatedVel.setY(vel.y());
        rotatedVel.setZ(vel.x() * std::sin(rotationAngle) + vel.z() * std::cos(rotationAngle));

        // Crear el ataque
        att.vel = rotatedVel;
        createAttackRangedOrMelee(em, ent, att, true, att.scale_to_respawn_attack, 0.5);
    }
}
void AttackSystem::createAttackRangedOrMelee(EntityManager& em, Entity& ent, AttackComponent& att, bool isRanged, double const scale_to_respawn_attack, double const ranged) {
    auto const& phy = em.getComponent<PhysicsComponent>(ent);

    // Crear la entidad ataque
    auto& e{ em.newEntity() };

    // Comprobar el tipo del ataque
    ElementalType tipoElemental;
    if (ent.hasComponent<TypeComponent>())
        tipoElemental = em.getComponent<TypeComponent>(ent).type;
    else
        tipoElemental = ElementalType::Neutral;

    if (ent.hasTag<PlayerTag>())
    {
        auto& plfi = em.getSingleton<PlayerInfo>();
        tipoElemental = plfi.currentSpell == Spells::None ? ElementalType::Neutral : plfi.currentSpell.type;
    }
    else if (ent.hasTag<SnowmanTag>())
        em.addTag<SnowBallTag>(e);

    em.addTag<HitPlayerTag>(e);
    auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = em.getComponent<PhysicsComponent>(ent).position + (isRanged ? vec3d{0, 0, 0} : att.vel * scale_to_respawn_attack), .scale = { isRanged ? 1.5 : 6.0, isRanged ? 1.5 : 3.0, isRanged ? 1.5 : 6.0 }, .color = D_BLACK });
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
        if (ent.hasTag<SpiderTag>() && isRanged) {
            em.addComponent<AttackComponent>(e);
            c.attackType = AttackType::Spiderweb;
        }
    }
}

void AttackSystem::createSpellAttack(EntityManager& em, Entity& ent, AttackComponent& att)
{
    auto& plfi = em.getSingleton<PlayerInfo>();
    auto& spell = plfi.currentSpell;
    ElementalType eleType = ElementalType::Neutral;

    switch (spell.spell)
    {
    case Spells::FireBall:
        att.type = AttackType::FireBallShot;
        eleType = ElementalType::Fire;
        break;
    case Spells::IceShards:
    {
        att.type = AttackType::Ranged;
        // Creamos el hechizo - 3 proyectiles
        createAttackMultipleShot(em, ent, att, 3);
        break;
    }
    case Spells::WaterBomb:
    {
        att.type = AttackType::WaterBombShot;
        eleType = ElementalType::Water;
        em.getSingleton<SoundSystem>().sonido_h_pompa();
        break;
    }
    case Spells::FireMeteorites:
    {
        // Creamos 3 meteoritos en el aire encima del enemigo fijado
        auto pos = em.getComponent<PhysicsComponent>(ent).position;
        auto& li = em.getSingleton<LevelInfo>();

        if (li.lockedEnemy != li.max)
            pos = em.getComponent<PhysicsComponent>(*em.getEntityByID(li.lockedEnemy)).position;

        for (uint16_t i = 1; i < 4; i++)
        {
            auto& e{ em.newEntity() };
            em.addTag<HitPlayerTag>(e);
            auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = getPosMeteorito(i, pos), .scale = { 4.0f, 4.0f, 4.0f }, .color = D_ORANGE_DARK });
            auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position{ r.position }, .scale = r.scale, .gravity = 0.02 });
            em.addComponent<ProjectileComponent>(e, ProjectileComponent{ .range = 0.5f });
            em.addComponent<TypeComponent>(e, TypeComponent{ .type = ElementalType::Fire });
            em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, p.scale, BehaviorType::ATK_PLAYER });
        }
        break;
    }
    case Spells::IceShield:
    {
        if (plfi.armor < plfi.max_armor)
            plfi.armor = plfi.max_armor;
        else
            return;
        break;
    }
    case Spells::WaterDash:
    {
        // El jugador hace un dash hacia adelante
        auto& phy = em.getComponent<PhysicsComponent>(ent);
        auto& ori = phy.orientation;

        vec3d originalPos = phy.position;
        vec3d posCopy = phy.position;
        posCopy += { std::sin(ori) * 10.0, 0, std::cos(ori) * 10.0 };

        em.getSingleton<SoundSystem>().sonido_h_dash();

        // Sacamos la normal entre la posición original y la nueva
        auto normal = (posCopy - originalPos).normalize();
        if (!col_sys->checkWallCollision(em, phy.position, normal))
        {
            phy.position = posCopy;
        }
        else
            posCopy = phy.position;

        // Creamos una capa de agua entre la posición original y la nueva
        auto distance = (originalPos - posCopy).length();
        for (double i = 0; i < distance; i += 2)
        {
            auto pos = originalPos + (posCopy - originalPos) * (i / distance);
            auto& e{ em.newEntity() };
            auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position{ pos }, .scale = { 2.0f, 0.1f, 2.0f }, .gravity = 0.05 });
            em.addComponent<ObjectComponent>(e, ObjectComponent{ .type = ObjectType::None, .life_time = 0.5f });
            em.addComponent<TypeComponent>(e, TypeComponent{ .type = ElementalType::Water });
            em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, p.scale, BehaviorType::ATK_PLAYER });
            em.addComponent<ParticleMakerComponent>(e, ParticleMakerComponent{ .active = true, .effect = Effects::WATER, .maxParticles = 10, .spawnRate = 0.01f, .lifeTime = 0.1f, });
        }
        break;
    }
    default:
        att.type = AttackType::Ranged;
        break;
    }

    if (eleType != ElementalType::Neutral)
    {
        createAttackType(em, ent, att);
    }

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

    auto& inpi = em.getSingleton<InputInfo>();
    inpi.setAttackFalse();
}

void AttackSystem::setCollisionSystem(CollisionSystem* col)
{
    col_sys = col;
}

void AttackSystem::setPlayerAtkVel(EntityManager& em, Entity& e, AttackComponent& att)
{
    auto& li = em.getSingleton<LevelInfo>();
    if (li.lockedEnemy != li.max)
    {
        auto& lockedEnemy = *em.getEntityByID(li.lockedEnemy);
        auto& lockedEnemyPos = em.getComponent<PhysicsComponent>(lockedEnemy).position;
        att.vel = (lockedEnemyPos - em.getComponent<PhysicsComponent>(e).position).normalize();
        att.vel.setY(att.vel.y() + 0.7);
    }
    else
        att.vel.setY(0.7);
}
