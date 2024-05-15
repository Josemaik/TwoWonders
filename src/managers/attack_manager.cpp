#include "attack_manager.hpp"

void AttackManager::createAttackType(EntityManager& em, Entity& ent, AttackType type, vec3d vel)
{
    auto& phy = em.getComponent<PhysicsComponent>(ent);
    // Tipo de ataque
    switch (type)
    {
    case AttackType::MeleePlayer:
    {
        auto position = phy.position;
        auto& orientation = phy.orientation;
        position += vec3d{ std::sin(orientation) * 2.0, 0, std::cos(orientation) * 2.0 };
        createAttackRangedOrMelee(em, position, vel, orientation, type, 2, ElementalType::Neutral, BehaviorType::ATK_PLAYER);
        break;
    }
    case AttackType::WaterBombShot:
    {
        // Creamos el hechizo
        auto& e{ em.newEntity() };
        em.addTag<HitPlayerTag>(e);
        em.addTag<WaterBombTag>(e);
        em.getSingleton<SoundSystem>().sonido_h_pompa();

        setPlayerAtkVel(em, phy.position, vel, phy.orientation);

        auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = phy.position, .scale = { 2.f, 2.f, 2.f }, .color = D_BLACK });
        auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position{ r.position }, .velocity = vel, .scale = r.scale, .gravity = 0 });
        em.addComponent<LifeComponent>(e, LifeComponent{ .life = 1 });
        em.addComponent<ProjectileComponent>(e, ProjectileComponent{ .range = 0.07f });
        em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::ATK_PLAYER });
        em.addComponent<AttackComponent>(e, AttackComponent{ .atkType = type, .damage = 3, .type = ElementalType::Water });
        em.addComponent<ParticleMakerComponent>(e, ParticleMakerComponent{ .active = true, .effect = Effects::WATER, .maxParticles = 10, .spawnRate = 0.01f, .lifeTime = 0.5f, });
        break;
    }
    case AttackType::WaterBomb:
    {
        auto& e{ em.newEntity() };
        em.addTag<HitPlayerTag>(e);
        auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position{ phy.position }, .scale = { 10.0f, 0.1f, 10.0f }, .gravity = 0.001 });
        em.addComponent<AttackComponent>(e, AttackComponent{ .atkType = type, .damage = 1, .type = ElementalType::Water, .lifeTime = 0.5f });
        em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, p.scale, BehaviorType::ATK_PLAYER });
        em.addComponent<ParticleMakerComponent>(e, ParticleMakerComponent{ .active = true, .effect = Effects::WATERSPLASH, .maxParticles = 25, .spawnRate = 0.01f, .lifeTime = 0.3f, });
        break;
    }
    case AttackType::FireBallShot:
    {
        // Creamos el hechizo
        auto& e{ em.newEntity() };
        em.addTag<HitPlayerTag>(e);
        em.addTag<FireBallTag>(e);

        setPlayerAtkVel(em, phy.position, vel, phy.orientation);

        auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = phy.position, .scale = { 2.f, 2.f, 2.f }, .color = D_BLACK });
        auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position{ r.position }, .velocity = vel, .scale = r.scale, .gravity = 0 });
        em.addComponent<LifeComponent>(e, LifeComponent{ .life = 1 });
        em.addComponent<ProjectileComponent>(e, ProjectileComponent{ .range = 0.07f });
        em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::ATK_PLAYER });
        em.addComponent<AttackComponent>(e, AttackComponent{ .atkType = type, .damage = 3, .type = ElementalType::Fire });
        em.addComponent<ParticleMakerComponent>(e, ParticleMakerComponent{ .active = true, .effect = Effects::FIREBALL, .maxParticles = 10, .spawnRate = 0.01f, .lifeTime = 0.5f, });
        break;
    }
    case AttackType::FireBall:
    {
        auto& e{ em.newEntity() };
        em.addTag<HitPlayerTag>(e);
        auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position{ phy.position }, .scale = { 10.0f, 0.1f, 10.0f }, .gravity = 0.001 });
        em.addComponent<AttackComponent>(e, AttackComponent{ .atkType = type, .damage = 1, .type = ElementalType::Fire, .lifeTime = 0.5f });
        em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, p.scale, BehaviorType::ATK_PLAYER });
        em.addComponent<ParticleMakerComponent>(e, ParticleMakerComponent{ .active = true, .effect = Effects::FIRESPLASH, .maxParticles = 25, .spawnRate = 0.01f, .lifeTime = 0.3f, });
        break;
    }
    case AttackType::WaterDashArea:
    {
        auto& ori = phy.orientation;

        vec3d originalPos = phy.position;
        vec3d posCopy = phy.position;
        posCopy += { std::sin(ori) * 10.0, 0, std::cos(ori) * 10.0 };

        em.getSingleton<SoundSystem>().sonido_h_dash();

        // Sacamos la normal entre la posición original y la nueva
        auto normal = (posCopy - originalPos).normalize();
        if (!checkWallCollision(em, phy.position, normal))
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
            em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, p.scale, BehaviorType::ATK_PLAYER });
            em.addComponent<AttackComponent>(e, AttackComponent{ .atkType = type, .damage = 1, .type = ElementalType::Water });
            em.addComponent<ParticleMakerComponent>(e, ParticleMakerComponent{ .active = true, .effect = Effects::WATER, .maxParticles = 10, .spawnRate = 0.01f, .lifeTime = 0.1f, });
        }
        break;
    }
    case AttackType::IceShield:
    {
        auto& plfi = em.getSingleton<PlayerInfo>();
        if (plfi.armor < plfi.max_armor)
            plfi.armor = plfi.max_armor;
        em.getSingleton<SoundSystem>().sonido_h_escudo();
        break;
    }
    case AttackType::MeteoritePlayer:
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
            em.addComponent<AttackComponent>(e, AttackComponent{ .atkType = type, .damage = 4, .type = ElementalType::Fire });
            em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, p.scale, BehaviorType::ATK_PLAYER });
        }
        break;
    }
    case AttackType::IceShard:
    {
        // Creamos el hechizo - 3 proyectiles
        auto& ori = phy.orientation;
        vec3d vel{};
        setPlayerAtkVel(em, phy.position, vel, ori);
        createAttackMultipleShot(em, phy.position, vel, ori, type, 2, ElementalType::Ice, BehaviorType::ATK_PLAYER, 3);
        break;
    }
    case AttackType::HealSpellSetup:
    {
        auto& e{ em.newEntity() };
        em.addComponent<RenderComponent>(e, RenderComponent{ .position{ phy.position }, .scale{ 3.0f, 3.0f, 3.0f }, .color = D_GREEN });
        em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position{ phy.position }, .scale{ 3.0f, 3.0f, 3.0f }, .gravity = 0 });
        em.addComponent<AttackComponent>(e, AttackComponent{ .atkType = type });
        break;
    }
    case AttackType::HealSpell:
    {
        // Crear una entidad que quite vida
        auto& e{ em.newEntity() };
        em.addTag<HitPlayerTag>(e);
        auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = phy.position, .scale = { 9.0f, 3.0f, 9.0f }, .color = D_GREEN });
        auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position{ r.position }, .scale = r.scale, .gravity = 0 });
        em.addComponent<LifeComponent>(e, LifeComponent{ .life = 5, .countdown = 0.0f });
        em.addComponent<AttackComponent>(e, AttackComponent{ .atkType = type, .damage = 1, .type = ElementalType::Neutral });
        em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::ATK_ENEMY });
        break;
    }
    case AttackType::SpiderShot:
    {
        auto& playerPhy = em.getComponent<PhysicsComponent>(*em.getEntityByID(em.getSingleton<LevelInfo>().playerID));
        setAtkVel(phy.position, vel, phy.orientation, playerPhy.position, 0.5);

        ElementalType eleType = ElementalType::Neutral;
        if (ent.hasComponent<TypeComponent>())
            eleType = em.getComponent<TypeComponent>(ent).type;

        createAttackRangedOrMelee(em, phy.position, vel, phy.orientation, type, 0, eleType, BehaviorType::ATK_ENEMY);
        break;
    }
    case AttackType::Spiderweb:
    {
        ElementalType eleType = ElementalType::Neutral;
        if (ent.hasComponent<TypeComponent>())
            eleType = em.getComponent<TypeComponent>(ent).type;

        auto& e{ em.newEntity() };
        auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = phy.position, .scale = { 6.0f, 0.1f, 6.0f }, .color = D_GREEN });
        auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position{ r.position }, .scale = r.scale, .gravity = 0.01 });
        em.addComponent<AttackComponent>(e, AttackComponent{ .atkType = type, .damage = 0, .type = eleType, .lifeTime = 2.0f });
        em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::ATK_ENEMY });
        break;
    }
    case AttackType::AreaAttack:
    {
        ElementalType eleType = ElementalType::Fire;
        if (ent.hasComponent<TypeComponent>())
            eleType = em.getComponent<TypeComponent>(ent).type;

        auto position = phy.position;
        auto& orientation = phy.orientation;
        position += vec3d{ std::sin(orientation) * 2.0, 0, std::cos(orientation) * 2.0 };

        auto& e{ em.newEntity() };
        em.addTag<HitPlayerTag>(e);
        auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = position, .scale = { 18.0f, 0.5f, 18.0f }, .color = D_GREEN });
        auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position{ r.position }, .scale = r.scale, .gravity = 0.01 });
        em.addComponent<AttackComponent>(e, AttackComponent{ .atkType = AttackType::AreaAttack, .damage = 3, .type = eleType, .lifeTime = 3.5f });
        em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::ATK_ENEMY });
        break;
    }
    case AttackType::CrusherAttack:
    {
        vec3d position = { phy.position.x(), (phy.position.y() - phy.scale.y() / 2) + 1, phy.position.z() };

        ElementalType eleType = ElementalType::Neutral;
        if (ent.hasComponent<TypeComponent>())
            eleType = em.getComponent<TypeComponent>(ent).type;

        auto& e{ em.newEntity() };
        auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = position, .scale = { 20.0f, 0.1f, 20.0f }, .color = D_GREEN });
        auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position{ r.position }, .scale = r.scale, .gravity = 0.0 });
        em.addComponent<AttackComponent>(e, AttackComponent{ .atkType = AttackType::CrusherAttack, .damage = 2, .type = eleType, .lifeTime = 0.2f });
        em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::ATK_ENEMY });
        em.getSingleton<SoundSystem>().sonido_apisonadora_ataque();
        break;
    }
    case AttackType::AirAttack:
    {
        switch (airAttackFase)
        {
        case 0: { //primera fase : creo indicador de donde se lanzara attaque
            auto& playerPos = em.getComponent<PhysicsComponent>(*em.getEntityByID(em.getSingleton<LevelInfo>().playerID)).position;

            auto& e{ em.newEntity() };
            auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = playerPos, .scale = { 12.0f, 0.3f, 12.0f }, .color = D_ORANGE });
            em.addComponent<ObjectComponent>(e, ObjectComponent{ .type = ObjectType::WarningZone, .life_time = 2.5f });
            auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position{ r.position }, .gravity = 0.03 });
            em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::WARNINGZONE });

            if (airAttackWarningElapsed >= airAttackWarningLimit) {
                airAttackWarningElapsed = 0;
                airAttackFase += 1;
            }
            else
                airAttackWarningElapsed += timeStep;

            break;
        }
        case 1: case 2: case 4: { //creo 1 meteorito
            if (airAttackElapsed >= airAttackLimit) {
                airAttackElapsed = 0;

                auto& playerPos = em.getComponent<PhysicsComponent>(*em.getEntityByID(em.getSingleton<LevelInfo>().playerID)).position;

                auto& e{ em.newEntity() };
                auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = getPosMeteorito(airAttackFase, playerPos), .scale = { 9.0f, 9.0f, 9.0f }, .color = D_ORANGE_DARK });
                auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position{ r.position }, .gravity = 0.03 });
                em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::ATK_ENEMY });
            }
            else
                airAttackElapsed += timeStep;

            if (airAttackFase == 4)
                airAttackFase = 0;
            else
                airAttackFase += 1;

            break;
        }
        default:
            break;
        }
        break;
    }
    case AttackType::TripleShot:
    {
        auto& playerPhy = em.getComponent<PhysicsComponent>(*em.getEntityByID(em.getSingleton<LevelInfo>().playerID));
        setAtkVel(phy.position, vel, phy.orientation, playerPhy.position);

        createAttackMultipleShot(em, phy.position, vel, phy.orientation, type, 2, ElementalType::Neutral, BehaviorType::ATK_ENEMY, 3);
        break;
    }
    case AttackType::SnowmanBall:
    {
        auto& li = em.getSingleton<LevelInfo>();

        // Creamos la bola de nieve/magma
        auto& e{ em.newEntity() };
        em.addTag<HitPlayerTag>(e);

        auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = phy.position, .scale = { 2.f, 2.f, 2.f }, .color = D_BLACK });
        auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position{ r.position }, .velocity = vel, .scale = r.scale, .gravity = 0 });
        em.addComponent<LifeComponent>(e, LifeComponent{ .life = 1 });
        em.addComponent<ProjectileComponent>(e, ProjectileComponent{ .range = 0.07f });
        em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::ATK_ENEMY });

        ElementalType eleType = ElementalType::Ice;
        if (li.mapID == 2)
        {
            eleType = ElementalType::Fire;
            em.addTag<MagmaBallTag>(e);
            em.addComponent<ParticleMakerComponent>(e, ParticleMakerComponent{ .active = true, .effect = Effects::FIREBALL, .maxParticles = 10, .spawnRate = 0.01f, .lifeTime = 0.5f, });
        }
        em.addComponent<AttackComponent>(e, AttackComponent{ .atkType = type, .damage = 3, .type = eleType });

        break;
    }
    case AttackType::RangedEnemy:
    {
        auto& playerPhy = em.getComponent<PhysicsComponent>(*em.getEntityByID(em.getSingleton<LevelInfo>().playerID));
        setAtkVel(phy.position, vel, phy.orientation, playerPhy.position);

        ElementalType eleType = ElementalType::Neutral;
        if (ent.hasComponent<TypeComponent>())
            eleType = em.getComponent<TypeComponent>(ent).type;

        createAttackRangedOrMelee(em, phy.position, vel, phy.orientation, type, 2, eleType, BehaviorType::ATK_ENEMY);
        break;
    }
    case AttackType::MeleeEnemy:
    {
        ElementalType eleType = ElementalType::Neutral;
        if (ent.hasComponent<TypeComponent>())
            eleType = em.getComponent<TypeComponent>(ent).type;

        createAttackRangedOrMelee(em, phy.position, vec3d::zero(), phy.orientation, type, 2, eleType, BehaviorType::ATK_ENEMY);
        break;
    }
    case AttackType::GollemAttack:
    {
        if (ent.hasTag<GolemTag>())
            em.getSingleton<SoundSystem>().sonido_golem_ataque();

        ElementalType eleType = ElementalType::Neutral;
        if (ent.hasComponent<TypeComponent>())
            eleType = em.getComponent<TypeComponent>(ent).type;

        auto position = phy.position;
        auto& orientation = phy.orientation;
        position += vec3d{ std::sin(orientation) * 3.0, 0, std::cos(orientation) * 3.0 };
        createAttackRangedOrMelee(em, position, vec3d::zero(), phy.orientation, type, 2, eleType, BehaviorType::ATK_ENEMY);
        break;
    }
    default:
        break;
    }
}

vec3d AttackManager::getPosMeteorito(uint16_t fase, vec3d posplayer) {
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

void AttackManager::createAttackMultipleShot(EntityManager& em, vec3d pos, vec3d vel, double ori, AttackType atkType, uint16_t damage, ElementalType eleType, BehaviorType behavior, int numShots) {
    float rotationAngle{}, spread{ 0.5f }; // Ángulo de dispersión entre los disparos
    vec3d auxVel{ vel }, rotatedVel{};
    int i{ 0 };

    // Crear los disparos
    if (numShots % 2 != 0)
    {
        // Creamos disparo central
        createAttackRangedOrMelee(em, pos, vel, ori, atkType, damage, eleType, behavior);
        i = 1;
    }

    for (; i < numShots; i++) {
        // Calcular el ángulo de rotación
        rotationAngle = spread * (static_cast<float>(i) - static_cast<float>(numShots) / 2.0f);

        // Rotar el vector de velocidad
        rotatedVel.setX(auxVel.x() * std::cos(rotationAngle) - auxVel.z() * std::sin(rotationAngle));
        rotatedVel.setY(auxVel.y());
        rotatedVel.setZ(auxVel.x() * std::sin(rotationAngle) + auxVel.z() * std::cos(rotationAngle));

        // Crear el ataque
        vel = rotatedVel;
        createAttackRangedOrMelee(em, pos, vel, ori, atkType, damage, eleType, behavior);
    }
}

void AttackManager::createAttackRangedOrMelee(EntityManager& em, vec3d pos, vec3d vel, double ori, AttackType atkType, uint16_t damage, ElementalType type, BehaviorType behavior)
{
    auto& e{ em.newEntity() };
    em.addTag<HitPlayerTag>(e);
    auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = pos, .scale = vel == vec3d::zero() ? vec3d{ 6.0, 3.0, 6.0 } : vec3d{ 1.5, 1.5, 1.5 }, .color = D_BLACK });
    auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position{ r.position }, .velocity = vel, .scale = r.scale, .gravity = 0, .orientation = ori });
    em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, behavior });
    em.addComponent<TypeComponent>(e, TypeComponent{ .type = type });
    em.addComponent<AttackComponent>(e, AttackComponent{ .atkType = atkType, .damage = damage, .type = type });

    if (p.velocity != vec3d::zero())
        em.addComponent<ProjectileComponent>(e, ProjectileComponent{ .range = 0.2f });
}

void AttackManager::setPlayerAtkVel(EntityManager& em, vec3d& pos, vec3d& vel, double ori)
{
    auto& li = em.getSingleton<LevelInfo>();
    vec3d otherPos{};
    if (li.lockedEnemy != li.max)
    {
        auto& lockedEnemy = *em.getEntityByID(li.lockedEnemy);
        auto& lockedEnemyPos = em.getComponent<PhysicsComponent>(lockedEnemy).position;
        otherPos = lockedEnemyPos;
    }

    setAtkVel(pos, vel, ori, otherPos);
}

void AttackManager::setAtkVel(vec3d& pos, vec3d& vel, double ori, vec3d otherPos, double velY)
{
    if (otherPos != vec3d::zero())
    {
        vel = (otherPos - pos).normalize();
        vel.setY(vel.y() + velY);
    }
    else
        vel = { 1.5 * std::sin(ori), velY, 1.5 * std::cos(ori) };
}

void AttackManager::resolvePlayerDirection(PhysicsComponent& playerPhy, PhysicsComponent& enemyPhy, bool isEnemy)
{
    auto& pos = playerPhy.position;
    auto& otherPos = enemyPhy.position;
    auto& enemyVel = enemyPhy.velocity;
    vec3d dir = { pos.x() - otherPos.x(), 0, pos.z() - otherPos.z() };
    int multiplier = 7;

    dir.normalize();

    if (isEnemy)
    {
        // Si la dirección del enemigo y la del jugador tienen menos de 20 grados de diferencia, el jugador se le suman 45º en la dirección contraria
        vec3d dirEnemy = { otherPos.x() - enemyVel.x(), 0, otherPos.z() - enemyVel.z() };
        dirEnemy.normalize();

        // Calcular el ángulo entre las dos direcciones
        double dot = dir.x() * dirEnemy.x() + dir.z() * dirEnemy.z(); // producto punto
        double det = dir.x() * dirEnemy.z() - dir.z() * dirEnemy.x(); // determinante
        double angle = atan2(det, dot); // atan2(y, x) o atan2(sin, cos)

        // Convertir el ángulo a grados
        double angleDeg = angle * 180 / K_PI;

        // Si el ángulo es menor de 20 grados, ajustar la dirección del jugador
        if (std::abs(angleDeg) < 20)
        {
            // Rotar la dirección del jugador 45 grados en la dirección contraria
            double angleRad = -135 * K_PI / 180; // Convertir a radianes
            double cosAngle = cos(angleRad);
            double sinAngle = sin(angleRad);
            vec3d newDir = { dir.x() * cosAngle - dir.z() * sinAngle, 0.0, dir.x() * sinAngle + dir.z() * cosAngle };

            dir = newDir;
        }
        multiplier = 7;
    }
    else
        playerPhy.gravity = playerPhy.gravity / 1.5;

    playerPhy.velocity = dir * multiplier;
    playerPhy.stopped = true;
}

bool AttackManager::checkWallCollision(EntityManager& em, vec3d& pos, vec3d& normalDir)
{
    // Queremos saber si hay una pared en la dirección de la normal
    // Creamos un rayo que salga de la posición en la dirección de la normal
    RayCast ray{ pos, normalDir };

    // Buscamos todas las paredes cercanas a la posición
    using noCMPs = MP::TypeList<ColliderComponent>;
    using wallTag = MP::TypeList<>;
    bool collision{ false };
    vec3d colPoint{};

    auto& frti = em.getSingleton<FrustumInfo>();
    em.forEach<noCMPs, wallTag>([&](Entity& e, ColliderComponent& col)
    {
        if (!frti.inFrustum(e.getID()))
            return;
        if (col.behaviorType & BehaviorType::STATIC || col.behaviorType & BehaviorType::LAVA)
        {
            auto& bbox = col.bbox;
            if (e.hasTag<LavaTag>())
            {
                if (bbox.intersectsRay2(ray.origin, ray.direction, colPoint) && colPoint.distance(pos) < 23.0)
                {
                    auto& phy = em.getComponent<PhysicsComponent>(e);

                    // Pillamos el centro de la caja para asegurarnos que no se quede en la lava o atraviese paredes
                    auto centreDir = (vec3d{ phy.position.x(), pos.y(), phy.position.z() } - pos).normalize();
                    ray.direction = centreDir;

                    bbox.intersectsRay2(ray.origin, ray.direction, colPoint);
                    auto newPos = colPoint + centreDir * 2.0;

                    pos = newPos;

                    collision = true;
                    return;
                }
            }
            else if (bbox.intersectsRay(ray.origin, ray.direction, colPoint) && colPoint.distance(pos) < 10.0)
            {
                pos = colPoint - normalDir * 1.5;

                collision = true;
                return;
            }
        }
    });

    return collision;
}