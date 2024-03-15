#include "zone_system.hpp"

void ZoneSystem::update(EntityManager& em, ENGI::GameEngine&, Ia_man& iam, EventManager& evm, MapManager& map, const float&) {
    auto& li = em.getSingleton<LevelInfo>();

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
                // iam.createEnemiesZone(em, zon.zone);
                // Es una zona
                auto* playerEn = em.getEntityByID(li.playerID);
                auto& p = em.getComponent<PhysicsComponent>(*playerEn);

                li.num_zone = zon.zone;
                switch (zon.zone)
                {

                    // SWORD //

                case 12: // TP a la cueva de la espada
                    switch (li.mapID)
                    {
                    case 0:
                    {
                        map.reset(em, 1, iam);
                        li.transition = true;

                        p.position = { 35.0, 22.0, -23.0 };
                        break;
                    }

                    }
                    break;

                    // case 15: // TP desde la cueva de la espada
                    //     map.reset(em, 0, iam);
                    //     li.transition = true;

                    //     p.position.setX(-5.0);
                    //     p.position.setZ(-4.5);
                    //     break;

                    //     // COINS //

                    // case 16: // TP a la cueva de las monedas
                    //     map.reset(em, 2, iam);
                    //     li.transition = true;

                    //     p.position.setX(71.0);
                    //     p.position.setZ(85.0);
                    //     break;

                    // case 17: // TP desde la cueva de las monedas
                    //     map.reset(em, 0, iam);
                    //     li.transition = true;

                    //     p.position.setX(3.0);
                    //     p.position.setZ(-20.0);
                    //     break;

                    //     // SHOP //

                    // case 18: // TP a la cueva de la tienda
                    //     map.reset(em, 2, iam);
                    //     li.transition = true;

                    //     p.position.setX(93.0);
                    //     p.position.setY(0.0);
                    //     p.position.setZ(85.0);
                    //     break;

                    // case 19: // TP desde la cueva de la tienda
                    //     map.reset(em, 0, iam);
                    //     li.transition = true;

                    //     p.position.setX(-17.0);
                    //     p.position.setY(2.0);
                    //     p.position.setZ(-20.0);
                    //     break;

                    //     // MAZMORRA //

                    // case 20: // TP a la cueva de la mazmorra
                    //     li.cameraChange = true;

                    //     map.reset(em, 1, iam);
                    //     li.transition = true;

                    //     p.position.setX(61.0);
                    //     p.position.setZ(-65.0);
                    //     break;

                    // case 21: // TP desde la cueva de la mazmorra
                    //     li.cameraChange = false;

                    //     map.reset(em, 0, iam);
                    //     li.transition = true;

                    //     p.position.setX(-50.0);
                    //     p.position.setZ(-3.5);
                    //     break;

                default:
                    break;
                }
            }

            zon.changeZone = false;
        }
    });

    // Cosas que hacer en cada zona
    if (li.mapID == 0)
        switch (li.num_zone)
        {
        case 4:
        {
            checkTutorialEnemies(em);
            break;
        }
        case 5:
        {
            checkTutorialEnemies(em);
            break;
        }
        case 6:
        {
            checkTutorialEnemies(em);
            break;
        }
        case 7:
        {
            checkTutorialEnemies(em);
            break;
        }
        default:
            break;
        }

    auto& zchi = em.getSingleton<ZoneCheckInfo>();

    checkZones(em, evm, zchi.getChestZones(), [&](EntityManager& em, EventManager& evm) { checkChests(em, evm); });
    checkZones(em, evm, zchi.getLeverZones(), [&](EntityManager& em, EventManager& evm) { checkLevers(em, evm); });
    checkZones(em, evm, zchi.getDoorZones(), [&](EntityManager& em, EventManager& evm) { checkDoors(em, evm); });
}

void ZoneSystem::checkZones(EntityManager& em, EventManager& evm, checkType zones, checkFuncType checkFunction)
{
    auto& li = em.getSingleton<LevelInfo>();
    for (auto zone : zones)
    {
        if (li.num_zone == zone)
            checkFunction(em, evm);
    }
}

void ZoneSystem::checkChests(EntityManager& em, EventManager& evm)
{
    auto& li = em.getSingleton<LevelInfo>();
    if (li.playerDetected)
        return;
    using chestCMP = MP::TypeList<ChestComponent, InteractiveComponent, PhysicsComponent, OneUseComponent>;
    using chestTag = MP::TypeList<ChestTag>;

    em.forEach<chestCMP, chestTag>([&](Entity& e, ChestComponent& ch, InteractiveComponent& ic, PhysicsComponent& phy, OneUseComponent& ouc)
    {
        if (ouc.zone == li.num_zone)
        {
            // Revisamos si el cofre ha sido abierto
            std::pair<uint8_t, uint8_t> pair{ li.mapID, ouc.id };

            if (li.dontLoad.find(pair) != li.dontLoad.end())
                return;

            // Calcula la distancia entre la posición del jugador y la posición del cofre
            auto& playerPhy = em.getComponent<PhysicsComponent>(*em.getEntityByID(li.playerID));
            auto& playerPos = playerPhy.position;

            double distance = playerPos.distance(phy.position);
            double range = 7.5;

            // Si el cofre se encuentra a menos de 2 unidades de distancia del se muestra el mensaje de abrir cofre
            if (distance < range && !ch.isOpen && !ic.showButton)
                ic.showButton = true;

            else if (distance > range && ic.showButton)
                ic.showButton = false;

            auto& inpi = em.getSingleton<InputInfo>();

            if (inpi.interact && ic.showButton && !ch.isOpen)
            {
                ch.isOpen = true;
                ic.showButton = false;
                li.chestToOpen = e.getID();
                evm.scheduleEvent(Event{ EventCodes::OpenChest });
                li.dontLoad.insert(pair);
                inpi.interact = false;
            }
        }
    });
}

void ZoneSystem::checkLevers(EntityManager& em, EventManager& evm)
{
    auto& li = em.getSingleton<LevelInfo>();
    if (li.playerDetected)
        return;
    using leverCMP = MP::TypeList<InteractiveComponent, PhysicsComponent, OneUseComponent>;
    using leverTAG = MP::TypeList<LeverTag>;

    em.forEach<leverCMP, leverTAG>([&](Entity&, InteractiveComponent& ic, PhysicsComponent& phy, OneUseComponent& ouc)
    {
        if (ouc.zone == li.num_zone)
        {
            // Revisamos si la palanca ha sido activada
            std::pair<uint8_t, uint8_t> pair{ li.mapID, ouc.id };

            if (li.dontLoad.find(pair) != li.dontLoad.end())
                return;

            // Calcula la distancia entre la posición del jugador y la posición de la palanca
            auto& playerPhy = em.getComponent<PhysicsComponent>(*em.getEntityByID(li.playerID));
            auto& playerPos = playerPhy.position;

            double distance = playerPos.distance(phy.position);
            double range = 7.5;

            // Si la palanca se encuentra a menos de 7.5 unidades de distancia del se muestra el mensaje de activar palanca
            if (distance < range && !ic.showButton)
                ic.showButton = true;

            else if (distance > range && ic.showButton)
                ic.showButton = false;

            auto& inpi = em.getSingleton<InputInfo>();

            if (inpi.interact && ic.showButton)
            {
                ic.showButton = false;
                li.dontLoad.insert(pair);
                inpi.interact = false;
                openDoorsZone(em, evm, phy.position);
            }
        }
    });
}

void ZoneSystem::checkSpawns(EntityManager& em, EventManager& evm)
{
    auto& li = em.getSingleton<LevelInfo>();
    using CMPs = MP::TypeList<InteractiveComponent, PhysicsComponent>;
    using spawnTag = MP::TypeList<SpawnTag>;

    em.forEach<CMPs, spawnTag>([&](Entity&, InteractiveComponent& ic, PhysicsComponent& phy)
    {
        auto& playerEnt = *em.getEntityByID(li.playerID);
        auto& playerPhy = em.getComponent<PhysicsComponent>(playerEnt);
        auto& playerPos = playerPhy.position;

        double distance = playerPos.distance(phy.position);
        double range = 4.5;
        if (distance < range && !ic.showButton && !li.playerDetected)
            ic.showButton = true;

        else if (distance > range && ic.showButton)
            ic.showButton = false;

        auto& inpi = em.getSingleton<InputInfo>();

        if (inpi.interact && ic.showButton && !li.playerDetected)
        {
            evm.scheduleEvent(Event{ EventCodes::SetSpawn });
        }
    });
}

void ZoneSystem::checkDoors(EntityManager& em, EventManager& evm)
{
    auto& li = em.getSingleton<LevelInfo>();
    using CMPs = MP::TypeList<InteractiveComponent, PhysicsComponent>;
    using doorTag = MP::TypeList<DoorTag>;

    em.forEach<CMPs, doorTag>([&](Entity& e, InteractiveComponent& ic, PhysicsComponent& phy)
    {
        if (e.hasTag<NoKeyTag>())
            return;
        auto& playerEnt = *em.getEntityByID(li.playerID);
        auto& playerPhy = em.getComponent<PhysicsComponent>(playerEnt);
        auto& playerPos = playerPhy.position;

        double distance = playerPos.distance(phy.position);
        double range = 8.5;

        auto& inpi = em.getSingleton<InputInfo>();
        auto& plfi = em.getSingleton<PlayerInfo>();

        if (distance < range && !ic.showButton && plfi.hasKey)
            ic.showButton = true;

        else if ((distance > range && ic.showButton) || !plfi.hasKey)
            ic.showButton = false;

        if (inpi.interact && ic.showButton && plfi.hasKey)
        {
            li.doorToOpen = e.getID();
            evm.scheduleEvent(Event{ EventCodes::OpenDoor });
        }
    });
}

void ZoneSystem::openDoorsZone(EntityManager& em, EventManager& evm, vec3d& leverPos)
{
    auto& li = em.getSingleton<LevelInfo>();
    using CMPs = MP::TypeList<PhysicsComponent>;
    using doorTag = MP::TypeList<NoKeyTag>;

    em.forEach<CMPs, doorTag>([&](Entity& e, PhysicsComponent& phy)
    {
        // Revisamos que la puerta esté en un radio de 30 unidades de la palanca
        double distance = leverPos.distance(phy.position);
        if (distance < 40.0)
        {
            li.doorToOpen = e.getID();
            evm.scheduleEvent(Event{ EventCodes::OpenDoor });
        }
    });
}

void ZoneSystem::checkTutorialEnemies(EntityManager& em)
{
    auto& li = em.getSingleton<LevelInfo>();
    auto& playerEnt = *em.getEntityByID(li.playerID);
    auto& playerPhy = em.getComponent<PhysicsComponent>(playerEnt);
    auto& playerPos = playerPhy.position;
    using noCMP = MP::TypeList<>;
    using enemyTag = MP::TypeList<EnemyTag, DestructibleTag>;
    li.tutorialEnemies.clear();

    em.forEachAny<noCMP, enemyTag>([&](Entity& e)
    {
        auto& phy = em.getComponent<PhysicsComponent>(e);
        double distance = playerPos.distance(phy.position);

        if (distance < 15.0)
        {
            li.tutorialEnemies.push_back(e.getID());
        }
    });

    // Si el jugador se choca con el primer golem, se le va a señalar el cofre con el bastón
    // if (!playerEnt.hasComponent<AttackComponent>() && playerPhy.stopped)
    //     li.viewPoint = { -33.714, 7.0, -43.494 };
}

// void ZoneSystem::checkDungeonSlimes(EntityManager& em, EventManager& evm)
// {
//     auto& li = em.getSingleton<LevelInfo>();

//     if (!li.dungeonKeyCreated)
//     {
//         using noCMPs = MP::TypeList<>;
//         using enemyTag = MP::TypeList<EnemyTag>;
//         bool slimesDead{ true };

//         em.forEach<noCMPs, enemyTag>([&](Entity& ent)
//         {
//             if (slimesDead && ent.hasTag<SlimeTag>())
//                 slimesDead = false;
//         });

//         if (slimesDead)
//             evm.scheduleEvent(Event{ EventCodes::SpawnDungeonKey });
//     }
// }