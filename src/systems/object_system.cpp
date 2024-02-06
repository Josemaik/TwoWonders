#include "object_system.hpp"

void ObjectSystem::update(EntityManager& em, float deltaTime) {
    auto& li = em.getSingleton<LevelInfo>();

    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& ent, ObjectComponent& obj)
    {
        if (obj.decreaseLifeTime(deltaTime) && (!obj.inmortal))
        {
            if (obj.type == Object_type::BombExplode || obj.type == Object_type::Heal_Spell)
                obj.effect();
            else
                li.dead_entities.insert(ent.getID());

        }

        // Recuperamos la entidad del player
        auto& li = em.getSingleton<LevelInfo>();
        auto* playerEnt = em.getEntityByID(li.playerID);
        bool shop_object = true;

        // Si existe el player se aplica el efecto del objeto
        if (playerEnt && obj.active) {
            auto& plfi = em.getSingleton<PlayerInfo>();

            switch (obj.type)
            {
            case Object_type::Life:
                if (playerEnt->hasComponent<LifeComponent>())
                    em.getComponent<LifeComponent>(*playerEnt).increaseLife();
                break;

            case Object_type::Sword:
                if (!playerEnt->hasComponent<AttackComponent>())
                    em.addComponent<AttackComponent>(*playerEnt, AttackComponent{});
                break;

            case Object_type::Bomb:
                plfi.addBomb();
                break;

            case Object_type::Coin:
                plfi.addCoin();
                break;

            case Object_type::Coin30:
                plfi.add30Coins();
                break;

            case Object_type::ShopItem_Bomb:
                shop_object = buyBomb(em);
                break;

            case Object_type::ShopItem_Life:
                shop_object = buyLife(em, playerEnt);
                break;

            case Object_type::ShopItem_ExtraLife:
                shop_object = buyExtraLife(em, playerEnt);
                break;

            case Object_type::BombExplode:
                explodeBomb(em, ent);
                break;
            case Object_type::Heal_Spell:
                explodeBombHeal(em, ent);
                break;
            case Object_type::Key:
                plfi.addKey();
                break;

            default:
                break;
            }
            if (shop_object)
                li.dead_entities.insert(ent.getID());
            else
                obj.active = false;

            if (obj.mapID != 255 && obj.objID != 255)
                li.notLoadSet.insert(std::make_pair(obj.mapID, obj.objID));
        }
    });
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
            em.getComponent<LifeComponent>(*ent).increaseLife();
            em.getComponent<LifeComponent>(*ent).increaseLife();
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
            em.getComponent<LifeComponent>(*ent).increaseLife();
            em.getComponent<LifeComponent>(*ent).increaseLife();
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
        auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position{ r.position }, .gravity = 0 });
        em.addComponent<LifeComponent>(e, LifeComponent{ .life = 5, .countdown = 0.0f });
        em.addComponent<ProjectileComponent>(e, ProjectileComponent{ .range = 0.2f });
        em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, type });
    }
}
