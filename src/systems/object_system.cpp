#include "object_system.hpp"

void ObjectSystem::update(EntityManager& em) {
    auto& li = em.getSingleton<LevelInfo>();

    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& ent, ObjectComponent& obj)
    {
        if (obj.decreaseLifeTime(timeStep) && (!obj.inmortal))
        {
            if (obj.type == ObjectType::BombExplode || obj.type == ObjectType::Heal_Spell)
                obj.effect();
            else
            {
                li.insertDeath(ent.getID());
                if (ent.hasComponent<RenderComponent>())
                    em.getComponent<RenderComponent>(ent).visible = false;
            }
        }

        // Recuperamos la entidad del player
        auto* playerEnt = em.getEntityByID(li.playerID);
        bool shop_object = true;

        // Si existe el player se aplica el efecto del objeto
        if (playerEnt && obj.active) {
            auto& plfi = em.getSingleton<PlayerInfo>();

            switch (obj.type)
            {
            case ObjectType::Life:
                if (playerEnt->hasComponent<LifeComponent>())
                {
                    em.getComponent<LifeComponent>(*playerEnt).increaseLife();
                    em.getSingleton<SoundSystem>().sonido_recoger_vida();
                }
                break;

            case ObjectType::Sword:
                if (!playerEnt->hasComponent<AttackComponent>())
                    em.addComponent<AttackComponent>(*playerEnt, AttackComponent{});
                break;

            case ObjectType::Mana_Potion:
                plfi.mana += 40;
                em.getSingleton<SoundSystem>().sonido_config();
                break;

            case ObjectType::Bomb:
                plfi.addBomb();
                break;

            case ObjectType::Coin:
                plfi.addCoin(5);
                em.getSingleton<SoundSystem>().sonido_destello();
                break;

            case ObjectType::Coin30:
                plfi.addCoin(30);
                em.getSingleton<SoundSystem>().sonido_destello();
                break;

            case ObjectType::ShopItem_Bomb:
                shop_object = buyBomb(em);
                break;

            case ObjectType::ShopItem_Life:
                shop_object = buyLife(em, playerEnt);
                break;

            case ObjectType::ShopItem_ExtraLife:
            {
                if (playerEnt->hasComponent<LifeComponent>())
                {
                    auto& life = em.getComponent<LifeComponent>(*playerEnt);
                    life.increaseMaxLife();
                    life.increaseLife(4);
                    // FIXME: Crashea el juego
                    em.getSingleton<SoundSystem>().sonido_aum_vida_max();
                }
                break;
            }
            case ObjectType::BombExplode:
                explodeBomb(em, ent);
                break;
            case ObjectType::Heal_Spell:
                explodeBombHeal(em, ent);
                break;
            case ObjectType::Key:
            {
                plfi.addKey();
                em.getSingleton<SoundSystem>().sonido_llave();
                Item key = { "Llave", "Una llave, parece que solo puede abrir una puerta" };
                plfi.addItem(std::make_unique<Item>(key));


                break;
            }
            case ObjectType::Fire_Spell:
            {
                Spell fire_pell("Bola de fuego", "Una bola de destrucción masiva", Spells::FireBall, 15.0, 1.0);

                plfi.addSpell(fire_pell);
                // auto& type = em.getComponent<TypeComponent>(*playerEnt);
                // if (!type.hasType(ElementalType::Fire))
                //     type.addType(ElementalType::Fire);
                break;
            }
            case ObjectType::Basic_Staff:
            {
                Staff staff("Bastón Básico", "É un bastón", ElementalType::Neutral, 1.0);
                plfi.addItem(std::make_unique<Staff>(staff));
                if (!playerEnt->hasComponent<AttackComponent>())
                    em.addComponent<AttackComponent>(*playerEnt, AttackComponent{});
                break;
            }
            default:
                break;
            }
            if (shop_object)
                li.insertDeath(ent.getID());
            else
                obj.active = false;

            if (obj.objID != 255)
                li.dontLoad.insert(std::make_pair(li.mapID, obj.objID));
        }
    });

    if (!toCreate.empty())
        createObjects(em);
}

// ent->hasComponent<LifeComponent<()

bool ObjectSystem::buyBomb(EntityManager& em) {
    auto& plfi = em.getSingleton<PlayerInfo>();

    return plfi.buyBomb();
}

bool ObjectSystem::buyLife(EntityManager& em, Entity* ent) {
    auto& plfi = em.getSingleton<PlayerInfo>();

    if (ent->hasComponent<LifeComponent>()) {
        if (plfi.decreaseCoins(10)) {
            auto& life = em.getComponent<LifeComponent>(*ent);
            life.increaseLife(4);
            return true;
        }
    }
    return false;
}

bool ObjectSystem::buyExtraLife(EntityManager& em, Entity* ent) {
    auto& plfi = em.getSingleton<PlayerInfo>();

    if (ent->hasComponent<LifeComponent>()) {
        if (plfi.decreaseCoins(30)) {
            em.getComponent<LifeComponent>(*ent).increaseMaxLife();
            em.getComponent<LifeComponent>(*ent).increaseLife(4);
            return true;
        }
    }
    return false;
}

void ObjectSystem::explodeBombHeal(EntityManager& em, Entity& ent) {
    createExplodeBomb(em, ent, BehaviorType::HEAL, GREEN);
    createExplodeBomb(em, ent, BehaviorType::ATK_ENEMY, GREEN);
}

void ObjectSystem::explodeBomb(EntityManager& em, Entity& ent) {
    createExplodeBomb(em, ent, BehaviorType::ATK_PLAYER, BLACK);
    createExplodeBomb(em, ent, BehaviorType::ATK_ENEMY, BLACK);
}

void ObjectSystem::createExplodeBomb(EntityManager& em, Entity& ent, BehaviorType type, Color color) {
    if (ent.hasComponent<RenderComponent>()) {
        auto& ren = em.getComponent<RenderComponent>(ent);
        // Crear una entidad que quite vida
        auto& e{ em.newEntity() };
        em.addTag<HitPlayerTag>(e);
        auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = ren.position, .scale = { 3.0f, 1.0f, 3.0f }, .color = color });
        auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position{ r.position }, .scale = r.scale, .gravity = 0 });
        em.addComponent<LifeComponent>(e, LifeComponent{ .life = 5, .countdown = 0.0f });
        em.addComponent<ProjectileComponent>(e, ProjectileComponent{ .range = 0.2f });
        em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, type });
    }
}

void ObjectSystem::addObject(ObjectType type, vec3d pos)
{
    toCreate.push_back(std::make_pair(type, pos));
}

void ObjectSystem::createObjects(EntityManager& em)
{
    // Se crean los objetos del vector
    for (auto& [obj, pos] : toCreate)
    {
        // Se crea el nuevo objeto
        auto& e{ em.newEntity() };
        Color color{};
        vec3d scl{ 1.5, 1.5, 1.5 };
        bool inmortal = false;
        bool visible = true;

        switch (obj)
        {
        case ObjectType::Life:
        {
            color = RED;
            break;
        }
        case ObjectType::Coin:
        {
            em.addTag<CoinTag>(e);
            color = YELLOW;
            break;
        }
        case ObjectType::Bomb:
        {
            color = GRAY;
            break;
        }
        case ObjectType::Mana_Potion:
        {
            color = SKYBLUE;
            break;
        }
        case ObjectType::Basic_Staff:
        {
            color = GRAY;
            scl = { 10.5, 10.3, 10.3 };
            inmortal = true;
            visible = false;

            auto& plfi = em.getSingleton<PlayerInfo>();
            plfi.hasStaff = true;
            break;
        }
        case ObjectType::Key:
        {
            color = GOLD;
            scl = { 10.5, 10.3, 10.3 };
            inmortal = true;
            visible = false;
            break;
        }
        case ObjectType::ShopItem_ExtraLife:
        {
            color = RED;
            scl = { 10.5, 10.5, 10.5 };
            inmortal = true;
            visible = false;
            break;
        }
        case ObjectType::Coin30:
        {
            color = RED;
            scl = { 10.5, 10.5, 10.5 };
            inmortal = true;
            visible = false;
            break;
        }
        case ObjectType::Fire_Spell:
        {
            color = RED;
            scl = { 1.5, 0.3, 0.3 };
            inmortal = true;
            break;
        }
        case ObjectType::GoodBoots:
        {
            color = GREEN;
            scl = { 10.5, 10.5, 10.5 };
            inmortal = true;
            visible = false;

            auto& plfi = em.getSingleton<PlayerInfo>();
            plfi.hasBoots = true;

            Item boots = { "Botas de la suerte", "Botas que te hacen correr más rápido" };
            break;
        }
        case ObjectType::RegularHat:
        {
            color = GREEN;
            scl = { 10.5, 10.5, 10.5 };
            inmortal = true;
            visible = false;

            auto& plfi = em.getSingleton<PlayerInfo>();
            plfi.hasHat = true;

            Item boots = { "Sombrero de Mago", "Te ayuda a canalizar mejor tu uso de magia" };
            break;
        }
        case ObjectType::AttackUpgrade:
        {
            color = RED;
            scl = { 10.5, 10.5, 10.5 };
            inmortal = true;
            visible = false;

            auto& plfi = em.getSingleton<PlayerInfo>();
            plfi.attackUpgrade = true;
            break;
        }

        default:
            break;
        }

        em.addTag<ObjectTag>(e);
        auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = pos, .scale = scl, .color = color, .visible = visible });
        auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position{ r.position }, .scale = r.scale });
        em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::OBJECT });
        em.addComponent<ObjectComponent>(e, ObjectComponent{ .type = obj, .inmortal = inmortal });
        if (visible)
            em.addComponent<ParticleMakerComponent>(e, ParticleMakerComponent{ .active = true, .effect = Effects::OBJECT, .maxParticles = 6, .spawnRate = 0.1f, .lifeTime = 0.4f });
    }

    // Limpiamos el vector
    toCreate.clear();
}
