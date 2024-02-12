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
    for (rapidjson::SizeType i = 0; i < chunks.Size() - 1; i++)
    {
        std::string chunkName = "Chunk" + std::to_string(i);
        const rapidjson::Value& chunk = chunks[i][chunkName.c_str()];
        generateChunkFromJSON(em, chunk, iam, mapID);
    }
}

void MapManager::generateChunkFromJSON(EntityManager& em, const rapidjson::Value& chunk, Ia_man& iam, uint8_t mapID)
{
    const rapidjson::Value& overworld = chunk[0]["overworld"];
    const rapidjson::Value& groundArray = overworld["Ground"];
    const rapidjson::Value& wallArray = overworld["Walls"];
    const rapidjson::Value& zoneArray = overworld["Zones"];
    const rapidjson::Value& rampArray = overworld["Ramps"];
    const rapidjson::Value& doorArray = overworld["Doors"];
    const rapidjson::Value& destructibleArray = overworld["Destroyables"];
    const rapidjson::Value& underworld = chunk[1]["underworld"];
    const rapidjson::Value& objectArray = underworld["Objects"];
    const rapidjson::Value& enemyArray = underworld["Enemies"];
    const rapidjson::Value& interactablesArray = underworld["Interactives"];

    for (rapidjson::SizeType i = 0; i < groundArray.Size(); i++)
    {
        auto& entity = em.newEntity();
        em.addTag<GroundTag>(entity);

        // Extraemos los datos del json
        const rapidjson::Value& ground = groundArray[i];
        vec3d position{ ground["position"][0].GetDouble(), ground["position"][2].GetDouble(), ground["position"][1].GetDouble() };
        vec3d scale{ ground["scale"][0].GetDouble(), ground["scale"][2].GetDouble(), ground["scale"][1].GetDouble() };
        vec3d rotationVec{ ground["rotation"][0].GetDouble(), ground["rotation"][2].GetDouble(), ground["rotation"][1].GetDouble() };
        double orientation{ ground["orientation"].GetDouble() };
        Color color{ BEIGE };

        // Creamos los componentes
        auto& r = em.addComponent<RenderComponent>(entity, RenderComponent{ .position = position, .scale = scale, .color = color, .rotationVec = rotationVec });
        auto& p = em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = r.position, .velocity = vec3d::zero(), .gravity = .0, .orientation = orientation * DEGTORAD, .rotationVec = r.rotationVec });
        em.addComponent<ColliderComponent>(entity, ColliderComponent{ p.position, r.scale, BehaviorType::STATIC });
    }

    for (rapidjson::SizeType i = 0; i < wallArray.Size(); i++)
    {
        auto& entity = em.newEntity();
        em.addTag<WallTag>(entity);
        // Extraemos los datos del json
        const rapidjson::Value& wall = wallArray[i];
        vec3d position{ wall["position"][0].GetDouble(), wall["position"][2].GetDouble(), wall["position"][1].GetDouble() };
        vec3d scale{ wall["scale"][0].GetDouble(), wall["scale"][2].GetDouble(), wall["scale"][1].GetDouble() };
        vec3d rotationVec{ wall["rotation"][0].GetDouble(), wall["rotation"][2].GetDouble(), wall["rotation"][1].GetDouble() };
        double orientation{ wall["orientation"].GetDouble() };
        Color color{ LIME };

        // Creamos los componentes
        auto& r = em.addComponent<RenderComponent>(entity, RenderComponent{ .position = position, .scale = scale, .color = color, .rotationVec = rotationVec });
        auto& p = em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = r.position, .velocity = vec3d::zero(), .gravity = .0, .orientation = orientation * DEGTORAD, .rotationVec = r.rotationVec });
        em.addComponent<ColliderComponent>(entity, ColliderComponent{ p.position, r.scale, BehaviorType::STATIC });
    }

    for (rapidjson::SizeType i = 0; i < zoneArray.Size(); i++)
    {
        auto& entity = em.newEntity();
        em.addTag<ZoneTag>(entity);

        // Extraemos los datos del json
        const rapidjson::Value& zone = zoneArray[i];
        vec3d position{ zone["position"][0].GetDouble(), zone["position"][2].GetDouble(), zone["position"][1].GetDouble() };
        vec3d scale{ zone["scale"][0].GetDouble(), zone["scale"][2].GetDouble(), zone["scale"][1].GetDouble() };
        uint16_t zoneNumber{ static_cast<uint16_t>(zone["zone"].GetUint()) };

        // Creamos los componentes
        em.addComponent<ZoneComponent>(entity, ZoneComponent{ .zone = zoneNumber });
        auto& r = em.addComponent<RenderComponent>(entity, RenderComponent{ .position = position, .scale = scale, .visible = false });
        auto& p = em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = r.position, .velocity = vec3d::zero(), .gravity = .0 });
        em.addComponent<ColliderComponent>(entity, ColliderComponent{ p.position, r.scale, BehaviorType::ZONE });
    }

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
        em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = r.position, .velocity = vec3d::zero(), .gravity = .0, .orientation = orientation * DEGTORAD, .rotationVec = rotationVec });
        em.addComponent<RampComponent>(entity, RampComponent{ .min = min, .max = max, .slope = slope, .offset = offset });
    }

    for (rapidjson::SizeType i = 0; i < doorArray.Size(); i++)
    {
        auto& entity = em.newEntity();
        em.addTag<DoorTag>(entity);

        // Extraemos los datos del json
        const rapidjson::Value& door = doorArray[i];
        vec3d position{ door["position"][0].GetDouble(), door["position"][2].GetDouble(), door["position"][1].GetDouble() };
        vec3d scale{ door["scale"][0].GetDouble(), door["scale"][2].GetDouble(), door["scale"][1].GetDouble() };
        vec3d rotationVec{ door["rotation"][0].GetDouble(), door["rotation"][2].GetDouble(), door["rotation"][1].GetDouble() };
        double orientation{ door["orientation"].GetDouble() };
        Color color{ DARKBROWN };

        // Creamos los componentes
        auto& r = em.addComponent<RenderComponent>(entity, RenderComponent{ .position = position, .scale = scale, .color = color, .rotationVec = rotationVec });
        auto& p = em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = r.position, .velocity = vec3d::zero(), .gravity = .0, .orientation = orientation * DEGTORAD, .rotationVec = r.rotationVec });
        em.addComponent<ColliderComponent>(entity, ColliderComponent{ p.position, r.scale, BehaviorType::STATIC });
    }

    for (rapidjson::SizeType i = 0; i < destructibleArray.Size(); i++)
    {
        // auto& entity = em.newEntity();
        // em.addTag<DestructibleTag>(entity);

        // // Extraemos los datos del json
        // const rapidjson::Value& destructible = destructibleArray[i];
        // vec3d position{ destructible["position"][0].GetDouble(), destructible["position"][2].GetDouble(), destructible["position"][1].GetDouble() };
        // vec3d scale{ destructible["scale"][0].GetDouble(), destructible["scale"][2].GetDouble(), destructible["scale"][1].GetDouble() };
        // vec3d rotationVec{ destructible["rotation"][0].GetDouble(), destructible["rotation"][2].GetDouble(), destructible["rotation"][1].GetDouble() };
        // double orientation{ destructible["orientation"].GetDouble() };
        // Color color{ BROWN };
        // int life{ destructible["life"].GetInt() };

        // // Creamos los componentes
        // auto& r = em.addComponent<RenderComponent>(entity, RenderComponent{ .position = position, .scale = scale, .color = color, .rotationVec = rotationVec });
        // auto& p = em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = r.position, .velocity = vec3d::zero(), .gravity = .0, .orientation = orientation * DEGTORAD, .rotationVec = r.rotationVec });
        // em.addComponent<ColliderComponent>(entity, ColliderComponent{ p.position, r.scale, BehaviorType::STATIC });
        // em.addComponent<LifeComponent>(entity, LifeComponent{ .life = life });
    }

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
        auto& p = em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = r.position, .velocity = vec3d::zero() });
        em.addComponent<ColliderComponent>(entity, ColliderComponent{ p.position, r.scale, BehaviorType::STATIC });
        em.addComponent<ObjectComponent>(entity, ObjectComponent{ .type = type, .inmortal = true, .objID = objId });
    }

    for (rapidjson::SizeType i = 0; i < enemyArray.Size(); i++)
    {
        const rapidjson::Value& enemy = enemyArray[i];
        iam.createEnemy(em, enemy);
    }

    for (rapidjson::SizeType i = 0; i < interactablesArray.Size(); i++)
    {
        // const rapidjson::Value& interactable = interactablesArray[i];

        // // Extraemos los datos del json
        // vec3d position{ interactable["position"][0].GetDouble(), interactable["position"][2].GetDouble(), interactable["position"][1].GetDouble() };
        // vec3d scale{ interactable["scale"][0].GetDouble(), interactable["scale"][2].GetDouble(), interactable["scale"][1].GetDouble() };
        // vec3d rotationVec{ interactable["rotation"][0].GetDouble(), interactable["rotation"][2].GetDouble(), interactable["rotation"][1].GetDouble() };
        // double orientation{ interactable["orientation"].GetDouble() };
        // Color color{ LIGHTGRAY };
        // InteractableType type{ static_cast<InteractableType>(interactable["type"].GetInt()) };

        // // Creamos los componentes
        // auto& entity = em.newEntity();
        // auto& r = em.addComponent<RenderComponent>(entity, RenderComponent{ .position = position, .scale = scale, .color = color, .rotationVec = rotationVec });
        // auto& p = em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = r.position, .velocity = vec3d::zero(), .orientation = orientation * DEGTORAD, .rotationVec = r.rotationVec });

        // switch (type)
        // {
        // case 1:
        // {
        //     // Es un cofre
        //     em.addTag<ChestTag>(entity);
        //     uint16_t zone = static_cast<uint16_t>(interactable["zone"].GetUint());
        //     ObjectType content{ static_cast<ObjectType>(interactable["content"].GetInt()) };
        //     uint8_t interId = static_cast<uint8_t>(interactable["id"].GetUint());

        //     em.addComponent<ColliderComponent>(entity, ColliderComponent{ p.position, r.scale, BehaviorType::STATIC });
        //     em.addComponent<ChestComponent>(entity, ChestComponent{ .id = interId, .zone = zone, .dropPosition = { vec3d::zero() }, .content = content });
        //     break;
        // }
        // }
    }

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


void MapManager::destroyMap(EntityManager& em)
{
    using CMPS = MP::TypeList<>;
    using TAGS = MP::TypeList<GroundTag, WaterTag, WallTag, ZoneTag, RampTag, DoorTag, ObjectTag, EnemyTag, DestructibleTag, ChestTag>;
    auto& li = em.getSingleton<LevelInfo>();

    em.forEachAny<CMPS, TAGS>([&](Entity& entity) { li.dead_entities.insert(entity.getID()); });
}

void MapManager::reset(EntityManager& em, uint8_t mapID, Ia_man& iam)
{
    destroyMap(em);
    createMap(em, mapID, iam);
}
