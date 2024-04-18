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
                    em.getSingleton<SoundSystem>().ambient_stop();
                    em.getSingleton<SoundSystem>().music_stop();
                    switch (li.mapID)
                    {
                    case 0:
                    {
                        map.changeMap(em, 1, iam);
                        li.transition = true;

                        auto& plfi = em.getSingleton<PlayerInfo>();
                        vec3d spawnPoint = { 7.0, 22.0, -21.0 };
                        plfi.spawnPoint = spawnPoint;
                        p.position = spawnPoint;
                        em.getSingleton<SoundSystem>().update();
                        break;
                    }
                    case 1:
                    {
                        map.changeMap(em, 2, iam);
                        li.transition = true;
                        // em.getSingleton<SoundSystem>().ambient_stop();
                        em.getSingleton<SoundSystem>().update();

                        auto& plfi = em.getSingleton<PlayerInfo>();
                        vec3d spawnPoint = { -28.0, 49.0, -30.0 };
                        plfi.spawnPoint = spawnPoint;
                        p.position = spawnPoint;
                        break;
                    }
                    case 2:
                    {
                        if (elapsedEnding < countdownEnding) {
                            elapsedEnding += timeStep;
                            return;
                        }
                        li.currentScreen = GameScreen::ENDING;
                        elapsedEnding = 0.0f;
                        // map.changeMap(em, 2, iam);
                        // li.transition = true;
                        // // em.getSingleton<SoundSystem>().ambient_stop();

                        // auto& plfi = em.getSingleton<PlayerInfo>();
                        // vec3d spawnPoint = { -28.0, 49.0, -30.0 };
                        // plfi.spawnPoint = spawnPoint;
                        // p.position = spawnPoint;
                        break;
                    }

                    }
                }
            }

            zon.changeZone = false;
        }
    });

    // Cosas que hacer en cada zona
    switch (li.mapID)
    {
    case 0:
    {

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
        break;
    }
    case 1:
    {
        switch (li.num_zone)
        {
        case 18:
        {
            checkTutorialEnemies(em);
            break;
        }
        case 23:
        {
            checkTutorialEnemies(em);
            break;
        }
        }
        break;
    }
    case 2:
    {
        switch (li.num_zone)
        {
        case 0:
        {
            checkVolcanoLava(em);
            break;
        }
        case 1:
        {
            checkVolcanoLava(em);
            break;
        }
        case 5:
        {
            checkVolcanoLava(em);
            break;
        }
        case 7:
        {
            checkVolcanoLava(em);
            break;
        }
        }
    }
    }

    auto& zchi = em.getSingleton<ZoneCheckInfo>();

    checkZones(em, evm, zchi.getZones(InteractableType::Chest), [&](EntityManager& em, EventManager& evm) { checkChests(em, evm); });
    checkZones(em, evm, zchi.getZones(InteractableType::Lever), [&](EntityManager& em, EventManager& evm) { checkLevers(em, evm); });
    checkZones(em, evm, zchi.getZones(InteractableType::Door), [&](EntityManager& em, EventManager& evm) { checkDoors(em, evm); });
    checkZones(em, evm, zchi.getZones(InteractableType::NPC), [&](EntityManager& em, EventManager& evm) { checkNPCs(em, evm); });
    checkZones(em, evm, zchi.getZones(InteractableType::Ladder), [&](EntityManager& em, EventManager&) { checkLadders(em); });
    checkZones(em, evm, zchi.getZones(InteractableType::Sign), [&](EntityManager& em, EventManager&) { checkSigns(em); });
    checkZones(em, evm, zchi.getZones(InteractableType::MissionOBJ), [&](EntityManager& em, EventManager& evm) { checkMissionObjs(em, evm); });
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
        if (ouc.zones.find(li.num_zone) != ouc.zones.end())
        {
            // Revisamos si el cofre ha sido abierto
            std::pair<uint8_t, uint8_t> pair{ li.mapID, ouc.id };

            if (li.dontLoad.find(pair) != li.dontLoad.end())
                return;

            // Calcula la distancia entre la posición del jugador y la posición del cofre
            auto& playerPhy = em.getComponent<PhysicsComponent>(*em.getEntityByID(li.playerID));
            auto& playerPos = playerPhy.position;

            double distance = playerPos.distance(phy.position);
            double range = ic.range;

            // if (ch.checkCrushers)
            // {
            using crusherCMP = MP::TypeList<PhysicsComponent>;
            using crusherTag = MP::TypeList<EnemyTag>;

            ch.closeEnemies = 0;
            em.forEach<crusherCMP, crusherTag>([&](Entity& e, PhysicsComponent& phyC)
            {
                if (e.hasTag<AngryBushTag>() || e.hasTag<DummyTag>() || e.hasTag<AngryBushTag2>())
                    return;
                if (phy.position.distance(phyC.position) < 50.0 &&
                    std::abs(phy.position.y() - phyC.position.y()) < 4.0)
                {
                    ch.closeEnemies += 1;
                }
            });

            if (ch.closeEnemies > ch.maxEnemies)
                ch.maxEnemies = ch.closeEnemies;
            // }

            // Si el cofre se encuentra a menos de 2 unidades de distancia del se muestra el mensaje de abrir cofre
            if (distance < range && !ch.isOpen && !ic.showButton && !li.playerDetected && ch.closeEnemies == 0)
                ic.showButton = true;

            else if ((distance > range && ic.showButton) || li.playerDetected)
                ic.showButton = false;

            if (distance < range && !ic.showButton && (li.playerDetected || ch.closeEnemies > 0))
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
        if (ouc.zones.find(li.num_zone) != ouc.zones.end())
        {
            // Revisamos si la palanca ha sido activada
            std::pair<uint8_t, uint8_t> pair{ li.mapID, ouc.id };

            if (li.dontLoad.find(pair) != li.dontLoad.end())
                return;

            // Calcula la distancia entre la posición del jugador y la posición de la palanca
            auto& playerPhy = em.getComponent<PhysicsComponent>(*em.getEntityByID(li.playerID));
            auto& playerPos = playerPhy.position;

            double distance = playerPos.distance(phy.position);
            double range = ic.range;

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
        double range = ic.range;

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

        double range = 15.0;
        if (li.mapID == 1)
            range = 18.0;

        if (distance < range)
        {
            li.tutorialEnemies.push_back(e.getID());
        }
    });
}

void ZoneSystem::checkVolcanoLava(EntityManager& em)
{
    auto& li = em.getSingleton<LevelInfo>();
    auto& playerEnt = *em.getEntityByID(li.playerID);
    auto& playerPhy = em.getComponent<PhysicsComponent>(playerEnt);
    auto& playerPos = playerPhy.position;
    using noCMP = MP::TypeList<>;
    using enemyTag = MP::TypeList<LavaTag>;
    li.volcanoLava.clear();

    em.forEachAny<noCMP, enemyTag>([&](Entity& e)
    {
        auto& phy = em.getComponent<PhysicsComponent>(e);
        double distance = playerPos.distance(phy.position);

        double range = 15.0;

        if (distance < range)
        {
            li.volcanoLava.push_back(e.getID());
        }
    });
}

void ZoneSystem::checkLadders(EntityManager& em)
{
    auto& plfi = em.getSingleton<PlayerInfo>();
    if (plfi.onLadder)
        return;

    auto& li = em.getSingleton<LevelInfo>();
    auto& playerEnt = *em.getEntityByID(li.playerID);
    auto& playerPhy = em.getComponent<PhysicsComponent>(playerEnt);
    auto& playerPos = playerPhy.position;
    using noCMP = MP::TypeList<PhysicsComponent, InteractiveComponent, LadderComponent>;
    using npcTag = MP::TypeList<LadderTag>;

    em.forEach<noCMP, npcTag>([&](Entity&, PhysicsComponent& phy, InteractiveComponent& ic, LadderComponent& ldc)
    {
        double distance = playerPos.distance(phy.position);
        double range = ic.range;

        auto& inpi = em.getSingleton<InputInfo>();

        if (distance < range && !ic.showButton)
            ic.showButton = true;

        else if (distance > range && ic.showButton)
            ic.showButton = false;

        if (inpi.interact && ic.showButton)
        {
            playerPos = { phy.position.x(), playerPos.y() + 0.5, phy.position.z() };
            playerPhy.gravity = 0;
            playerPhy.orientation = ldc.orientation;

            plfi.onLadder = true;
            inpi.interact = false;
            ic.showButton = false;
        }
    });
}

void ZoneSystem::checkSigns(EntityManager& em)
{
    auto& li = em.getSingleton<LevelInfo>();
    auto& playerEnt = *em.getEntityByID(li.playerID);
    auto& playerPhy = em.getComponent<PhysicsComponent>(playerEnt);
    auto& playerPos = playerPhy.position;
    using noCMP = MP::TypeList<PhysicsComponent, InteractiveComponent, MessageComponent>;
    using npcTag = MP::TypeList<SignTag>;

    em.forEach<noCMP, npcTag>([&](Entity&, PhysicsComponent& phy, InteractiveComponent& ic, MessageComponent& msg)
    {
        double distance = playerPos.distance(phy.position);
        double range = ic.range;

        auto& inpi = em.getSingleton<InputInfo>();

        if (distance < range && !ic.showButton)
            ic.showButton = true;

        else if (distance > range && ic.showButton)
            ic.showButton = false;

        if (inpi.interact && ic.showButton)
        {
            auto& txti = em.getSingleton<TextInfo>();
            auto msgs = msg.messages;

            while (!msgs.empty())
            {
                txti.addText({ SpeakerType::NONE, msgs.front() });
                msgs.pop();
            }

            inpi.interact = false;
        }
    });
}

void ZoneSystem::checkMissionObjs(EntityManager& em, EventManager& evm)
{
    auto& li = em.getSingleton<LevelInfo>();
    auto& playerEnt = *em.getEntityByID(li.playerID);
    auto& playerPhy = em.getComponent<PhysicsComponent>(playerEnt);
    auto& playerPos = playerPhy.position;
    using noCMP = MP::TypeList<PhysicsComponent, InteractiveComponent>;
    using npcTag = MP::TypeList<MissionObjTag>;

    em.forEach<noCMP, npcTag>([&](Entity& e, PhysicsComponent& phy, InteractiveComponent& ic)
    {
        double distance = playerPos.distance(phy.position);
        double range = ic.range;

        auto& inpi = em.getSingleton<InputInfo>();

        if (distance < range && !ic.showButton)
            ic.showButton = true;

        else if (distance > range && ic.showButton)
            ic.showButton = false;

        if (inpi.interact && ic.showButton)
        {
            li.boatPartFound = e.getID();
            evm.scheduleEvent(Event{ EventCodes::BoatPartFound });

            inpi.interact = false;
        }
    });
}

void ZoneSystem::checkNPCs(EntityManager& em, EventManager&)
{
    auto& li = em.getSingleton<LevelInfo>();
    auto& playerEnt = *em.getEntityByID(li.playerID);
    auto& playerPhy = em.getComponent<PhysicsComponent>(playerEnt);
    auto& playerPos = playerPhy.position;
    using noCMP = MP::TypeList<PhysicsComponent, InteractiveComponent, OneUseComponent, DispatcherComponent>;
    using npcTag = MP::TypeList<NPCTag>;

    em.forEach<noCMP, npcTag>([&](Entity& e, PhysicsComponent& phy, InteractiveComponent& ic, OneUseComponent& ouc, DispatcherComponent& dc)
    {
        // Revisamos si ya se ha hablado con el npc
        std::pair<uint8_t, uint8_t> pair{ li.mapID, ouc.id };

        if (li.dontLoad.find(pair) != li.dontLoad.end())
            return;

        double distance = playerPos.distance(phy.position);
        double distanceY = std::abs(playerPos.y() - phy.position.y());
        double range = ic.range;

        if (distance < range && distanceY < 2.0 && !li.playerDetected)
        {
            li.dontLoad.insert(pair);
            li.npcToTalk = e.getID();
            playerPhy.lookAt(phy.position);
            playerPhy.notMove = true;
            playerPhy.moveAt(phy.position);
            phy.lookAt(playerPhy.position);

            li.viewPoint = phy.position;
            li.events.insert(dc.eventCodes.begin(), dc.eventCodes.end());
            dc.eventCodes.clear();
            // if (dc.eventCodes.size() > 0)
            //     evm.scheduleEvent({ static_cast<EventCodes>(dc.eventCodes[0]) });
        }
    });
}

// void ZoneSystem::checkSpawns(EntityManager& em, EventManager& evm)
// {
//     auto& li = em.getSingleton<LevelInfo>();
//     using CMPs = MP::TypeList<InteractiveComponent, PhysicsComponent>;
//     using spawnTag = MP::TypeList<SpawnTag>;

//     em.forEach<CMPs, spawnTag>([&](Entity&, InteractiveComponent& ic, PhysicsComponent& phy)
//     {
//         auto& playerEnt = *em.getEntityByID(li.playerID);
//         auto& playerPhy = em.getComponent<PhysicsComponent>(playerEnt);
//         auto& playerPos = playerPhy.position;

//         double distance = playerPos.distance(phy.position);
//         double range = 4.5;
//         if (distance < range && !ic.showButton && !li.playerDetected)
//             ic.showButton = true;

//         else if (distance > range && ic.showButton)
//             ic.showButton = false;

//         auto& inpi = em.getSingleton<InputInfo>();

//         if (inpi.interact && ic.showButton && !li.playerDetected)
//         {
//             evm.scheduleEvent(Event{ EventCodes::SetSpawn });
//         }
//     });
// }

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