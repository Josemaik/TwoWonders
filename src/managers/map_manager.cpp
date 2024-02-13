#include "map_manager.hpp"
#include <fstream>

void MapManager::createMap(EntityManager& em, uint8_t mapID, Ia_man& iam) {

    mapType map{};
    auto& li = em.getSingleton<LevelInfo>();
    iam.resetVec();

    switch (mapID)
    {
    case 0:
        li.mapID = 0;
        map = loadMap("assets/levels/bin_maps/Zona_0-Bosque.kaiwa");
        break;

    case 1:
        li.mapID = 1;
        map = loadMap("assets/levels/mazmorra_level.json");
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
        generateChunkFromJSON(em, chunk, iam, mapID, j);
    }
}

void MapManager::generateChunkFromJSON(EntityManager& em, const rapidjson::Value& chunk, Ia_man& iam, uint8_t mapID, int& j)
{
    const rapidjson::Value& overworld = chunk[0]["overworld"];
    const rapidjson::Value& groundArray = overworld["Ground"];
    const rapidjson::Value& wallArray = overworld["Walls"];
    const rapidjson::Value& rampArray = overworld["Ramps"];
    const rapidjson::Value& destructibleArray = overworld["Destroyables"];
    const rapidjson::Value& underworld = chunk[1]["underworld"];
    const rapidjson::Value& objectArray = underworld["Objects"];
    const rapidjson::Value& enemyArray = underworld["Enemies"];
    const rapidjson::Value& interactablesArray = underworld["Interactives"];

    generateGround(em, groundArray, j);

    generateWalls(em, wallArray);

    generateRamps(em, rampArray);

    generateDestructibles(em, destructibleArray);

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

void MapManager::generateGround(EntityManager& em, const rapidjson::Value& groundArray, int& j)
{
    for (rapidjson::SizeType i = 0; i < groundArray.Size(); i++)
    {
        auto& groundEntity = em.newEntity();
        em.addTag<GroundTag>(groundEntity);

        // Extraemos los datos del json
        const rapidjson::Value& ground = groundArray[i];
        vec3d groundPosition{ ground["position"][0].GetDouble(), ground["position"][2].GetDouble(), -ground["position"][1].GetDouble() };
        vec3d groundScale{ ground["scale"][0].GetDouble(), ground["scale"][2].GetDouble(), ground["scale"][1].GetDouble() };
        vec3d rotationVec{ ground["rotation"][0].GetDouble(), ground["rotation"][2].GetDouble(), ground["rotation"][1].GetDouble() };
        double orientation{ ground["orientation"].GetDouble() };
        Color color{ 255, 255, 255, 255 };

        // Creamos los componentes del suelo
        auto& r = em.addComponent<RenderComponent>(groundEntity, RenderComponent{ .position = groundPosition, .scale = groundScale, .color = color, .rotationVec = rotationVec });
        auto& p = em.addComponent<PhysicsComponent>(groundEntity, PhysicsComponent{ .position = r.position, .velocity = vec3d::zero(), .scale = r.scale, .gravity = .0, .orientation = orientation * DEGTORAD, .rotationVec = r.rotationVec });
        em.addComponent<ColliderComponent>(groundEntity, ColliderComponent{ p.position, r.scale, BehaviorType::STATIC });

        if (j < 4)
        {
            r.model = LoadModel("assets/models/map_models/lvl_0-cnk0.obj");
            Texture2D t = LoadTexture("assets/models/textures/map_textures/lvl0_texture.png");
            r.model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = t;

            r.meshLoaded = true;
            groundPos = groundPosition;
        }
        else
        {
            auto& modelEntity = em.newEntity();
            auto& r2 = em.addComponent<RenderComponent>(modelEntity, RenderComponent{ .position = groundPos, .scale = groundScale, .color = color, .rotationVec = rotationVec });
            r2.model = LoadModel("assets/models/map_models/lvl_0-cnk1.obj");
            Texture2D t = LoadTexture("assets/models/textures/map_textures/lvl0_texture.png");
            r2.model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = t;

            r2.meshLoaded = true;
        }

        // Creamos las 4 zonas
        vec3d zoneScale = groundScale / 2; // Dividimos la escala del suelo por 2
        zoneScale.setY(2); // Elevamos la zona en 2 unidades en y
        int k = 0;
        int limit = j + 4;
        for (; j < limit; j++)
        {
            auto& zoneEntity = em.newEntity();
            em.addTag<ZoneTag>(zoneEntity);

            // Ajustamos la posición de la zona
            vec3d zonePosition = groundPosition;
            zonePosition.setX(groundPosition.x() + (k % 2 - 0.5) * zoneScale.x());
            zonePosition.setZ(groundPosition.z() + (k / 2 - 0.5) * zoneScale.z());
            zonePosition.setY(groundPosition.y() + 2); // Elevamos la zona en 2 unidades en y

            // Creamos los componentes de la zona
            em.addComponent<ZoneComponent>(zoneEntity, ZoneComponent{ .zone = static_cast<uint16_t>(j) });
            auto& r = em.addComponent<RenderComponent>(zoneEntity, RenderComponent{ .position = zonePosition, .scale = zoneScale, .visible = false });
            auto& p = em.addComponent<PhysicsComponent>(zoneEntity, PhysicsComponent{ .position = r.position, .velocity = vec3d::zero(), .scale = r.scale, .gravity = .0 });
            em.addComponent<ColliderComponent>(zoneEntity, ColliderComponent{ p.position, r.scale, BehaviorType::ZONE });
            k += 1;
        }

        if (!r.meshLoaded)
            em.destroyComponent<RenderComponent>(groundEntity);
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
        vec3d position{ wall["position"][0].GetDouble(), wall["position"][2].GetDouble(), -wall["position"][1].GetDouble() };
        vec3d scale{ wall["scale"][0].GetDouble(), wall["scale"][2].GetDouble(), wall["scale"][1].GetDouble() };
        vec3d rotationVec{ wall["rotation"][0].GetDouble(), wall["rotation"][2].GetDouble(), wall["rotation"][1].GetDouble() };
        double orientation{ wall["orientation"].GetDouble() };
        Color color{ LIME };

        // Creamos los componentes
        auto& r = em.addComponent<RenderComponent>(entity, RenderComponent{ .position = position, .scale = scale, .color = color,.visible = false, .rotationVec = rotationVec });
        auto& p = em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = r.position, .velocity = vec3d::zero(), .scale = r.scale, .gravity = .0, .orientation = orientation * DEGTORAD, .rotationVec = r.rotationVec });
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
        vec3d position{ ramp["position"][0].GetDouble(), ramp["position"][2].GetDouble(), ramp["position"][1].GetDouble() };
        vec3d scale{ ramp["scale"][0].GetDouble(), ramp["scale"][2].GetDouble(), ramp["scale"][1].GetDouble() };
        Color color{ BEIGE };
        float orientation{ ramp["rotation"].GetFloat() };
        vec3d rotationVec{ ramp["rotVector"][0].GetDouble(), ramp["rotVector"][2].GetDouble(), ramp["rotVector"][1].GetDouble() };
        vec2d min{ ramp["min"][0].GetDouble(), ramp["min"][1].GetDouble() };
        vec2d max{ ramp["max"][0].GetDouble(), ramp["max"][1].GetDouble() };
        double slope{ ramp["slope"].GetDouble() };
        vec2d offset{ ramp["offset"][0].GetDouble(), ramp["offset"][1].GetDouble() };

        // Creamos los componentes
        auto& r = em.addComponent<RenderComponent>(entity, RenderComponent{ .position = position, .scale = scale, .color = color, .rotationVec = rotationVec });
        em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = r.position, .velocity = vec3d::zero(), .scale = r.scale, .gravity = .0, .orientation = orientation * DEGTORAD, .rotationVec = rotationVec });
        em.addComponent<RampComponent>(entity, RampComponent{ .min = min, .max = max, .slope = slope, .offset = offset });
    }
}

void MapManager::generateDestructibles(EntityManager& em, const rapidjson::Value& destructibleArray)
{
    for (rapidjson::SizeType i = 0; i < destructibleArray.Size(); i++)
    {
        auto& entity = em.newEntity();
        em.addTag<DestructibleTag>(entity);

        // Extraemos los datos del json
        const rapidjson::Value& destructible = destructibleArray[i];
        vec3d position{ destructible["position"][0].GetDouble(), destructible["position"][2].GetDouble(), -destructible["position"][1].GetDouble() };
        vec3d scale{ destructible["scale"][0].GetDouble(), destructible["scale"][2].GetDouble(), destructible["scale"][1].GetDouble() };
        vec3d rotationVec{ destructible["rotation"][0].GetDouble(), destructible["rotation"][2].GetDouble(), destructible["rotation"][1].GetDouble() };
        double orientation{ destructible["orientation"].GetDouble() };
        Color color{ BROWN };
        int life{ destructible["life"].GetInt() };

        // Creamos los componentes
        auto& r = em.addComponent<RenderComponent>(entity, RenderComponent{ .position = position, .scale = scale, .color = color, .rotationVec = rotationVec });
        auto& p = em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = r.position, .velocity = vec3d::zero(), .scale = r.scale, .gravity = .0, .orientation = orientation * DEGTORAD, .rotationVec = r.rotationVec });
        em.addComponent<ColliderComponent>(entity, ColliderComponent{ p.position, r.scale, BehaviorType::STATIC });
        em.addComponent<LifeComponent>(entity, LifeComponent{ .life = life });
        auto& d = em.addComponent<DestructibleComponent>(entity);

        for (rapidjson::SizeType j = 0; j < destructible["weaknesses"].Size(); j++)
        {
            auto& weakness = destructible["weaknesses"][j];
            d.addWeakness(static_cast<ElementalType>(weakness.GetInt()));
        }
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
        vec3d position{ interactable["position"][0].GetDouble(), interactable["position"][2].GetDouble(), -interactable["position"][1].GetDouble() };
        vec3d scale{ interactable["scale"][0].GetDouble(), interactable["scale"][2].GetDouble(), interactable["scale"][1].GetDouble() };
        vec3d rotationVec{ interactable["rotation"][0].GetDouble(), interactable["rotation"][2].GetDouble(), interactable["rotation"][1].GetDouble() };
        double orientation{ interactable["orientation"].GetDouble() };
        Color color{ LIGHTGRAY };
        InteractableType type{ static_cast<InteractableType>(interactable["type"].GetInt()) };

        // Creamos los componentes
        auto& entity = em.newEntity();
        auto& r = em.addComponent<RenderComponent>(entity, RenderComponent{ .position = position, .scale = scale, .color = color, .rotationVec = rotationVec });
        auto& p = em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = r.position, .velocity = vec3d::zero(), .scale = r.scale, .orientation = orientation * DEGTORAD, .rotationVec = r.rotationVec });
        em.addComponent<ColliderComponent>(entity, ColliderComponent{ p.position, r.scale, BehaviorType::STATIC });
        em.addComponent<InteractiveComponent>(entity);

        switch (type)
        {
        case InteractableType::Chest:
        {
            em.addTag<ChestTag>(entity);
            uint16_t zone = static_cast<uint16_t>(interactable["zone"].GetUint());
            ObjectType content{ static_cast<ObjectType>(interactable["content"].GetInt()) };
            uint8_t interId = static_cast<uint8_t>(interactable["id"].GetUint());

            em.addComponent<ChestComponent>(entity, ChestComponent{ .id = interId, .zone = zone, .dropPosition = { vec3d::zero() }, .content = content });
            break;
        }
        case InteractableType::Spawn:
        {
            em.addTag<SpawnTag>(entity);
            r.color = ORANGE;
            break;
        }
        case InteractableType::Door:
        {
            em.addTag<DoorTag>(entity);
            r.color = DARKBROWN;
            break;
        }
        }
    }
}

void MapManager::destroyMap(EntityManager& em)
{
    using CMPS = MP::TypeList<>;
    using TAGS = MP::TypeList<GroundTag, WaterTag, WallTag, ZoneTag, RampTag, DoorTag, ObjectTag, EnemyTag, DestructibleTag, ChestTag>;
    auto& li = em.getSingleton<LevelInfo>();

    em.forEachAny<CMPS, TAGS>([&](Entity& entity) { li.dead_entities.insert(entity.getID()); });
}

void MapManager::spawnReset(EntityManager& em, Ia_man& iam)
{
    using TAGs = MP::TypeList <EnemyTag, DestructibleTag>;
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

    for (rapidjson::SizeType i = 0; i < chunks.Size(); i++)
    {
        std::string chunkName = "Chunk" + std::to_string(i);
        const rapidjson::Value& chunk = chunks[i][chunkName.c_str()];
        const rapidjson::Value& destructibleArray = chunk[0]["overworld"]["Destroyables"];

        generateDestructibles(em, destructibleArray);
    }
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

void MapManager::reset(EntityManager& em, uint8_t mapID, Ia_man& iam)
{
    destroyMap(em);
    createMap(em, mapID, iam);
}
