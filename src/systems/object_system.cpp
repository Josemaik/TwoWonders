#include "object_system.hpp"

void ObjectSystem::update(EntityManager& em) {
    auto& li = em.getSingleton<LevelInfo>();

    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& ent, ObjectComponent& obj)
    {
        if (obj.decreaseLifeTime(timeStep) && (!obj.inmortal))
        {
            li.insertDeath(ent.getID());
            if (ent.hasComponent<RenderComponent>())
                em.getComponent<RenderComponent>(ent).visible = false;

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
                if (!playerEnt->hasComponent<AttackerComponent>())
                    em.addComponent<AttackerComponent>(*playerEnt, AttackerComponent{});
                break;

            case ObjectType::Mana_Potion:
                plfi.mana += 40;
                em.getSingleton<SoundSystem>().recoger_mana();
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
            case ObjectType::Key:
            {
                plfi.addKey();
                em.getSingleton<SoundSystem>().sonido_llave();
                Item key = { "Llave", "Una llave, parece que solo puede abrir una puerta" };
                plfi.addItem(std::make_unique<Item>(key));
                break;
            }
            case ObjectType::Basic_Staff:
            {
                Staff staff("Bastón Básico", "É un bastón", ElementalType::Neutral, 1.0);
                plfi.addItem(std::make_unique<Staff>(staff));
                if (!playerEnt->hasComponent<AttackerComponent>())
                    em.addComponent<AttackerComponent>(*playerEnt, AttackerComponent{});
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
            em.addTag<LifeDropTag>(e);
            color = D_RED;
            break;
        }
        case ObjectType::Coin:
        {
            em.addTag<CoinTag>(e);
            color = D_YELLOW;
            break;
        }
        case ObjectType::Bomb:
        {
            color = D_GRAY;
            break;
        }
        case ObjectType::Mana_Potion:
        {
            em.addTag<ManaDropTag>(e);
            color = D_BLUE_LIGHT;
            break;
        }
        case ObjectType::Basic_Staff:
        {
            color = D_GRAY;
            scl = { 10.5, 10.3, 10.3 };
            inmortal = true;
            visible = false;

            auto& plfi = em.getSingleton<PlayerInfo>();
            plfi.hasStaff = true;
            break;
        }
        case ObjectType::Key:
        {
            color = D_YELLOW_LIGHT;
            scl = { 10.5, 10.3, 10.3 };
            inmortal = true;
            visible = false;
            break;
        }
        case ObjectType::ShopItem_ExtraLife:
        {
            color = D_RED;
            scl = { 10.5, 10.5, 10.5 };
            inmortal = true;
            visible = false;
            break;
        }
        case ObjectType::Coin30:
        {
            color = D_RED;
            scl = { 10.5, 10.5, 10.5 };
            inmortal = true;
            visible = false;
            break;
        }
        case ObjectType::GoodBoots:
        {
            color = D_GREEN;
            scl = { 10.5, 10.5, 10.5 };
            inmortal = true;
            visible = false;

            auto& plfi = em.getSingleton<PlayerInfo>();
            plfi.hasBoots = true;
            em.getSingleton<SoundSystem>().sonido_equipar_botas();
            em.getSingleton<SoundSystem>().sonido_boost_pasos();

            Item boots = { "Botas de la suerte", "Botas que te hacen correr más rápido" };
            break;
        }
        case ObjectType::RegularHat:
        {
            color = D_GREEN;
            scl = { 10.5, 10.5, 10.5 };
            inmortal = true;
            visible = false;

            auto& plfi = em.getSingleton<PlayerInfo>();
            plfi.hasHat = true;

            em.getSingleton<SoundSystem>().sonido_equipar_gorro();

            Item boots = { "Sombrero de Mago", "Te ayuda a canalizar mejor tu uso de magia" };
            break;
        }
        case ObjectType::AttackUpgrade:
        {
            color = D_RED;
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
