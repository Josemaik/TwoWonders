#include "map_manager.hpp"
#include <fstream>

void MapManager::createMap(EntityManager& em, uint8_t mapID, Ia_man& iam) {

    auto& li = em.getSingleton<LevelInfo>();

    if (li.levelChanged || fileMap.size() == 0)
    {
        switch (mapID)
        {
        case 0:
        {
            li.mapID = 0;
            map = loadMap("assets/levels/maps/lvl_0.kaiwa");
            em.getSingleton<SoundSystem>().playAmbient();
            break;
        }
        case 1:
        {
            li.mapID = 1;
            map = loadMap("assets/levels/maps/lvl_1.kaiwa");
            em.getSingleton<SoundSystem>().sonido_mazmorra();
            break;
        }
        case 2:
            li.mapID = 2;
            map = loadMap("assets/levels/maps/lvl_2.kaiwa");
            break;

        default:
            break;
        }

        li.levelChanged = false;
    }

    generateMapFromJSON(em, map, iam);
}

mapType MapManager::loadMap(const std::string& mapFile)
{
    if (fileMap != mapFile)
        fileMap = mapFile;
    std::ifstream file(mapFile);
    rapidjson::IStreamWrapper isw(file);
    rapidjson::Document map;
    map.ParseStream(isw);
    return map;
}

void MapManager::generateMapFromJSON(EntityManager& em, const mapType& map, Ia_man& iam)
{
    mapSizeType size{};
    const valueType* chunks = nullptr;
    int j = 0;

    if (state == LoadState::LOAD_CHUNKS)
    {
        chunks = &map["Chunks"];
        size = chunks->Size();
        chunksVec.reserve(size);
    }
    else
        size = static_cast<mapSizeType>(chunksVec.size());

    for (mapSizeType i = 0; i < size; i++)
    {
        if (state == LoadState::LOAD_CHUNKS)
        {
            std::string chunkName = "Chunk" + std::to_string(i);
            chunksVec.push_back(&(*chunks)[i][chunkName.c_str()]);
            generateChunkModel(em, i);
        }
        else
        {
            const valueType& chunk = *chunksVec[i];
            switch (state)
            {
            case LoadState::LOAD_GROUND:
            {
                const valueType& overworld = chunk[0]["overworld"];
                const valueType& groundArray = overworld["Ground"];
                generateGround(em, groundArray, j);
                break;
            }
            case LoadState::LOAD_WALLS:
            {
                const valueType& overworld = chunk[0]["overworld"];
                const valueType& wallArray = overworld["Walls"];
                generateWalls(em, wallArray);
                break;
            }
            case LoadState::LOAD_RAMPS:
            {
                const valueType& overworld = chunk[0]["overworld"];
                const valueType& rampArray = overworld["Ramps"];
                generateRamps(em, rampArray);
                break;
            }
            case LoadState::LOAD_INTERACTABLES:
            {
                const valueType& overworld = chunk[0]["overworld"];
                const valueType& interactablesArray = overworld["Interactive"];
                generateInteractables(em, interactablesArray);
                break;
            }
            case LoadState::LOAD_OBJECTS:
            {
                auto& li = em.getSingleton<LevelInfo>();
                const valueType& underworld = chunk[1]["underworld"];
                const valueType& objectArray = underworld["Objects"];
                generateObjects(em, objectArray, li.mapID);
                break;
            }
            case LoadState::LOAD_ENEMIES:
            {
                const valueType& underworld = chunk[1]["underworld"];
                const valueType& enemyArray = underworld["Enemies"];
                generateEnemies(em, enemyArray, iam);
                break;
            }
            case LoadState::LOAD_NPCS:
            {
                const valueType& underworld = chunk[1]["underworld"];
                const valueType& NPCsArray = underworld["NPCs"];
                generateNPCs(em, NPCsArray);
                break;
            }
            case LoadState::LOAD_NAVMESHES:
            {
                generateNavmeshes(em);
                break;
            }
            default:
                break;
            }
        }

    }

    if (!(state == LoadState::LOAD_COMPLETE))
        state = static_cast<LoadState>(static_cast<int>(state) + 1);
}

void MapManager::generateChunkModel(EntityManager& em, mapSizeType& i)
{

    // Creamos la entidad modelo del chunk
    auto& modelEntity{ em.newEntity() };

    // Asignamos etiquetas al número del chunk
    switch (i)
    {
    case 0:
        em.addTag<Chunk0Tag>(modelEntity);
        break;
    case 1:
        em.addTag<Chunk1Tag>(modelEntity);
        break;
    case 2:
        em.addTag<Chunk2Tag>(modelEntity);
        break;
    case 3:
        em.addTag<Chunk3Tag>(modelEntity);
        break;
    case 4:
        em.addTag<Chunk4Tag>(modelEntity);
        break;
    case 5:
        em.addTag<Chunk5Tag>(modelEntity);
        break;
    case 6:
        em.addTag<Chunk6Tag>(modelEntity);
        break;
    }
    em.addComponent<RenderComponent>(modelEntity, RenderComponent{ .position = vec3d::zero(), .scale = vec3d::zero(), .orientation = 90.0 * DEGTORAD, .rotationVec = { 0.0, -1.0, 0.0 } });
}

void MapManager::generateGround(EntityManager& em, const valueType& groundArray, int& j)
{
    vec2d min{}, max{};
    double minHeigth = 0.0;
    for (mapSizeType i = 0; i < groundArray.Size(); i++)
    {
        auto& groundEntity = em.newEntity();
        em.addTag<GroundTag>(groundEntity);

        // Extraemos los datos del json
        const valueType& ground = groundArray[i];
        vec3d groundPosition{ ground["position"][1].GetDouble(), ground["position"][2].GetDouble(), ground["position"][0].GetDouble() };
        vec3d groundScale{ ground["scale"][1].GetDouble(), ground["scale"][2].GetDouble(), ground["scale"][0].GetDouble() };
        vec3d rotationVec{ ground["rotVector"][1].GetDouble(), ground["rotVector"][2].GetDouble(), ground["rotVector"][0].GetDouble() };
        double orientation{ ground["rotation"].GetDouble() };
        double rot = orientation * DEGTORAD;
        Color color{ WHITE };

        // Sacamos los máximos y los mínimos
        if (i == 0)
        {
            min.x = groundPosition.x() - groundScale.x() / 2;
            min.y = groundPosition.z() - groundScale.z() / 2;

            max.x = groundPosition.x() + groundScale.x() / 2;
            max.y = groundPosition.z() + groundScale.z() / 2;

            minHeigth = groundPosition.y();
        }
        else
        {
            if (min.x > groundPosition.x() - groundScale.x() / 2)
                min.x = groundPosition.x() - groundScale.x() / 2;
            if (min.y > groundPosition.z() - groundScale.z() / 2)
                min.y = groundPosition.z() - groundScale.z() / 2;

            if (max.x < groundPosition.x() + groundScale.x() / 2)
                max.x = groundPosition.x() + groundScale.x() / 2;
            if (max.y < groundPosition.z() + groundScale.z() / 2)
                max.y = groundPosition.z() + groundScale.z() / 2;

            if (minHeigth > groundPosition.y())
                minHeigth = groundPosition.y();
        }

        // Creamos los componentes del suelo
        auto& r = em.addComponent<RenderComponent>(groundEntity, RenderComponent{ .position = groundPosition, .scale = groundScale, .color = color, .visible = false, .orientation = rot, .rotationVec = rotationVec });
        auto& p = em.addComponent<PhysicsComponent>(groundEntity, PhysicsComponent{ .position = r.position, .velocity = vec3d::zero(), .scale = r.scale, .gravity = .0, .orientation = rot, .rotationVec = r.rotationVec });
        em.addComponent<ColliderComponent>(groundEntity, ColliderComponent{ p.position, r.scale, BehaviorType::STATIC });
        em.addTag<SeparateModelTag>(groundEntity);
    }

    // Sacamos la posición del centro del chunk
    vec3d center = { (min.x + max.x) / 2, minHeigth, (min.y + max.y) / 2 };

    auto& li = em.getSingleton<LevelInfo>();
    double zoneHeight = 20.0;
    if (li.mapID == 2)
        zoneHeight = 65.0;

    // Creamos 4 zonas para el chunk
    vec3d zoneScale = { (max.x - min.x) / 2, zoneHeight, (max.y - min.y) / 2 };
    int k = 0;
    int limit = j + 4;
    for (; j < limit; j++)
    {
        auto& zoneEntity = em.newEntity();
        em.addTag<ZoneTag>(zoneEntity);

        // Ajustamos la posición de la zona
        vec3d zonePosition = center;
        zonePosition.setX(center.x() + (k % 2 - 0.5) * zoneScale.x());
        zonePosition.setZ(center.z() + (k / 2 - 0.5) * zoneScale.z());
        zonePosition.setY(center.y() + 2); // Elevamos la zona en 2 unidades en y

        // Creamos los componentes de la zona
        em.addComponent<ZoneComponent>(zoneEntity, ZoneComponent{ .zone = static_cast<uint16_t>(j) });
        auto& rz = em.addComponent<RenderComponent>(zoneEntity, RenderComponent{ .position = zonePosition, .scale = zoneScale, .visible = false });
        auto& pz = em.addComponent<PhysicsComponent>(zoneEntity, PhysicsComponent{ .position = rz.position, .velocity = vec3d::zero(), .scale = rz.scale, .gravity = .0 });
        auto& cz = em.addComponent<ColliderComponent>(zoneEntity, ColliderComponent{ pz.position, rz.scale, BehaviorType::ZONE });
        zoneBounds.insert({ j, cz.boundingBox });
        k += 1;
    }
}

void MapManager::generateWalls(EntityManager& em, const valueType& wallArray)
{
    for (mapSizeType i = 0; i < wallArray.Size(); i++)
    {
        auto& entity = em.newEntity();
        em.addTag<WallTag>(entity);
        // Extraemos los datos del json
        const valueType& wall = wallArray[i];
        vec3d position{ wall["position"][1].GetDouble(), wall["position"][2].GetDouble(), wall["position"][0].GetDouble() };
        vec3d scale{ wall["scale"][1].GetDouble(), wall["scale"][2].GetDouble(), wall["scale"][0].GetDouble() };
        vec3d rotationVec{ wall["rotVector"][1].GetDouble(), wall["rotVector"][2].GetDouble(), wall["rotVector"][0].GetDouble() };
        double orientation{ wall["rotation"].GetDouble() };
        Color color{ LIME };

        double rot = orientation * DEGTORAD;

        // Creamos los componentes
        auto& r = em.addComponent<RenderComponent>(entity, RenderComponent{ .position = position, .scale = scale, .color = color,  .visible = false, .orientation = rot, .rotationVec = rotationVec });
        auto& p = em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = r.position, .velocity = vec3d::zero(), .scale = r.scale, .gravity = .0, .orientation = rot, .rotationVec = r.rotationVec });
        em.addComponent<ColliderComponent>(entity, ColliderComponent{ p.position, r.scale, BehaviorType::STATIC });
    }
}

void MapManager::generateRamps(EntityManager& em, const valueType& rampArray)
{
    for (mapSizeType i = 0; i < rampArray.Size(); i++)
    {
        auto& entity = em.newEntity();
        em.addTag<RampTag>(entity);

        // Extraemos los datos del json
        const valueType& ramp = rampArray[i];
        vec2d min{ ramp["min"][1].GetDouble(), ramp["min"][0].GetDouble() };
        vec2d max{ ramp["max"][1].GetDouble(), ramp["max"][0].GetDouble() };
        vec3d offset{ ramp["offset"][1].GetDouble(), ramp["offset"][2].GetDouble(), ramp["offset"][0].GetDouble() };
        double slope{ ramp["slope"].GetDouble() };
        RampType type{ static_cast<RampType>(ramp["type"].GetUint()) };

        // Creamos las dimensiones de la rampa y su posición
        vec3d rmin = { min.x, offset.y() - 5, min.y };
        vec3d rmax = { max.x, offset.y() + 20, max.y };

        vec3d rampPos = (rmin + rmax) / 2;
        vec3d rampSize = rmax - rmin;

        // Creamos los componentes
        em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = rampPos, .scale = rampSize, .gravity = 0 });
        em.addComponent<ColliderComponent>(entity, ColliderComponent{ rampPos, rampSize, BehaviorType::RAMP });
        em.addComponent<RampComponent>(entity, RampComponent{ .min = min, .max = max, .slope = slope, .offset = offset, .type = type });
    }
}

void MapManager::generateObjects(EntityManager& em, const valueType& objectArray, uint8_t mapID)
{
    for (mapSizeType i = 0; i < objectArray.Size(); i++)
    {
        auto& li = em.getSingleton<LevelInfo>();
        const valueType& obj = objectArray[i];
        uint8_t objId = static_cast<uint8_t>(obj["id"].GetUint());

        std::pair<uint8_t, uint8_t> pair{ mapID, objId };

        if (li.dontLoad.find(pair) != li.dontLoad.end())
            continue;

        // Extraemos los datos del json
        vec3d position{ obj["position"][0].GetDouble(), obj["position"][2].GetDouble(), obj["position"][1].GetDouble() };
        vec3d scale{ obj["scale"][0].GetDouble(), obj["scale"][2].GetDouble(), obj["scale"][1].GetDouble() };
        Color color{ RED };
        ObjectType type{ static_cast<ObjectType>(obj["type"].GetDouble()) };

        auto& entity = em.newEntity();
        em.addTag<ObjectTag>(entity);

        // Creamos los componentes
        auto& r = em.addComponent<RenderComponent>(entity, RenderComponent{ .position = position, .scale = scale, .color = color });
        auto& p = em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = r.position, .velocity = vec3d::zero(), .scale = r.scale });
        em.addComponent<ColliderComponent>(entity, ColliderComponent{ p.position, r.scale, BehaviorType::STATIC });
        em.addComponent<ObjectComponent>(entity, ObjectComponent{ .type = type, .inmortal = true, .objID = objId });
    }
}

void MapManager::generateEnemies(EntityManager& em, const valueType& enemyArray, Ia_man& iam)
{
    for (mapSizeType i = 0; i < enemyArray.Size(); i++)
    {
        const valueType& enemy = enemyArray[i];
        iam.createEnemy(em, enemy);
    }
}

void MapManager::generateInteractables(EntityManager& em, const valueType& interactablesArray)
{
    for (mapSizeType i = 0; i < interactablesArray.Size(); i++)
    {
        const valueType& interactable = interactablesArray[i];

        // Extraemos los datos del json
        vec3d position{ interactable["position"][1].GetDouble(), interactable["position"][2].GetDouble(), interactable["position"][0].GetDouble() };
        vec3d scale{ interactable["scale"][1].GetDouble(), interactable["scale"][2].GetDouble(), interactable["scale"][0].GetDouble() };
        vec3d rotationVec{ interactable["rotVector"][1].GetDouble(), interactable["rotVector"][2].GetDouble(), interactable["rotVector"][0].GetDouble() };
        double orientation{ interactable["rotation"].GetDouble() };
        Color color{ LIGHTGRAY };
        InteractableType type{ static_cast<InteractableType>(interactable["type"].GetInt()) };

        double rot = orientation * DEGTORAD;

        // Creamos los componentes
        auto& entity = em.newEntity();
        auto& r = em.addComponent<RenderComponent>(entity, RenderComponent{ .position = position, .scale = scale, .color = color, .orientation = rot, .rotationVec = rotationVec });
        auto& p = em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = r.position, .velocity = vec3d::zero(), .scale = r.scale, .gravity = 0, .orientation = rot, .rotationVec = r.rotationVec });
        auto& c = em.addComponent<ColliderComponent>(entity, ColliderComponent{ p.position, r.scale, BehaviorType::STATIC });
        em.addComponent<InteractiveComponent>(entity);

        switch (type)
        {
        case InteractableType::Chest:
        {
            em.addTag<ChestTag>(entity);
            em.addTag<WallTag>(entity);
            ObjectType content{ static_cast<ObjectType>(interactable["content"].GetInt()) };

            addMessageCmp(em, entity, interactable);

            em.addComponent<OneUseComponent>(entity, OneUseComponent{ .id = unique_ids++ });
            [[maybe_unused]] auto& cc = em.addComponent<ChestComponent>(entity, ChestComponent{ .dropPosition = { vec3d::zero() }, .content = content });

            if (interactable.HasMember("offsetZ"))
            {
                r.offset = interactable["offsetZ"][0].GetDouble();
            }

            if (interactable.HasMember("checkCrushers"))
            {
                cc.checkCrushers = true;
            }

            checkDispatcher(em, entity, interactable);
            addToZone(em, entity, type);
            break;
        }
        case InteractableType::Destructible:
        {
            em.addTag<DestructibleTag>(entity);
            em.addTag<WallTag>(entity);
            int life{ interactable["life"].GetInt() };
            em.addComponent<LifeComponent>(entity, LifeComponent{ .life = life });
            auto& d = em.addComponent<DestructibleComponent>(entity);

            for (mapSizeType j = 0; j < interactable["weaknesses"].Size(); j++)
            {
                auto& weakness = interactable["weaknesses"][j];
                d.addWeakness(static_cast<ElementalType>(weakness.GetInt()));
            }

            if (interactable.HasMember("offsetZ"))
            {
                r.offset = interactable["offsetZ"][0].GetDouble();
            }

            em.destroyComponent<InteractiveComponent>(entity);
            break;
        }
        case InteractableType::Door:
        {
            auto& li = em.getSingleton<LevelInfo>();

            switch (li.mapID)
            {
            case 0:
            {
                em.addTag<SeparateModelTag>(entity);
                r.position = vec3d::zero();
                break;
            }
            }
            em.addTag<DoorTag>(entity);
            em.addTag<WallTag>(entity);
            if (interactable.HasMember("noKey"))
                em.addTag<NoKeyTag>(entity);

            addToZone(em, entity, type);
            break;
        }
        case InteractableType::Level:
        {
            if (interactable.HasMember("offsetZ"))
            {
                r.offset = interactable["offsetZ"][0].GetDouble();
                em.addTag<SeparateModelTag>(entity);
                r.position = vec3d::zero();
            }
            else
                r.visible = false;

            em.addTag<LevelChangeTag>(entity);
            c.behaviorType = BehaviorType::ZONE;

            zoneBounds.insert({ zoneBounds.size(), c.boundingBox });

            em.addComponent<ZoneComponent>(entity, ZoneComponent{ .zone = static_cast<uint16_t>(zoneBounds.size() - 1) });
            break;
        }
        case InteractableType::Spawn:
        {
            em.addTag<SpawnTag>(entity);
            c.behaviorType = BehaviorType::SPAWN;
            if (interactable.HasMember("offsetZ"))
            {
                r.offset = interactable["offsetZ"][0].GetDouble();
            }

            checkDispatcher(em, entity, interactable);
            break;
        }
        case InteractableType::Lever:
        {
            em.addTag<WallTag>(entity);
            em.addTag<LeverTag>(entity);
            em.addComponent<OneUseComponent>(entity, OneUseComponent{ .id = unique_ids++ });
            addToZone(em, entity, type);
            break;
        }
        case InteractableType::DamageObj:
        {
            em.addTag<LavaTag>(entity);
            c.behaviorType = BehaviorType::LAVA;
            if (interactable.HasMember("offsetZ"))
            {
                r.offset = interactable["offsetZ"][0].GetDouble();
            }
            break;
        }
        case InteractableType::Roca:
            r.visible = false;
            break;
        case InteractableType::Ladder:
        {
            em.addTag<LadderTag>(entity);
            r.visible = false;
            c.behaviorType = BehaviorType::LADDER;
            addToZone(em, entity, type);

            auto yMin = c.boundingBox.min.y();
            auto yMax = c.boundingBox.max.y();
            auto rot2 = (orientation - 90.0) * DEGTORAD;
            em.addComponent<LadderComponent>(entity, LadderComponent{ .orientation = rot2, .yMin = yMin, .yMax = yMax });
            break;
        }
        case InteractableType::Sign:
        {
            em.addTag<SignTag>(entity);

            addMessageCmp(em, entity, interactable);
            addToZone(em, entity, type);
            if (interactable.HasMember("offsetZ"))
            {
                r.offset = interactable["offsetZ"][0].GetDouble();
            }
            break;
        }
        case InteractableType::Table:
        {
            em.addTag<TableTag>(entity);
            if (interactable.HasMember("offsetZ"))
            {
                r.offset = interactable["offsetZ"][0].GetDouble();
            }
            break;
        }
        default:
            break;
        }
    }
}


void MapManager::generateNPCs(EntityManager& em, const valueType& npcArray)
{
    for (mapSizeType i = 0; i < npcArray.Size(); i++)
    {
        const valueType& npc = npcArray[i];

        vec3d position = { npc["position"][0].GetDouble(), npc["position"][1].GetDouble(), npc["position"][2].GetDouble() };
        vec3d rotationVec{ npc["rotVector"][1].GetDouble(), npc["rotVector"][2].GetDouble(), npc["rotVector"][0].GetDouble() };
        double orientation{ npc["rotation"].GetDouble() };
        vec3d scale = { npc["scale"][0].GetDouble(), npc["scale"][1].GetDouble(), npc["scale"][2].GetDouble() };
        Color color = { static_cast<unsigned char>(npc["color"][0].GetUint()), static_cast<unsigned char>(npc["color"][1].GetUint()), static_cast<unsigned char>(npc["color"][2].GetUint()), static_cast<unsigned char>(npc["color"][3].GetUint()) };
        double rot = orientation * DEGTORAD;
        double max_speed = npc["max_speed"].GetDouble();
        Path_t<4> path = { vec3d{npc["path"][0][0].GetDouble(), npc["path"][0][1].GetDouble(), npc["path"][0][2].GetDouble()},
                       vec3d{npc["path"][1][0].GetDouble(), npc["path"][1][1].GetDouble(), npc["path"][1][2].GetDouble()},
                       vec3d{npc["path"][2][0].GetDouble(), npc["path"][2][1].GetDouble(), npc["path"][2][2].GetDouble()},
                       vec3d{npc["path"][3][0].GetDouble(), npc["path"][3][1].GetDouble(), npc["path"][3][2].GetDouble()} };
        double arrival_radius = npc["arrival_radius"].GetDouble();
        uint8_t npcType = static_cast<uint8_t>(npc["npc"].GetUint());

        vec_t.push_back(std::make_unique<BehaviourTree_t>());
        auto& tree = *vec_t.back();

        tree.createNode<BTAction_PatrolNPC>();

        auto& entity{ em.newEntity() };
        em.addTag<NPCTag>(entity);

        switch (npcType)
        {
        case 0:
            em.addTag<NomadTag>(entity);
            break;
        case 1:
            em.addTag<InvestigatorTag>(entity);
            break;
        default:
            break;
        }

        auto& wr = em.addComponent<RenderComponent>(entity, RenderComponent{ .position = position, .scale = scale, .color = color,.orientation = rot,.rotationVec = rotationVec });
        auto& wp = em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = wr.position, .scale = wr.scale,.orientation = rot,.rotationVec = rotationVec, .max_speed = max_speed });
        em.addComponent<ColliderComponent>(entity, ColliderComponent{ wp.position, wr.scale, BehaviorType::STATIC });
        em.addComponent<InteractiveComponent>(entity);
        em.addComponent<NPCComponent>(entity, NPCComponent{ .path = path,.behaviourTree = &tree,.arrival_radius = arrival_radius });

        em.addComponent<OneUseComponent>(entity, OneUseComponent{ .id = unique_ids++ });

        checkDispatcher(em, entity, npc);

        addToZone(em, entity, InteractableType::NPC);
    }
}

void MapManager::generateNavmeshes(EntityManager& em)
{
    auto& navs = em.getSingleton<NavmeshInfo>();
    mapType navmeshkaiwa = loadMap("assets/levels/navmeshtest/Lvl_2-navmeshes.kaiwa");

    const valueType& navmeshes = navmeshkaiwa["NavMesh"];
    for (mapSizeType i = 0; i < navmeshes.Size(); i++) {
        std::array<vec3d, 9> vecnodes{};

        //Obtenemos el centro
        const valueType& navmesh = navmeshes[i];
        vecnodes[0] = { navmesh["center"][1].GetDouble(), navmesh["center"][2].GetDouble(), navmesh["center"][0].GetDouble() };

        //Recorremos vertices
        vec3d min{}, max{};
        const valueType& vertices = navmesh["vertices"];
        for (mapSizeType j = 0; j < vertices.Size(); j++)
        {
            vecnodes[j + 1] = { vertices[j][1].GetDouble(),  vertices[j][2].GetDouble(),  vertices[j][0].GetDouble() };
            if (j == 0)
            {
                max = vecnodes[j + 1];
                min = vecnodes[j + 1];
            }
            else {
                if (max.x() < vecnodes[j + 1].x()) {
                    max.setX(vecnodes[j + 1].x());
                }
                if (max.z() < vecnodes[j + 1].z()) {
                    max.setZ(vecnodes[j + 1].z());
                }
                if (min.x() > vecnodes[j + 1].x()) {
                    min.setX(vecnodes[j + 1].x());
                }
                if (min.z() > vecnodes[j + 1].z()) {
                    min.setZ(vecnodes[j + 1].z());
                }
            }
        }

        //Recorremos puntos medios
        const valueType& midpoints = navmesh["puntos medios"];
        for (mapSizeType k = 0; k < midpoints.Size(); k++)
        {
            vecnodes[k + 4] = { midpoints[k][1].GetDouble(),  midpoints[k][2].GetDouble(),  midpoints[k][0].GetDouble() };
            navs.midpoints.insert(vecnodes[k + 4]);
        }

        //Creamos NavMesh BBox
        BBox b{ min,max };
        //Creamos NavMesh
        Navmesh nav{ .box = b};
        //Rellenamos los nodos
        for (auto& n : vecnodes) {
            auto pair = std::make_pair(navs.num_nodes, n);
            //Se añade al navinfo
            navs.nodes.insert(pair);
            //si es el centro, se guarda
            if(n.x() == vecnodes[0].x() && n.z() == vecnodes[0].z()){
                nav.centerpoint = pair;
            }
            nav.nodes.insert(pair);
            navs.num_nodes++;
        }
        // Guardamos info
        navs.NavMeshes.push_back(nav);

        //Creamos conexiones de el navmesh
        for (auto it = nav.nodes.begin(); it != std::prev(nav.nodes.end()); ++it) {
            auto& currentNode = it->second;
            auto& nextNode = std::next(it)->second;

            // Comprobamos que no se repitan y que no hayan conexiones que pasen por puntos medios
            if (navs.insert_ids(it->first, std::next(it)->first) &&
                navs.checkmidpoints(currentNode, nextNode)) {
                Conection c{ 1, it->first, std::next(it)->first };
                navs.conexiones.push_back(c);
            }
        }
    }
    //Crear conexiones de navmesh con otros con los que colisiona
    for (auto it = navs.NavMeshes.begin(); it != std::prev(navs.NavMeshes.end()); ++it) {
        auto  nextnavmesh = std::next(it);
        auto& currentbbox = it->box;
        auto& nextbbox = nextnavmesh->box;
        //Conexiones con navmeshes colisionables
        if (currentbbox.intersects(nextbbox)) {
            //Recorremos nodos del primer navmesh
            for (auto it2 = it->nodes.begin(); it2 != std::prev(it->nodes.end()); ++it2) {
                const auto& currentNode = it2;
                //Recorremos nodos del segundo navmesh
                for (auto it3 = nextnavmesh->nodes.begin(); it3 != std::prev(nextnavmesh->nodes.end()); ++it3) {
                    const auto& nextNode = it3;
                    //Comprobamos que no se repitan y que no hayan conexiones que pasen por puntos medios
                    if (navs.insert_ids(currentNode->first, nextNode->first)) {
                        Conection c{ 1, currentNode->first, nextNode->first };
                        navs.conexiones.push_back(c);
                    }
                }
            }
        }
    }

    //Creamos Grafo
    graph.createGraph(navs.conexiones, navs.nodes);
}

void MapManager::destroyMap(EntityManager& em)
{
    auto& li = em.getSingleton<LevelInfo>();

    for (auto e : em.getEntities())
    {
        if (e.getID() != li.playerID)
            li.insertDeath(e.getID());
    }
}

void MapManager::addToZone(EntityManager& em, Entity& e, InteractableType type)
{
    auto& zchi = em.getSingleton<ZoneCheckInfo>();
    auto& c = em.getComponent<ColliderComponent>(e);
    for (auto [num, bbox] : zoneBounds)
        if (bbox.intersects(c.boundingBox))
        {
            zchi.insertZone(num, type);

            if (e.hasComponent<OneUseComponent>())
            {
                auto& ouc = em.getComponent<OneUseComponent>(e);
                ouc.zone = num;
            }
        }
}

void MapManager::spawnReset(EntityManager& em, Ia_man& iam)
{
    if (!reSpawn)
    {
        using TAGs = MP::TypeList<EnemyTag>;
        using TAGs2 = MP::TypeList<NPCTag>;
        destroyParts<TAGs>(em);
        destroyParts<TAGs2>(em);
        reSpawn = true;
    }
    else
    {
        const valueType& chunks = map["Chunks"];
        for (mapSizeType i = 0; i < chunks.Size(); i++)
        {
            std::string chunkName = "Chunk" + std::to_string(i);
            const valueType& chunk = chunks[i][chunkName.c_str()];
            const valueType& enemyArray = chunk[1]["underworld"]["Enemies"];
            const valueType& npcArray = chunk[1]["underworld"]["NPCs"];

            generateEnemies(em, enemyArray, iam);
            generateNPCs(em, npcArray);
        }
        reSpawn = false;
    }
}

void MapManager::addMessageCmp(EntityManager& em, Entity& e, const valueType& value)
{
    std::queue<std::string> messages;
    for (mapSizeType j = 0; j < value["message"].Size(); j++)
    {
        messages.emplace(value["message"][j].GetString());
    }
    em.addComponent<MessageComponent>(e, MessageComponent{ .messages = messages });
}

// template<>
// void MapManager::resetTag<WallTag>(EntityManager& em)
// {
//     mapType map = loadMap(fileMap);

//     const valueType& chunks = map["Chunks"];
//     for (mapSizeType i = 0; i < chunks.Size(); i++)
//     {
//         std::string chunkName = "Chunk" + std::to_string(i);
//         const valueType& chunk = chunks[i][chunkName.c_str()];
//         const valueType& wallArray = chunk[0]["overworld"]["Walls"];

//         generateWalls(em, wallArray);
//     }
// }

// template<>
// void MapManager::resetTag<RampTag>(EntityManager& em)
// {
//     mapType map = loadMap(fileMap);

//     const valueType& chunks = map["Chunks"];
//     for (mapSizeType i = 0; i < chunks.Size(); i++)
//     {
//         std::string chunkName = "Chunk" + std::to_string(i);
//         const valueType& chunk = chunks[i][chunkName.c_str()];
//         const valueType& rampArray = chunk[0]["overworld"]["Ramps"];

//         generateWalls(em, rampArray);
//     }
// }

// template<>
// void MapManager::resetTag<DestructibleTag>(EntityManager& em)
// {
//     mapType map = loadMap(fileMap);

//     const valueType& chunks = map["Chunks"];
//     for (mapSizeType i = 0; i < chunks.Size(); i++)
//     {
//         std::string chunkName = "Chunk" + std::to_string(i);
//         const valueType& chunk = chunks[i][chunkName.c_str()];
//         const valueType& destructibleArray = chunk[0]["overworld"]["Destroyables"];

//         generateDestructibles(em, destructibleArray);
//     }
// }

// template<>
// void MapManager::resetTag<EnemyTag>(EntityManager& em, Ia_man& iam)
// {
//     mapType map = loadMap(fileMap);

//     const valueType& chunks = map["Chunks"];
//     for (mapSizeType i = 0; i < chunks.Size(); i++)
//     {
//         std::string chunkName = "Chunk" + std::to_string(i);
//         const valueType& chunk = chunks[i][chunkName.c_str()];
//         const valueType& enemyArray = chunk[1]["underworld"]["Enemies"];

//         generateEnemies(em, enemyArray, iam);
//     }
// }

// template<>
// void MapManager::resetTag<ObjectTag>(EntityManager& em)
// {
//     mapType map = loadMap(fileMap);
//     auto& li = em.getSingleton<LevelInfo>();

//     const valueType& chunks = map["Chunks"];
//     for (mapSizeType i = 0; i < chunks.Size(); i++)
//     {
//         std::string chunkName = "Chunk" + std::to_string(i);
//         const valueType& chunk = chunks[i][chunkName.c_str()];
//         const valueType& objectsArray = chunk[1]["underworld"]["Objects"];

//         generateObjects(em, objectsArray, li.mapID);
//     }
// }

void MapManager::reset(EntityManager& em, uint8_t mapID, Ia_man& iam)
{
    destroyMap(em);
    iam.resetVec();
    auto& li = em.getSingleton<LevelInfo>();
    auto& zchi = em.getSingleton<ZoneCheckInfo>();

    li.mapID = mapID;
    li.levelChanged = true;
    li.loading = false;
    state = LoadState::LOAD_CHUNKS;
    zchi.clearSets();
    zoneBounds.clear();
    chunksVec.clear();
}

void MapManager::changeMap(EntityManager& em, uint8_t mapID, Ia_man& iam)
{
    reset(em, mapID, iam);
    auto& li = em.getSingleton<LevelInfo>();
    li.loading = true;
    unique_ids = 0;
}

void MapManager::checkDispatcher(EntityManager& em, Entity& e, const valueType& value)
{
    if (value.HasMember("events"))
    {
        auto& dc = em.addComponent<DispatcherComponent>(e);
        for (mapSizeType j = 0; j < value["events"].Size(); j++)
        {
            dc.eventCodes.push_back(value["events"][j].GetInt());
        }
    }
}
