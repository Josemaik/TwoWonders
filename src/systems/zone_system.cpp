#include "zone_system.hpp"

void ZoneSystem::update(EntityManager& em, ENGI::GameEngine& engine, Ia_man& iam) {

    updateZoneEnemies(em);

    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& ent, ZoneComponent& zon)
    {
        if (zon.changeZone) {
            // Comprobar en que zona estamos
            auto& li = em.getSingleton<LevelInfo>();
            if (li.num_zone != zon.zone) {
                //Crear enemigos de la zona nueva
                // if(!iam.checkEnemiesCreaeted(zon.zone)){
                // }
                //borro enemigos si cambio de zona
                deleteZoneEnemies(em);
                iam.createEnemiesZone(em, zon.zone);
                // Es una zona
                li.num_zone = zon.zone;
                if (zon.zone <= 13)
                {
                    if (ent.hasComponent<RenderComponent>()) {
                        auto& r = em.getComponent<RenderComponent>(ent);
                        engine.setPositionCamera({ r.position.x(), 30.0f, r.position.z() + 12.0f });
                        engine.setTargetCamera({ r.position.x(), r.position.y() + 3.0f, r.position.z() });
                    }
                }
                // Es un TP
                else
                {
                    auto* playerEn = em.getEntityByID(li.playerID);
                    auto& p = em.getComponent<PhysicsComponent>(*playerEn);
                    switch (zon.zone)
                    {

                        // SWORD //

                    case 14: // TP a la cueva de la espada
                        p.position.setX(49.0f);
                        p.position.setZ(85.0f);
                        break;

                    case 15: // TP desde la cueva de la espada
                        p.position.setX(-5.0f);
                        p.position.setZ(-4.5f);
                        break;

                        // COINS //

                    case 16: // TP a la cueva de las monedas
                        p.position.setX(71.0f);
                        p.position.setZ(85.0f);
                        break;

                    case 17: // TP desde la cueva de las monedas
                        p.position.setX(3.0f);
                        p.position.setZ(-20.0f);
                        break;

                        // SHOP //

                    case 18: // TP a la cueva de la tienda
                        p.position.setX(93.0f);
                        p.position.setZ(85.0f);
                        break;

                    case 19: // TP desde la cueva de la tienda
                        p.position.setX(-17.0f);
                        p.position.setZ(-20.0f);
                        break;

                        // MAZMORRA //

                    case 20: // TP a la cueva de la mazmorra
                        p.position.setX(61.0f);
                        p.position.setZ(-65.0f);
                        break;

                    case 21: // TP desde la cueva de la mazmorra
                        p.position.setX(-50.0f);
                        p.position.setZ(-3.5f);
                        break;

                    default:
                        break;
                    }
                }
            }
            zon.changeZone = false;
        }
    });

    if (!dead_entities.empty())
    {
        em.destroyEntities(dead_entities);
        dead_entities.clear();
    }
}

void ZoneSystem::deleteZoneEnemies(EntityManager& em)
{
    auto const& li = em.getSingleton<LevelInfo>();

    for (auto& enemy : li.enemiesID)
        dead_entities.insert(enemy);
}

void ZoneSystem::updateZoneEnemies(EntityManager& em)
{
    using noCMPs = MP::TypeList<>;
    using enemyTag = MP::TypeList<EnemyTag>;

    auto& li = em.getSingleton<LevelInfo>();

    std::unordered_set<std::size_t> enemies;
    em.forEach<noCMPs, enemyTag>([&](Entity& ent)
    {
        enemies.insert(ent.getID());
    });

    if (enemies != li.enemiesID)
        li.enemiesID = std::move(enemies);

    if (li.num_zone == 12 && li.enemiesID.empty() && !keyCreated)
        createKey(em);
}

void ZoneSystem::createKey(EntityManager& em)
{
    auto& e{ em.newEntity() };

    em.addTag<ObjectTag>(e);

    auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = { 83.f, 0.f, -71.0f }, .scale = { 1.f, 0.3f, 0.3f }, .color = GOLD });
    auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position = { r.position }, .velocity = { .0f, .0f, .0f } });
    em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::STATIC });
    em.addComponent<ObjectComponent>(e, ObjectComponent{ .type = Object_type::Key, .inmortal = true });

    keyCreated = true;
}
