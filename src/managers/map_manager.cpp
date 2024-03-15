#include "map_manager.hpp"
#include <fstream>

void MapManager::createMap(EntityManager& em, uint8_t mapID, Ia_man& iam) {

    mapType map{};
    auto& li = em.getSingleton<LevelInfo>();
    li.levelChanged = false;
    iam.resetVec();

    switch (mapID)
    {
    case 0:
        li.mapID = 0;
        map = loadMap("assets/levels/maps/lvl_0.kaiwa");
        break;

    case 1:
        li.mapID = 1;
        map = loadMap("assets/levels/maps/lvl_1.kaiwa");
        break;

    case 2:
        li.mapID = 2;
        map = loadMap("assets/levels/caves_level.json");
        break;

    default:
        break;
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
    const rapidjson::Value& id = map["id"];
    uint8_t mapID = static_cast<uint8_t>(id.GetUint());

    const rapidjson::Value& chunks = map["Chunks"];
    int j = 0;

    for (rapidjson::SizeType i = 0; i < chunks.Size(); i++)
    {
        std::string chunkName = "Chunk" + std::to_string(i);
        const rapidjson::Value& chunk = chunks[i][chunkName.c_str()];
        generateChunkFromJSON(em, chunk, iam, mapID, i, j);
    }
}

void MapManager::generateChunkFromJSON(EntityManager& em, const rapidjson::Value& chunk, Ia_man& iam, uint8_t mapID, rapidjson::SizeType& i, int& j)
{
    const rapidjson::Value& overworld = chunk[0]["overworld"];
    const rapidjson::Value& groundArray = overworld["Ground"];
    const rapidjson::Value& wallArray = overworld["Walls"];
    const rapidjson::Value& rampArray = overworld["Ramps"];
    const rapidjson::Value& interactablesArray = overworld["Interactive"];
    const rapidjson::Value& underworld = chunk[1]["underworld"];
    const rapidjson::Value& objectArray = underworld["Objects"];
    const rapidjson::Value& enemyArray = underworld["Enemies"];

    generateChunkModel(em, i);

    generateGround(em, groundArray, j);

    generateWalls(em, wallArray);

    generateRamps(em, rampArray);

    generateObjects(em, objectArray, mapID);

    generateEnemies(em, enemyArray, iam);

    generateInteractables(em, interactablesArray);

    // pseudo codigo para scheduling xdddd
    // for enemy : map[enemies]
    // {

    //     if(tiempo pasado > x)
    //     {
    //         texturas enemigo = map[enemies][slime][texturas][1];
    //     }
    //     else
    //     {
    //         texturas enemigo = map[enemies][slime][texturas][0];
    //     }
    // }
}

void MapManager::generateChunkModel(EntityManager& em, rapidjson::SizeType& i)
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

void MapManager::generateGround(EntityManager& em, const rapidjson::Value& groundArray, int& j)
{
    vec2d min{}, max{};
    double minHeigth = 0.0;
    for (rapidjson::SizeType i = 0; i < groundArray.Size(); i++)
    {
        auto& groundEntity = em.newEntity();
        em.addTag<GroundTag>(groundEntity);

        // Extraemos los datos del json
        const rapidjson::Value& ground = groundArray[i];
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

    // Creamos 4 zonas para el chunk
    vec3d zoneScale = { (max.x - min.x) / 2, 20, (max.y - min.y) / 2 };
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

void MapManager::generateWalls(EntityManager& em, const rapidjson::Value& wallArray)
{
    for (rapidjson::SizeType i = 0; i < wallArray.Size(); i++)
    {
        auto& entity = em.newEntity();
        em.addTag<WallTag>(entity);
        // Extraemos los datos del json
        const rapidjson::Value& wall = wallArray[i];
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

void MapManager::generateRamps(EntityManager& em, const rapidjson::Value& rampArray)
{
    for (rapidjson::SizeType i = 0; i < rampArray.Size(); i++)
    {
        auto& entity = em.newEntity();
        em.addTag<RampTag>(entity);

        // Extraemos los datos del json
        const rapidjson::Value& ramp = rampArray[i];
        vec2d min{ ramp["min"][1].GetDouble(), ramp["min"][0].GetDouble() };
        vec2d max{ ramp["max"][1].GetDouble(), ramp["max"][0].GetDouble() };
        vec3d offset{ ramp["offset"][1].GetDouble(), ramp["offset"][2].GetDouble(), ramp["offset"][0].GetDouble() };
        double slope{ ramp["slope"].GetDouble() };
        RampType type{ static_cast<RampType>(ramp["type"].GetUint()) };

        // Creamos las dimensiones de la rampa y su posición
        vec3d rmin = { min.x, offset.y() - 20, min.y };
        vec3d rmax = { max.x, offset.y() + 20, max.y };

        vec3d rampPos = (rmin + rmax) / 2;
        vec3d rampSize = rmax - rmin;

        // Creamos los componentes
        em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = rampPos, .scale = rampSize, .gravity = 0 });
        em.addComponent<ColliderComponent>(entity, ColliderComponent{ rampPos, rampSize, BehaviorType::RAMP });
        em.addComponent<RampComponent>(entity, RampComponent{ .min = min, .max = max, .slope = slope, .offset = offset, .type = type });
    }
}

void MapManager::generateObjects(EntityManager& em, const rapidjson::Value& objectArray, uint8_t mapID)
{
    for (rapidjson::SizeType i = 0; i < objectArray.Size(); i++)
    {
        auto& li = em.getSingleton<LevelInfo>();
        const rapidjson::Value& obj = objectArray[i];
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

void MapManager::generateEnemies(EntityManager& em, const rapidjson::Value& enemyArray, Ia_man& iam)
{
    for (rapidjson::SizeType i = 0; i < enemyArray.Size(); i++)
    {
        const rapidjson::Value& enemy = enemyArray[i];
        iam.createEnemy(em, enemy);
    }
}

void MapManager::generateInteractables(EntityManager& em, const rapidjson::Value& interactablesArray)
{
    for (rapidjson::SizeType i = 0; i < interactablesArray.Size(); i++)
    {
        const rapidjson::Value& interactable = interactablesArray[i];

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
            uint16_t zone = static_cast<uint16_t>(interactable["zone"].GetUint());
            ObjectType content{ static_cast<ObjectType>(interactable["content"].GetInt()) };
            uint8_t interId = static_cast<uint8_t>(interactable["id"].GetUint());

            std::queue<std::string> messages;
            for (rapidjson::SizeType j = 0; j < interactable["message"].Size(); j++)
            {
                messages.emplace(interactable["message"][j].GetString());
            }

            em.addComponent<OneUseComponent>(entity, OneUseComponent{ .id = interId, .zone = zone });
            [[maybe_unused]] auto& cc = em.addComponent<ChestComponent>(entity, ChestComponent{ .dropPosition = { vec3d::zero() }, .content = content, .messages = messages });

            if (interactable.HasMember("offsetZ"))
            {
                r.offset = interactable["offsetZ"][0].GetDouble();
            }

            if (interactable.HasMember("events"))
            {
                auto& dc = em.addComponent<DispatcherComponent>(entity);
                for (rapidjson::SizeType j = 0; j < interactable["events"].Size(); j++)
                {
                    dc.eventCodes.push_back(interactable["events"][j].GetInt());
                }
            }
            break;
        }
        case InteractableType::Destructible:
        {
            em.addTag<DestructibleTag>(entity);
            em.addTag<WallTag>(entity);
            int life{ interactable["life"].GetInt() };
            em.addComponent<LifeComponent>(entity, LifeComponent{ .life = life });
            auto& d = em.addComponent<DestructibleComponent>(entity);

            for (rapidjson::SizeType j = 0; j < interactable["weaknesses"].Size(); j++)
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

            break;
        }
        case InteractableType::Level:
        {
            r.visible = false;
            c.behaviorType = BehaviorType::ZONE;
            em.addComponent<ZoneComponent>(entity, ZoneComponent{ .zone = static_cast<uint16_t>(interactable["zone"].GetUint()) });
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
            break;
        }
        case InteractableType::Lever:
        {
            em.addTag<WallTag>(entity);
            em.addTag<LeverTag>(entity);
            uint16_t zone = static_cast<uint16_t>(interactable["zone"].GetUint());
            uint8_t interId = static_cast<uint8_t>(interactable["id"].GetUint());

            em.addComponent<OneUseComponent>(entity, OneUseComponent{ .id = interId, .zone = zone });
            break;
        }
        }
        addToZone(em, c.boundingBox, type);
    }
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

void MapManager::spawnReset(EntityManager& em, Ia_man& iam)
{
    using TAGs = MP::TypeList <EnemyTag>;
    destroyParts<TAGs>(em);

    mapType map = loadMap(fileMap);

    const rapidjson::Value& chunks = map["Chunks"];
    for (rapidjson::SizeType i = 0; i < chunks.Size(); i++)
    {
        std::string chunkName = "Chunk" + std::to_string(i);
        const rapidjson::Value& chunk = chunks[i][chunkName.c_str()];
        const rapidjson::Value& enemyArray = chunk[1]["underworld"]["Enemies"];

        generateEnemies(em, enemyArray, iam);
    }

    // for (rapidjson::SizeType i = 0; i < chunks.Size(); i++)
    // {
    //     std::string chunkName = "Chunk" + std::to_string(i);
    //     const rapidjson::Value& chunk = chunks[i][chunkName.c_str()];
    //     const rapidjson::Value& destructibleArray = chunk[0]["overworld"]["Destroyables"];

    //     generateDestructibles(em, destructibleArray);
    // }
}

void MapManager::addToZone(EntityManager& em, BBox& b, InteractableType type)
{
    auto& zchi = em.getSingleton<ZoneCheckInfo>();
    for (auto [num, bbox] : zoneBounds)
        if (bbox.intersects(b))
            zchi.insertZone(num, type);
}

// template<>
// void MapManager::resetTag<WallTag>(EntityManager& em)
// {
//     mapType map = loadMap(fileMap);

//     const rapidjson::Value& chunks = map["Chunks"];
//     for (rapidjson::SizeType i = 0; i < chunks.Size(); i++)
//     {
//         std::string chunkName = "Chunk" + std::to_string(i);
//         const rapidjson::Value& chunk = chunks[i][chunkName.c_str()];
//         const rapidjson::Value& wallArray = chunk[0]["overworld"]["Walls"];

//         generateWalls(em, wallArray);
//     }
// }

// template<>
// void MapManager::resetTag<RampTag>(EntityManager& em)
// {
//     mapType map = loadMap(fileMap);

//     const rapidjson::Value& chunks = map["Chunks"];
//     for (rapidjson::SizeType i = 0; i < chunks.Size(); i++)
//     {
//         std::string chunkName = "Chunk" + std::to_string(i);
//         const rapidjson::Value& chunk = chunks[i][chunkName.c_str()];
//         const rapidjson::Value& rampArray = chunk[0]["overworld"]["Ramps"];

//         generateWalls(em, rampArray);
//     }
// }

// template<>
// void MapManager::resetTag<DestructibleTag>(EntityManager& em)
// {
//     mapType map = loadMap(fileMap);

//     const rapidjson::Value& chunks = map["Chunks"];
//     for (rapidjson::SizeType i = 0; i < chunks.Size(); i++)
//     {
//         std::string chunkName = "Chunk" + std::to_string(i);
//         const rapidjson::Value& chunk = chunks[i][chunkName.c_str()];
//         const rapidjson::Value& destructibleArray = chunk[0]["overworld"]["Destroyables"];

//         generateDestructibles(em, destructibleArray);
//     }
// }

// template<>
// void MapManager::resetTag<EnemyTag>(EntityManager& em, Ia_man& iam)
// {
//     mapType map = loadMap(fileMap);

//     const rapidjson::Value& chunks = map["Chunks"];
//     for (rapidjson::SizeType i = 0; i < chunks.Size(); i++)
//     {
//         std::string chunkName = "Chunk" + std::to_string(i);
//         const rapidjson::Value& chunk = chunks[i][chunkName.c_str()];
//         const rapidjson::Value& enemyArray = chunk[1]["underworld"]["Enemies"];

//         generateEnemies(em, enemyArray, iam);
//     }
// }

// template<>
// void MapManager::resetTag<ObjectTag>(EntityManager& em)
// {
//     mapType map = loadMap(fileMap);
//     auto& li = em.getSingleton<LevelInfo>();

//     const rapidjson::Value& chunks = map["Chunks"];
//     for (rapidjson::SizeType i = 0; i < chunks.Size(); i++)
//     {
//         std::string chunkName = "Chunk" + std::to_string(i);
//         const rapidjson::Value& chunk = chunks[i][chunkName.c_str()];
//         const rapidjson::Value& objectsArray = chunk[1]["underworld"]["Objects"];

//         generateObjects(em, objectsArray, li.mapID);
//     }
// }

void MapManager::reset(EntityManager& em, uint8_t mapID, Ia_man&)
{
    destroyMap(em);
    auto& li = em.getSingleton<LevelInfo>();
    auto& zchi = em.getSingleton<ZoneCheckInfo>();

    li.mapID = mapID;
    li.levelChanged = true;
    zchi.clearSets();
    // createMap(em, mapID, iam);
}
