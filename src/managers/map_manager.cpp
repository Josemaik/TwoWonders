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
        map = loadMap("assets/levels/demo_level.json");
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
    const rapidjson::Value& overworld = map["overworld"];
    const rapidjson::Value& groundArray = overworld["ground"];
    const rapidjson::Value& waterArray = overworld["water"];
    const rapidjson::Value& wallArray = overworld["walls"];
    const rapidjson::Value& zoneArray = overworld["zones"];
    const rapidjson::Value& rampArray = overworld["ramps"];
    const rapidjson::Value& doorArray = overworld["doors"];
    const rapidjson::Value& destructibleArray = overworld["destructibles"];
    const rapidjson::Value& underworld = map["underworld"];
    const rapidjson::Value& objectArray = underworld["objects"];
    const rapidjson::Value& enemyArray = underworld["enemies"];
    const rapidjson::Value& interactablesArray = underworld["interactables"];
    const rapidjson::Value& id = map["id"];
    uint8_t mapId = static_cast<uint8_t>(id.GetUint());

    for (rapidjson::SizeType i = 0; i < groundArray.Size(); i++)
    {
        auto& entity = em.newEntity();
        em.addTag<GroundTag>(entity);

        // Extraemos los datos del json
        const rapidjson::Value& ground = groundArray[i];
        vec3d position{ ground["position"][0].GetDouble(), ground["position"][1].GetDouble(), ground["position"][2].GetDouble() };
        vec3d scale{ ground["scale"][0].GetDouble(), ground["scale"][1].GetDouble(), ground["scale"][2].GetDouble() };
        Color color{ static_cast<u_char>(ground["color"][0].GetUint()), static_cast<u_char>(ground["color"][1].GetUint()), static_cast<u_char>(ground["color"][2].GetUint()), static_cast<u_char>(ground["color"][3].GetUint()) };

        // Creamos los componentes
        auto& r = em.addComponent<RenderComponent>(entity, RenderComponent{ .position = position, .scale = scale, .color = color });
        auto& p = em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = r.position, .velocity = vec3d::zero(), .gravity = .0 });
        em.addComponent<ColliderComponent>(entity, ColliderComponent{ p.position, r.scale, BehaviorType::STATIC });
    }

    for (rapidjson::SizeType i = 0; i < waterArray.Size(); i++)
    {
        auto& entity = em.newEntity();
        em.addTag<WaterTag>(entity);

        // Extraemos los datos del json
        const rapidjson::Value& water = waterArray[i];
        vec3d position{ water["position"][0].GetDouble(), water["position"][1].GetDouble(), water["position"][2].GetDouble() };
        vec3d scale{ water["scale"][0].GetDouble(), water["scale"][1].GetDouble(), water["scale"][2].GetDouble() };
        Color color{ static_cast<u_char>(water["color"][0].GetUint()), static_cast<u_char>(water["color"][1].GetUint()), static_cast<u_char>(water["color"][2].GetUint()), static_cast<u_char>(water["color"][3].GetUint()) };

        // Creamos los componentes
        auto& r = em.addComponent<RenderComponent>(entity, RenderComponent{ .position = position, .scale = scale, .color = color });
        auto& p = em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = r.position, .velocity = vec3d::zero(), .gravity = .0 });
        em.addComponent<ColliderComponent>(entity, ColliderComponent{ p.position, r.scale, BehaviorType::STATIC });
    }

    for (rapidjson::SizeType i = 0; i < wallArray.Size(); i++)
    {
        auto& entity = em.newEntity();
        em.addTag<WallTag>(entity);
        // Extraemos los datos del json
        const rapidjson::Value& wall = wallArray[i];
        vec3d position{ wall["position"][0].GetDouble(), wall["position"][1].GetDouble(), wall["position"][2].GetDouble() };
        vec3d scale{ wall["scale"][0].GetDouble(), wall["scale"][1].GetDouble(), wall["scale"][2].GetDouble() };
        Color color{ static_cast<u_char>(wall["color"][0].GetUint()), static_cast<u_char>(wall["color"][1].GetUint()), static_cast<u_char>(wall["color"][2].GetUint()), static_cast<u_char>(wall["color"][3].GetUint()) };

        // Creamos los componentes
        auto& r = em.addComponent<RenderComponent>(entity, RenderComponent{ .position = position, .scale = scale, .color = color });
        auto& p = em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = r.position, .velocity = vec3d::zero(), .gravity = .0 });
        em.addComponent<ColliderComponent>(entity, ColliderComponent{ p.position, r.scale, BehaviorType::STATIC });
    }

    for (rapidjson::SizeType i = 0; i < zoneArray.Size(); i++)
    {
        auto& entity = em.newEntity();
        em.addTag<ZoneTag>(entity);

        // Extraemos los datos del json
        const rapidjson::Value& zone = zoneArray[i];
        vec3d position{ zone["position"][0].GetDouble(), zone["position"][1].GetDouble(), zone["position"][2].GetDouble() };
        vec3d scale{ zone["scale"][0].GetDouble(), zone["scale"][1].GetDouble(), zone["scale"][2].GetDouble() };
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
        vec3d position{ ramp["position"][0].GetDouble(), ramp["position"][1].GetDouble(), ramp["position"][2].GetDouble() };
        vec3d scale{ ramp["scale"][0].GetDouble(), ramp["scale"][1].GetDouble(), ramp["scale"][2].GetDouble() };
        Color color{ static_cast<u_char>(ramp["color"][0].GetUint()), static_cast<u_char>(ramp["color"][1].GetUint()), static_cast<u_char>(ramp["color"][2].GetUint()), static_cast<u_char>(ramp["color"][3].GetUint()) };
        float orientation{ ramp["rotation"].GetFloat() };
        vec3d rotationVec{ ramp["rotVector"][0].GetDouble(), ramp["rotVector"][1].GetDouble(), ramp["rotVector"][2].GetDouble() };
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
        vec3d position{ door["position"][0].GetDouble(), door["position"][1].GetDouble(), door["position"][2].GetDouble() };
        vec3d scale{ door["scale"][0].GetDouble(), door["scale"][1].GetDouble(), door["scale"][2].GetDouble() };
        Color color{ static_cast<u_char>(door["color"][0].GetUint()), static_cast<u_char>(door["color"][1].GetUint()), static_cast<u_char>(door["color"][2].GetUint()), static_cast<u_char>(door["color"][3].GetUint()) };

        // Creamos los componentes
        auto& r = em.addComponent<RenderComponent>(entity, RenderComponent{ .position = position, .scale = scale, .color = color });
        auto& p = em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = r.position, .velocity = vec3d::zero(), .gravity = .0 });
        em.addComponent<ColliderComponent>(entity, ColliderComponent{ p.position, r.scale, BehaviorType::STATIC });
    }

    for (rapidjson::SizeType i = 0; i < destructibleArray.Size(); i++)
    {
        auto& entity = em.newEntity();
        em.addTag<DestructibleTag>(entity);

        // Extraemos los datos del json
        const rapidjson::Value& destructible = destructibleArray[i];
        vec3d position{ destructible["position"][0].GetDouble(), destructible["position"][1].GetDouble(), destructible["position"][2].GetDouble() };
        vec3d scale{ destructible["scale"][0].GetDouble(), destructible["scale"][1].GetDouble(), destructible["scale"][2].GetDouble() };
        Color color{ static_cast<u_char>(destructible["color"][0].GetUint()), static_cast<u_char>(destructible["color"][1].GetUint()), static_cast<u_char>(destructible["color"][2].GetUint()), static_cast<u_char>(destructible["color"][3].GetUint()) };
        int life{ destructible["life"].GetInt() };

        // Creamos los componentes
        auto& r = em.addComponent<RenderComponent>(entity, RenderComponent{ .position = position, .scale = scale, .color = color });
        auto& p = em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = r.position, .velocity = vec3d::zero(), .gravity = .0 });
        em.addComponent<ColliderComponent>(entity, ColliderComponent{ p.position, r.scale, BehaviorType::STATIC });
        em.addComponent<LifeComponent>(entity, LifeComponent{ .life = life });
    }

    for (rapidjson::SizeType i = 0; i < objectArray.Size(); i++)
    {
        auto& li = em.getSingleton<LevelInfo>();
        const rapidjson::Value& obj = objectArray[i];
        uint8_t objId = static_cast<uint8_t>(obj["id"].GetUint());

        std::pair<uint8_t, uint8_t> pair{ mapId, objId };

        if (li.notLoadSet.find(pair) != li.notLoadSet.end())
            continue;

        // Extraemos los datos del json
        vec3d position{ obj["position"][0].GetDouble(), obj["position"][1].GetDouble(), obj["position"][2].GetDouble() };
        vec3d scale{ obj["scale"][0].GetDouble(), obj["scale"][1].GetDouble(), obj["scale"][2].GetDouble() };
        Color color{ static_cast<u_char>(obj["color"][0].GetUint()), static_cast<u_char>(obj["color"][1].GetUint()), static_cast<u_char>(obj["color"][2].GetUint()), static_cast<u_char>(obj["color"][3].GetUint()) };
        ObjectType type{ static_cast<ObjectType>(obj["type"].GetDouble()) };

        auto& entity = em.newEntity();
        em.addTag<ObjectTag>(entity);

        // Creamos los componentes
        auto& r = em.addComponent<RenderComponent>(entity, RenderComponent{ .position = position, .scale = scale, .color = color });
        auto& p = em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = r.position, .velocity = vec3d::zero() });
        em.addComponent<ColliderComponent>(entity, ColliderComponent{ p.position, r.scale, BehaviorType::STATIC });
        em.addComponent<ObjectComponent>(entity, ObjectComponent{ .type = type, .inmortal = true, .mapID = mapId, .objID = objId });
    }

    for (rapidjson::SizeType i = 0; i < enemyArray.Size(); i++)
    {
        const rapidjson::Value& enemy = enemyArray[i];
        iam.createEnemy(em, enemy);
    }

    for (rapidjson::SizeType i = 0; i < interactablesArray.Size(); i++)
    {
        const rapidjson::Value& interactable = interactablesArray[i];

        // Extraemos los datos del json
        vec3d position{ interactable["position"][0].GetDouble(), interactable["position"][1].GetDouble(), interactable["position"][2].GetDouble() };
        vec3d scale{ interactable["scale"][0].GetDouble(), interactable["scale"][1].GetDouble(), interactable["scale"][2].GetDouble() };
        Color color{ static_cast<u_char>(interactable["color"][0].GetUint()), static_cast<u_char>(interactable["color"][1].GetUint()), static_cast<u_char>(interactable["color"][2].GetUint()), static_cast<u_char>(interactable["color"][3].GetUint()) };
        InteractableType type{ static_cast<InteractableType>(interactable["type"].GetInt()) };

        // Creamos los componentes
        auto& entity = em.newEntity();
        auto& r = em.addComponent<RenderComponent>(entity, RenderComponent{ .position = position, .scale = scale, .color = color });
        auto& p = em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = r.position, .velocity = vec3d::zero() });

        std::cout << "Type: " << type << "\n\n\n" << std::endl;
        switch (type)
        {
        case 1:
        {
            // Es un cofre
            em.addTag<ChestTag>(entity);
            uint16_t zone = static_cast<uint16_t>(interactable["zone"].GetUint());
            ObjectType content{ static_cast<ObjectType>(interactable["content"].GetInt()) };
            uint8_t interId = static_cast<uint8_t>(interactable["id"].GetUint());

            em.addComponent<ColliderComponent>(entity, ColliderComponent{ p.position, r.scale, BehaviorType::STATIC });
            em.addComponent<ChestComponent>(entity, ChestComponent{ .id = interId, .zone = zone, .dropPosition = { vec3d::zero() }, .content = content });
            break;
        }
        }
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
