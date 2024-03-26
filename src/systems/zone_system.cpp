#include "zone_system.hpp"

void ZoneSystem::update(EntityManager& em, ENGI::GameEngine&, Ia_man& iam, EventManager& evm, MapManager& map) {
    auto& li = em.getSingleton<LevelInfo>();

    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, ZoneComponent& zon)
    {
        if (zon.changeZone) {

            // Comprobar en que zona estamos
            if (li.num_zone != zon.zone) {
                auto* playerEn = em.getEntityByID(li.playerID);
                auto& p = em.getComponent<PhysicsComponent>(*playerEn);

                li.num_zone = zon.zone;

                if (e.hasTag<LevelChangeTag>())
                {
                    switch (li.mapID)
                    {
                    case 0:
                    {
                        map.changeMap(em, 1, iam);
                        li.transition = true;
                        em.getSingleton<SoundSystem>().ambient_stop();

                        auto& plfi = em.getSingleton<PlayerInfo>();
                        vec3d spawnPoint = { 7.0, 22.0, -21.0 };
                        plfi.spawnPoint = spawnPoint;
                        p.position = spawnPoint;
                        break;
                    }
                    case 1:
                    {
                        li.currentScreen = GameScreen::ENDING;
                        break;
                    }

                    }
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
    checkZones(em, evm, zchi.getNpcZones(), [&](EntityManager& em, EventManager& evm) { checkNPCs(em, evm); });
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
            bool crusherClose = false;

            if (ch.checkCrushers)
            {
                using crusherCMP = MP::TypeList<PhysicsComponent>;
                using crusherTag = MP::TypeList<CrusherTag>;

                em.forEach<crusherCMP, crusherTag>([&](Entity&, PhysicsComponent& phyC)
                {
                    if (phy.position.distance(phyC.position) < 50.0)
                    {
                        crusherClose = true;
                    }

                    if (crusherClose)
                        return;
                });
            }

            // Si el cofre se encuentra a menos de 2 unidades de distancia del se muestra el mensaje de abrir cofre
            if (distance < range && !ch.isOpen && !ic.showButton && !li.playerDetected && !crusherClose)
                ic.showButton = true;

            else if ((distance > range && ic.showButton) || li.playerDetected)
                ic.showButton = false;

            if (distance < range && !ic.showButton && (li.playerDetected || crusherClose))
                ic.showLock = true;

            else if ((distance > range && ic.showLock) || !li.playerDetected)
                ic.showLock = false;

            auto& inpi = em.getSingleton<InputInfo>();

            if (inpi.interact && ic.showButton && !ch.isOpen)
            {
                ch.isOpen = true;
                ic.showButton = false;
                li.chestToOpen = e.getID();
                evm.scheduleEvent(Event{ EventCodes::OpenChest });
                li.dontLoad.insert(pair);
                em.getSingleton<SoundSystem>().sonido_interaccion_e();
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

                em.getSingleton<SoundSystem>().sonido_palanca();
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

        if (distance < range && !ic.showButton && !plfi.hasKey)
            ic.showLock = true;

        else if ((distance > range && ic.showLock) || plfi.hasKey)
            ic.showLock = false;

        if (inpi.interact && ic.showButton && plfi.hasKey)
        {
            li.doorToOpen = e.getID();
            evm.scheduleEvent(Event{ EventCodes::OpenDoor });
            em.getSingleton<SoundSystem>().sonido_interaccion_e();

            inpi.interact = false;
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

void ZoneSystem::checkNPCs(EntityManager& em, EventManager& evm)
{
    auto& li = em.getSingleton<LevelInfo>();
    auto& playerEnt = *em.getEntityByID(li.playerID);
    auto& playerPhy = em.getComponent<PhysicsComponent>(playerEnt);
    auto& playerPos = playerPhy.position;
    using noCMP = MP::TypeList<PhysicsComponent, InteractiveComponent, OneUseComponent>;
    using npcTag = MP::TypeList<NPCTag>;

    em.forEach<noCMP, npcTag>([&](Entity& e, PhysicsComponent& phy, InteractiveComponent& ic, OneUseComponent& ouc)
    {
        // Revisamos si ya se ha hablado con el npc
        std::pair<uint8_t, uint8_t> pair{ li.mapID, ouc.id };

        if (li.dontLoad.find(pair) != li.dontLoad.end())
            return;

        double distance = playerPos.distance(phy.position);
        double range = 7.0;
        if (distance < range && !ic.showButton && !li.playerDetected)
            ic.showButton = true;

        else if (distance > range && ic.showButton)
            ic.showButton = false;

        auto& inpi = em.getSingleton<InputInfo>();

        if (inpi.interact && ic.showButton && !li.playerDetected)
        {
            li.dontLoad.insert(pair);
            li.npcToTalk = e.getID();
            inpi.interact = false;
            ic.showButton = false;
            evm.scheduleEvent(Event{ EventCodes::NPCDialog });
        }
    });
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