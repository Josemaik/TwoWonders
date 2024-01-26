#include "map.hpp"
#include <fstream>

void Map::createMap(EntityManager& em, uint8_t mapID, Ia_man& iam) {

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

mapType Map::loadMap(const std::string& mapFile)
{
    std::ifstream file(mapFile);
    mapType map{};
    file >> map;
    return map;
}

void Map::generateMapFromJSON(EntityManager& em, const mapType& map, Ia_man& iam)
{
    for (const auto& ground : map["overworld"]["ground"])
    {
        auto& entity = em.newEntity();
        em.addTag<GroundTag>(entity);

        // Extraemos los datos del json
        vec3d position{ ground["position"][0], ground["position"][1], ground["position"][2] };
        vec3d scale{ ground["scale"][0], ground["scale"][1], ground["scale"][2] };
        Color color{ ground["color"][0], ground["color"][1], ground["color"][2], ground["color"][3] };

        // Creamos los componentes
        auto& r = em.addComponent<RenderComponent>(entity, RenderComponent{ .position = position, .scale = scale, .color = color });
        auto& p = em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = r.position, .velocity = vec3d::zero(), .gravity = .0 });
        em.addComponent<ColliderComponent>(entity, ColliderComponent{ p.position, r.scale, BehaviorType::STATIC });
    }

    for (const auto& water : map["overworld"]["water"])
    {
        auto& entity = em.newEntity();
        em.addTag<WaterTag>(entity);

        // Extraemos los datos del json
        vec3d position{ water["position"][0], water["position"][1], water["position"][2] };
        vec3d scale{ water["scale"][0], water["scale"][1], water["scale"][2] };
        Color color{ water["color"][0], water["color"][1], water["color"][2], water["color"][3] };

        // Creamos los componentes
        auto& r = em.addComponent<RenderComponent>(entity, RenderComponent{ .position = position, .scale = scale, .color = color });
        auto& p = em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = r.position, .velocity = vec3d::zero(), .gravity = .0 });
        em.addComponent<ColliderComponent>(entity, ColliderComponent{ p.position, r.scale, BehaviorType::STATIC });
    }

    for (const auto& wall : map["overworld"]["walls"])
    {
        auto& entity = em.newEntity();
        em.addTag<WallTag>(entity);
        // Extraemos los datos del json
        vec3d position{ wall["position"][0], wall["position"][1], wall["position"][2] };
        vec3d scale{ wall["scale"][0], wall["scale"][1], wall["scale"][2] };
        Color color{ wall["color"][0], wall["color"][1], wall["color"][2], wall["color"][3] };

        // Creamos los componentes
        auto& r = em.addComponent<RenderComponent>(entity, RenderComponent{ .position = position, .scale = scale, .color = color });
        auto& p = em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = r.position, .velocity = vec3d::zero(), .gravity = .0 });
        em.addComponent<ColliderComponent>(entity, ColliderComponent{ p.position, r.scale, BehaviorType::STATIC });
    }

    for (const auto& zone : map["overworld"]["zones"])
    {
        auto& entity = em.newEntity();
        em.addTag<ZoneTag>(entity);

        // Extraemos los datos del json
        vec3d position{ zone["position"][0], zone["position"][1], zone["position"][2] };
        vec3d scale{ zone["scale"][0], zone["scale"][1], zone["scale"][2] };
        uint16_t zoneNumber{ zone["zone"] };

        // Creamos los componentes
        em.addComponent<ZoneComponent>(entity, ZoneComponent{ .zone = zoneNumber });
        auto& r = em.addComponent<RenderComponent>(entity, RenderComponent{ .position = position, .scale = scale, .visible = false });
        auto& p = em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = r.position, .velocity = vec3d::zero(), .gravity = .0 });
        em.addComponent<ColliderComponent>(entity, ColliderComponent{ p.position, r.scale, BehaviorType::ZONE });
    }

    for (const auto& ramp : map["overworld"]["ramps"])
    {
        auto& entity = em.newEntity();
        em.addTag<RampTag>(entity);

        // Extraemos los datos del json
        vec3d position{ ramp["position"][0], ramp["position"][1], ramp["position"][2] };
        vec3d scale{ ramp["scale"][0], ramp["scale"][1], ramp["scale"][2] };
        Color color{ ramp["color"][0], ramp["color"][1], ramp["color"][2], ramp["color"][3] };
        float orientation{ ramp["rotation"] };
        vec3d rotationVec{ ramp["rotVector"][0], ramp["rotVector"][1], ramp["rotVector"][2] };
        vec2d min{ ramp["min"][0], ramp["min"][1] };
        vec2d max{ ramp["max"][0], ramp["max"][1] };
        double slope{ ramp["slope"] };
        vec2d offset{ ramp["offset"][0], ramp["offset"][1] };

        // Creamos los componentes
        auto& r = em.addComponent<RenderComponent>(entity, RenderComponent{ .position = position, .scale = scale, .color = color, .rotationVec = rotationVec });
        em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = r.position, .velocity = vec3d::zero(), .gravity = .0, .orientation = orientation * DEGTORAD, .rotationVec = rotationVec });
        em.addComponent<RampComponent>(entity, RampComponent{ .min = min, .max = max, .slope = slope, .offset = offset });
    }

    for (const auto& door : map["overworld"]["doors"])
    {
        auto& entity = em.newEntity();
        em.addTag<DoorTag>(entity);

        // Extraemos los datos del json
        vec3d position{ door["position"][0], door["position"][1], door["position"][2] };
        vec3d scale{ door["scale"][0], door["scale"][1], door["scale"][2] };
        Color color{ door["color"][0], door["color"][1], door["color"][2], door["color"][3] };

        // Creamos los componentes
        auto& r = em.addComponent<RenderComponent>(entity, RenderComponent{ .position = position, .scale = scale, .color = color });
        auto& p = em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = r.position, .velocity = vec3d::zero(), .gravity = .0 });
        em.addComponent<ColliderComponent>(entity, ColliderComponent{ p.position, r.scale, BehaviorType::STATIC });
    }

    uint8_t mapId = map["id"];
    for (const auto& obj : map["underworld"]["objects"])
    {
        auto& li = em.getSingleton<LevelInfo>();
        uint8_t objId = obj["id"];

        std::pair<uint8_t, uint8_t> pair{ mapId, objId };

        if (li.notLoadSet.find(pair) != li.notLoadSet.end())
            continue;

        // Extraemos los datos del json
        vec3d position{ obj["position"][0], obj["position"][1], obj["position"][2] };
        vec3d scale{ obj["scale"][0], obj["scale"][1], obj["scale"][2] };
        Color color{ obj["color"][0], obj["color"][1], obj["color"][2], obj["color"][3] };
        Object_type type{ obj["type"] };

        auto& entity = em.newEntity();
        em.addTag<ObjectTag>(entity);

        // Creamos los componentes
        auto& r = em.addComponent<RenderComponent>(entity, RenderComponent{ .position = position, .scale = scale, .color = color });
        auto& p = em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = r.position, .velocity = vec3d::zero(), .gravity = .0 });
        em.addComponent<ColliderComponent>(entity, ColliderComponent{ p.position, r.scale, BehaviorType::STATIC });
        em.addComponent<ObjectComponent>(entity, ObjectComponent{ .type = type, .inmortal = true, .mapID = mapId, .objID = objId });
    }

    for (const auto& enemy : map["underworld"]["enemies"])
    {
        iam.createEnemy(em, enemy);
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

void Map::destroyMap(EntityManager& em)
{
    using CMPS = MP::TypeList<>;
    using TAGS = MP::TypeList<GroundTag, WaterTag, WallTag, ZoneTag, RampTag, DoorTag, ObjectTag, EnemyTag>;

    deathSet set{};

    em.forEachAny<CMPS, TAGS>([&](Entity& entity) {
        set.insert(entity.getID());
    });

    em.destroyEntities(set);
}

void Map::reset(EntityManager& em, uint8_t mapID, Ia_man& iam)
{
    destroyMap(em);
    createMap(em, mapID, iam);
}