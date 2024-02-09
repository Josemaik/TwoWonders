#include "zone_system.hpp"

void ZoneSystem::update(EntityManager& em, ENGI::GameEngine&, Ia_man& iam, Eventmanager& evm, MapManager& map) {
    auto& li = em.getSingleton<LevelInfo>();
    updateZoneEnemies(em);

    em.forEach<SYSCMPs, SYSTAGs>([&](Entity&, ZoneComponent& zon)
    {
        if (zon.changeZone) {
            // Comprobar en que zona estamos
            if (li.num_zone != zon.zone) {
                // crear una funcion que devuelva un Evento
                //Crear enemigos de la zona nueva
                // if(!iam.checkEnemiesCreaeted(zon.zone)){
                // }
                //lanzar evento
                // evm.scheduleEvent(Event{ EVENT_CODE_CHANGE_ZONE });
                //borro enemigos si cambio de zona
                deleteZoneEnemies(em);
                // iam.createEnemiesZone(em, zon.zone);
                // Es una zona
                li.num_zone = zon.zone;
                if (zon.zone <= 13)
                {
                    // Aquí pondremos cosas de las zonas que no sean de tp
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
                        map.reset(em, 2, iam);
                        li.transition = true;

                        p.position.setX(49.0);
                        p.position.setZ(85.0);
                        break;

                    case 15: // TP desde la cueva de la espada
                        map.reset(em, 0, iam);
                        li.transition = true;

                        p.position.setX(-5.0);
                        p.position.setZ(-4.5);
                        break;

                        // COINS //

                    case 16: // TP a la cueva de las monedas
                        map.reset(em, 2, iam);
                        li.transition = true;

                        p.position.setX(71.0);
                        p.position.setZ(85.0);
                        break;

                    case 17: // TP desde la cueva de las monedas
                        map.reset(em, 0, iam);
                        li.transition = true;

                        p.position.setX(3.0);
                        p.position.setZ(-20.0);
                        break;

                        // SHOP //

                    case 18: // TP a la cueva de la tienda
                        map.reset(em, 2, iam);
                        li.transition = true;

                        p.position.setX(93.0);
                        p.position.setY(0.0);
                        p.position.setZ(85.0);
                        break;

                    case 19: // TP desde la cueva de la tienda
                        map.reset(em, 0, iam);
                        li.transition = true;

                        p.position.setX(-17.0);
                        p.position.setY(2.0);
                        p.position.setZ(-20.0);
                        break;

                        // MAZMORRA //

                    case 20: // TP a la cueva de la mazmorra
                        li.cameraChange = true;

                        map.reset(em, 1, iam);
                        li.transition = true;

                        p.position.setX(61.0);
                        p.position.setZ(-65.0);
                        break;

                    case 21: // TP desde la cueva de la mazmorra
                        li.cameraChange = false;

                        map.reset(em, 0, iam);
                        li.transition = true;

                        p.position.setX(-50.0);
                        p.position.setZ(-3.5);
                        break;

                    default:
                        break;
                    }
                }
            }
            zon.changeZone = false;
        }
    });

    // Cosas que hacer en cada zona
    switch (li.num_zone)
    {
    case 1:
    {
        checkChests(em, evm, 1);
        break;
    }
    case 11:
    {
        auto& bb = em.getSingleton<BlackBoard_t>();
        if (bb.create_subdito) {
            iam.createSubdito(em, 3.0);
            bb.create_subdito = false;
        }
        break;
    }
    default:
        break;
    }
}

void ZoneSystem::deleteZoneEnemies(EntityManager&)
{
    // auto const& li = em.getSingleton<LevelInfo>();

    // for (auto& enemy : li.enemiesID)
    //     dead_entities.insert(enemy);

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

    bool slimesDead{ true };

    for (auto enemy : li.enemiesID)
    {
        auto ent = em.getEntityByID(enemy);

        if (ent->hasTag<SlimeTag>())
            slimesDead = false;
    }

    if ((li.num_zone == 12 || li.num_zone == 11) && slimesDead && !keyCreated)
        createKey(em);
}

void ZoneSystem::createKey(EntityManager& em)
{
    auto& e{ em.newEntity() };

    em.addTag<ObjectTag>(e);

    auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = { 83., 0., -71.0 }, .scale = { 1., 0.3, 0.3 }, .color = GOLD });
    auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position = { r.position }, .velocity = { .0, .0, .0 } });
    em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::STATIC });
    em.addComponent<ObjectComponent>(e, ObjectComponent{ .type = ObjectType::Key, .inmortal = true });

    keyCreated = true;
}

void ZoneSystem::checkChests(EntityManager& em, Eventmanager& evm, uint16_t zone)
{
    auto& li = em.getSingleton<LevelInfo>();
    using chestCMP = MP::TypeList<ChestComponent, PhysicsComponent>;
    using noTag = MP::TypeList<>;

    em.forEach<chestCMP, noTag>([&](Entity& e, ChestComponent& ch, PhysicsComponent& phy)
    {
        if (ch.zone == zone)
        {
            // Revisamos si el cofre ha sido abierto
            std::pair<uint8_t, uint8_t> pair{ li.mapID, ch.id };

            if (li.notLoadSet.find(pair) != li.notLoadSet.end())
                return;

            // Calcula la distancia entre la posición del jugador y la posición del cofre
            auto& playerPhy = em.getComponent<PhysicsComponent>(*em.getEntityByID(li.playerID));
            auto& playerPos = playerPhy.position;

            double distance = playerPos.distance(phy.position);

            // Si el cofre se encuentra a menos de 2 unidades de distancia del se muestra el mensaje de abrir cofre
            if (distance < 2.0 && !ch.isOpen && !ch.showButton)
            {
                ch.showButton = true;
                std::cout << "Chest" << std::endl;
            }
            else if (distance > 2.0 && ch.showButton)
            {
                ch.showButton = false;
            }

            auto& inpi = em.getSingleton<InputInfo>();

            if (inpi.interact && ch.showButton && !ch.isOpen)
            {
                ch.isOpen = true;
                ch.showButton = false;
                li.chestToOpen = e.getID();
                evm.scheduleEvent(Event{ EventCodes::OpenChest });
                li.notLoadSet.insert(pair);
            }

        }
    });
}

void ZoneSystem::reset()
{
    keyCreated = false;
}
