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
            map = loadMap("assets/Niveles/Lvl_0/Lvl_0.kaiwa");
            em.getSingleton<SoundSystem>().sonido_amb_bosque();
            em.getSingleton<SoundSystem>().sonido_pasos_pradera();
            break;
        }
        case 1:
        {
            li.mapID = 1;
            map = loadMap("assets/Niveles/Lvl_1/Lvl_1.kaiwa");

            em.getSingleton<SoundSystem>().SFX_pasos_stop();
            em.getSingleton<SoundSystem>().sonido_mazmorra();
            em.getSingleton<SoundSystem>().sonido_music_mazmorra();
            em.getSingleton<SoundSystem>().sonido_pasos_prision();
            break;
        }
        case 2:
        {
            li.mapID = 2;
            map = loadMap("assets/Niveles/Lvl_2/Lvl_2.kaiwa");
            em.getSingleton<SoundSystem>().SFX_pasos_stop();
            em.getSingleton<SoundSystem>().sonido_amb_volcan();
            em.getSingleton<SoundSystem>().sonido_music_volcan();
            em.getSingleton<SoundSystem>().sonido_pasos_volcan();

            break;
        }
        case 3:
        {
            li.mapID = 3;
            map = loadMap("assets/Niveles/Lvl_3/Lvl_3.kaiwa");
            em.getSingleton<SoundSystem>().sonido_amb_pradera();
            em.getSingleton<SoundSystem>().sonido_music_pradera();
            em.getSingleton<SoundSystem>().sonido_pasos_pradera();
            break;
        }
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
    auto& li = em.getSingleton<LevelInfo>();

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
                if (li.loadingTime < 0.5f)
                    return;
                const valueType& overworld = chunk[0]["overworld"];
                const valueType& wallArray = overworld["Walls"];
                generateWalls(em, wallArray);
                break;
            }
            case LoadState::LOAD_RAMPS:
            {
                if (li.loadingTime < 1.0f)
                    return;
                const valueType& overworld = chunk[0]["overworld"];
                const valueType& rampArray = overworld["Ramps"];
                generateRamps(em, rampArray);
                break;
            }
            case LoadState::LOAD_INTERACTABLES:
            {
                if (li.loadingTime < 1.5f)
                    return;
                const valueType& overworld = chunk[0]["overworld"];
                const valueType& interactablesArray = overworld["Interactive"];
                generateInteractables(em, interactablesArray);
                break;
            }
            case LoadState::LOAD_OBJECTS:
            {
                if (li.loadingTime < 2.0f)
                    return;
                auto& li = em.getSingleton<LevelInfo>();
                const valueType& underworld = chunk[1]["underworld"];
                const valueType& objectArray = underworld["Objects"];
                generateObjects(em, objectArray, li.mapID);
                break;
            }
            case LoadState::LOAD_ENEMIES:
            {
                if (li.loadingTime < 2.5f)
                    return;
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
                // if (li.mapID == 2)
                generateNavmeshes(em, li.mapID);
                generateCheats(em);
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
    em.addTag<ChunkTag>(modelEntity);
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
        Color color{ D_WHITE };

        // Sacamos los máximos y los mínimos
        vec3d groundMax = groundPosition + groundScale / 2;
        vec3d groundMin = groundPosition - groundScale / 2;
        if (i == 0)
        {
            min.x = groundMin.x();
            min.y = groundMin.z();

            max.x = groundMax.x();
            max.y = groundMax.z();

            minHeigth = groundPosition.y();
        }
        else
        {
            if (min.x > groundMin.x())
                min.x = groundMin.x();
            if (min.y > groundMin.z())
                min.y = groundMin.z();

            if (max.x < groundMax.x())
                max.x = groundMax.x();
            if (max.y < groundMax.z())
                max.y = groundMax.z();

            if (minHeigth > groundPosition.y())
                minHeigth = groundPosition.y();
        }

        auto& li = em.getSingleton<LevelInfo>();
        if (li.mapID == 0)
            em.addComponent<GrassComponent>(groundEntity);

        // Creamos los componentes del suelo
        auto& r = em.addComponent<RenderComponent>(groundEntity, RenderComponent{ .position = groundPosition, .scale = groundScale, .color = color, .visible = false, .orientation = rot, .rotationVec = rotationVec });
        auto& p = em.addComponent<PhysicsComponent>(groundEntity, PhysicsComponent{ .position = r.position, .velocity = vec3d::zero(), .scale = r.scale, .gravity = .0, .orientation = rot, .rotationVec = r.rotationVec });
        em.addComponent<ColliderComponent>(groundEntity, ColliderComponent{ p.position, r.scale, BehaviorType::STATIC });
        em.addTag<SeparateModelTag>(groundEntity);
    }

    // Sacamos la posición del centro del chunk
    vec3d center = { (min.x + max.x) / 2, minHeigth, (min.y + max.y) / 2 };

    auto& li = em.getSingleton<LevelInfo>();

    static std::map<uint8_t, double> zoneHeights = {
        {1, 30.0},
        {2, 65.0},
        {3, 100.0}
    };

    // Creamos 4 zonas para el chunk
    vec3d zoneScale = { (max.x - min.x) / 2, zoneHeights[li.mapID], (max.y - min.y) / 2 };
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

        auto& zchi = em.getSingleton<ZoneCheckInfo>();
        zchi.zoneBounds.insert({ j, cz.bbox });
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
        Color color{ D_GREEN_LIGHT };

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
        Color color{ D_RED };
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
        Color color{ D_GRAY };
        InteractableType type{ static_cast<InteractableType>(interactable["type"].GetInt()) };

        double rot = orientation * DEGTORAD;

        // Creamos los componentes
        auto& entity = em.newEntity();
        auto& r = em.addComponent<RenderComponent>(entity, RenderComponent{ .position = position, .scale = scale, .color = color, .orientation = rot, .rotationVec = rotationVec });
        auto& p = em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = r.position, .velocity = vec3d::zero(), .scale = r.scale, .gravity = 0, .orientation = rot, .rotationVec = r.rotationVec });
        auto& c = em.addComponent<ColliderComponent>(entity, ColliderComponent{ p.position, r.scale, BehaviorType::STATIC });
        auto& ic = em.addComponent<InteractiveComponent>(entity);

        switch (type)
        {
        case InteractableType::Chest:
        {
            em.addTag<ChestTag>(entity);
            em.addTag<WallTag>(entity);
            ObjectType content{ static_cast<ObjectType>(interactable["content"].GetInt()) };

            addMessageCmp(em, entity, interactable);

            ic.range = 7.5;
            em.addComponent<PointLightComponent>(entity);
            em.addComponent<OneUseComponent>(entity, OneUseComponent{ .id = unique_ids++ });
            em.addComponent<ParticleMakerComponent>(entity, ParticleMakerComponent{ .active = true, .effect = ParticleMakerComponent::ParticleEffect::CHEST, .maxParticles = 10, .spawnRate = 0.1f });
            [[maybe_unused]] auto& cc = em.addComponent<ChestComponent>(entity, ChestComponent{ .content = content });

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
            auto& li = em.getSingleton<LevelInfo>();
            em.addTag<DestructibleTag>(entity);
            em.addTag<WallTag>(entity);
            em.addTag<LockableTag>(entity);

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

            if (li.mapID == 1)
            {
                em.addTag<SeparateModelTag>(entity);
                r.orientation += 90.0 * DEGTORAD;
                // r.position.setY(r.position.y() + 4.0);

                em.addComponent<ParticleMakerComponent>(entity, ParticleMakerComponent{ .active = true, .effect = Effects::PRISONDOOR, .maxParticles = 16, .spawnRate = 0.1f, .lifeTime = 0.3f });
                em.addComponent<AnimationComponent>(entity);
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
            case 1:
            {
                em.addTag<SeparateModelTag>(entity);
                r.orientation += 90.0 * DEGTORAD;
                r.position.setY(r.position.y() + 4.0);
                em.addComponent<ParticleMakerComponent>(entity, ParticleMakerComponent{ .active = true, .effect = Effects::PRISONDOOR, .maxParticles = 16, .spawnRate = 0.1f, .lifeTime = 0.3f });
                break;
            }
            }
            em.addTag<DoorTag>(entity);
            em.addTag<WallTag>(entity);

            ic.range = 8.5;
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

            auto& zchi = em.getSingleton<ZoneCheckInfo>();
            zchi.zoneBounds.insert({ zchi.zoneBounds.size(), c.bbox });

            em.addComponent<ZoneComponent>(entity, ZoneComponent{ .zone = static_cast<uint16_t>(zchi.zoneBounds.size() - 1) });
            break;
        }
        case InteractableType::Spawn:
        {
            em.addTag<SpawnTag>(entity);

            auto& spc = em.addComponent<SpawnComponent>(entity);
            c.behaviorType = BehaviorType::SPAWN;
            ic.range = 12.5;

            if (interactable.HasMember("offsetZ"))
            {
                r.offset = interactable["offsetZ"][0].GetDouble();
            }

            auto& pos = p.position;
            auto posY = pos.y() - 2.0;
            double rotCopy = rot;

            if (rotationVec[1] > 0.0)
                rotCopy *= -1;

            std::map<std::size_t, std::pair<vec3d, vec3d>> wallPoints = {
                {0, {rotateAroundPoint(vec3d{pos.x() + 9.5, posY, pos.z() + 14.5}, pos, rotCopy), rotateScale(vec3d{ 3.0, 7.0, 3.0 }, rotCopy)}},
                {1, {rotateAroundPoint(vec3d{pos.x() + 6.0, posY, pos.z() - 10.0}, pos, rotCopy), rotateScale(vec3d{ 3.0, 7.0, 3.0 }, rotCopy)}},
                {2, {rotateAroundPoint(vec3d{pos.x() - 17.5, posY, pos.z() + 15.0}, pos, rotCopy), rotateScale(vec3d{ 3.0, 7.0, 3.0 }, rotCopy)}},
                {3, {rotateAroundPoint(vec3d{pos.x() - 15.5, posY, pos.z() - 11.5}, pos, rotCopy), rotateScale(vec3d{ 3.0, 7.0, 3.0 }, rotCopy)}},
                {4, {rotateAroundPoint(vec3d{pos.x() - 3.5, posY, pos.z() + 14.0}, pos, rotCopy), rotateScale(vec3d{ 14.0, 4.0, 4.0 }, rotCopy)}},
                {5, {rotateAroundPoint(vec3d{pos.x() - 16.0, posY, pos.z() - 3.0}, pos, rotCopy), rotateScale(vec3d{ 5.0, 4.0, 13.0 }, rotCopy)}},
                {6, {rotateAroundPoint(vec3d{pos.x() - 3.0, posY - 2.0, pos.z() + 3.5}, pos, rotCopy), rotateScale(vec3d{ 4.5, 2.0, 4.5 }, rotCopy)}},
                {7, {rotateAroundPoint(vec3d{pos.x() + 7.0, posY, pos.z() + 8.0}, pos, rotCopy), rotateScale(vec3d{ 6.0, 7.0, 6.0 }, rotCopy)}},
                {8, {rotateAroundPoint(vec3d{pos.x() - 10.5, posY, pos.z() + 8.0}, pos, rotCopy), rotateScale(vec3d{ 1.5, 4.0, 1.5 }, rotCopy)}},
                {9, {rotateAroundPoint(vec3d{pos.x() + 4.2, posY + 2.0, pos.z() - 6.0}, pos, rotCopy), rotateScale(vec3d{ .5, 2.0, .5 }, rotCopy)}},
                {10, {rotateAroundPoint(vec3d{pos.x() - 17.5, posY + 2.0, pos.z() - 9.5}, pos, rotCopy), rotateScale(vec3d{ .5, 2.0, .5 }, rotCopy)}}
            };

            std::map<std::size_t, std::tuple<Effects, uint8_t, float, float>> particleParts = {
                {6, {Effects::FIRE, 10, 0.1f, 0.3f}},
                {7, {Effects::SMOKE, 10, 0.1f, 0.3f}},
                {8, {Effects::PURPLEM, 4, 0.1f, 0.3f}},
                {9, {Effects::SPARKLES, 3, 0.05f, 0.3f}},
                {10, {Effects::SPARKLES, 3, 0.05f, 0.3f}},
            };

            // Creamos 4 paredes para el spawn
            for (std::size_t j = 0; j < wallPoints.size(); j++)
            {
                auto& wall{ em.newEntity() };
                em.addTag<WallTag>(wall);
                auto& rWall = em.addComponent<RenderComponent>(wall, RenderComponent{ .position = wallPoints[j].first, .scale = wallPoints[j].second, .color = color, .visible = false, .orientation = 0.0, .rotationVec = rotationVec });
                em.addComponent<PhysicsComponent>(wall, PhysicsComponent{ .position = wallPoints[j].first, .velocity = vec3d::zero(), .scale = wallPoints[j].second, .gravity = 0, .orientation = 0.0, .rotationVec = rotationVec });
                em.addComponent<ColliderComponent>(wall, ColliderComponent{ position, scale, BehaviorType::STATIC });

                if (j > 5)
                {
                    auto& plc = em.addComponent<PointLightComponent>(wall);
                    auto& p = em.addComponent<ParticleMakerComponent>(wall, ParticleMakerComponent{ .active = true, .effect = std::get<0>(particleParts[j]), .maxParticles = std::get<1>(particleParts[j]), .spawnRate = std::get<2>(particleParts[j]), .lifeTime = std::get<3>(particleParts[j]) });
                    spc.parts.push_back({ &rWall, &p, &plc });
                }
            }

            addToZone(em, entity, type);
            checkDispatcher(em, entity, interactable);
            break;
        }
        case InteractableType::Lever:
        {
            em.addTag<WallTag>(entity);
            em.addTag<LeverTag>(entity);
            em.addComponent<OneUseComponent>(entity, OneUseComponent{ .id = unique_ids++ });
            addToZone(em, entity, type);

            ic.range = 7.5;
            break;
        }
        case InteractableType::DamageObj:
        {
            em.addTag<LavaTag>(entity);
            em.addComponent<ParticleMakerComponent>(entity, ParticleMakerComponent{ .active = true, .effect = ParticleMakerComponent::ParticleEffect::LAVA, .maxParticles = 10, .spawnRate = 0.1f });
            c.behaviorType = BehaviorType::LAVA;

            if (interactable.HasMember("offsetZ"))
            {
                r.offset = interactable["offsetZ"][0].GetDouble();
            }
            break;
        }
        case InteractableType::Roca:
        {
            em.addTag<ObstacleTag>(entity);
            r.visible = false;
            break;
        }
        case InteractableType::Ladder:
        {
            em.addTag<LadderTag>(entity);
            r.visible = false;
            c.behaviorType = BehaviorType::LADDER;
            ic.range = 12.0;
            addToZone(em, entity, type);

            auto yMin = c.bbox.min.y();
            auto yMax = c.bbox.max.y();
            // auto rot2 = (orientation - 90.0) * DEGTORAD;
            em.addComponent<LadderComponent>(entity, LadderComponent{ .orientation = rot, .yMin = yMin, .yMax = yMax });
            break;
        }
        case InteractableType::Sign:
        {
            em.addTag<SignTag>(entity);

            ic.range = 12.0;
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
        case InteractableType::MissionOBJ:
        {
            auto& li = em.getSingleton<LevelInfo>();
            em.addTag<MissionObjTag>(entity);
            switch (li.mapID)
            {
            case 2:
            {
                em.addComponent<ParticleMakerComponent>(entity, ParticleMakerComponent{ .active = true, .effect = ParticleMakerComponent::ParticleEffect::MISSIONOBJ, .maxParticles = 10, .spawnRate = 0.1f });
                auto& bc = em.addComponent<BoatComponent>(entity);
                bc.setPart(boatParts);
                ic.range = 7.0;

                addToZone(em, entity, type);
                if (interactable.HasMember("offsetZ"))
                {
                    r.offset = interactable["offsetZ"][0].GetDouble();
                }
                break;
            }
            case 3:
            {
                em.addTag<LockableTag>(entity);
                c.behaviorType = BehaviorType::RELAY;
                auto relayType = static_cast<ElementalType>(interactable["relay"].GetUint());
                em.addComponent<TypeComponent>(entity, TypeComponent({ .type = relayType }));
                em.addComponent<LifeComponent>(entity, LifeComponent({ .life = 20 }));
                break;
            }
            default:
                break;
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
    auto& li = em.getSingleton<LevelInfo>();
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
        NPCType npcType = static_cast<NPCType>(npc["npc"].GetUint());

        vec_t.push_back(std::make_unique<BehaviourTree_t>());
        auto& tree = *vec_t.back();

        tree.createNode<BTAction_PatrolNPC>();

        auto& entity{ em.newEntity() };
        em.addTag<NPCTag>(entity);

        auto& wr = em.addComponent<RenderComponent>(entity, RenderComponent{ .position = position, .scale = scale, .color = color,.orientation = rot,.rotationVec = rotationVec });
        auto& wp = em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = wr.position, .scale = wr.scale,.orientation = rot,.rotationVec = rotationVec, .max_speed = max_speed });
        em.addComponent<ColliderComponent>(entity, ColliderComponent{ wp.position, wp.scale, BehaviorType::STATIC });

        double range = 0.0;
        switch (npcType)
        {
        case NPCType::NOMAD:
        {
            em.addTag<NomadTag>(entity);
            switch (li.mapID)
            {
            case 1:
                range = 30.0;
                break;
            case 2:
                range = 7.5;
                break;
            }
            break;
        }
        case NPCType::INVESTIGATOR:
            em.addTag<InvestigatorTag>(entity);
            range = 21.0;
            break;
        default:
            break;
        }

        em.addComponent<InteractiveComponent>(entity, InteractiveComponent{ .range = range });
        em.addComponent<NPCComponent>(entity, NPCComponent{ .path = path, .type = npcType, .behaviourTree = &tree,.arrival_radius = arrival_radius });

        em.addComponent<OneUseComponent>(entity, OneUseComponent{ .id = unique_ids++ });

        checkDispatcher(em, entity, npc);

        addToZone(em, entity, InteractableType::NPC);
    }
}
vec3d getNodeVec3d(uint16_t nodeId, const std::map<uint16_t, vec3d>& nodes) {
    for (const auto& node : nodes) {
        if (node.first == nodeId) {
            return node.second;
        }
    }
    // Si no se encuentra el nodo, se devuelve un vec3d con valores predeterminados o se maneja el error de alguna otra manera
    return { 0.0f, 0.0f, 0.0f };
}
void MapManager::generateNavmeshes(EntityManager& em, uint8_t map)
{

    auto& navs = em.getSingleton<NavmeshInfo>();
    mapType navmeshkaiwa{};
    if (map == 2) {
        navmeshkaiwa = loadMap("assets/Niveles/Lvl_2/Lvl_2-navmeshes.kaiwa");
    }
    else {
        navmeshkaiwa = loadMap("assets/Niveles/Lvl_4/Lvl_4-navmeshes.kaiwa");
    }
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
            navs.corners.insert(vecnodes[j + 1]);
            if (j == 0)
            {
                max = vecnodes[j + 1];
                min = vecnodes[j + 1];
            }
            else {
                if (max.x() < vecnodes[j + 1].x()) {
                    max.setX(vecnodes[j + 1].x());
                }
                if (max.y() < vecnodes[j + 1].y()) {
                    max.setY(vecnodes[j + 1].y());
                }
                if (max.z() < vecnodes[j + 1].z()) {
                    max.setZ(vecnodes[j + 1].z());
                }
                if (min.x() > vecnodes[j + 1].x()) {
                    min.setX(vecnodes[j + 1].x());
                }
                if (min.y() > vecnodes[j + 1].y()) {
                    min.setY(vecnodes[j + 1].y());
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
            vecnodes[k + 5] = { midpoints[k][1].GetDouble(),  midpoints[k][2].GetDouble(),  midpoints[k][0].GetDouble() };
            navs.midpoints.insert(vecnodes[k + 5]);
        }

        //Rampa?
        bool hasramp{ false };

        // const valueType& xd = navmesh["ramp"];
        // if(xd==NULL){
        //     hasramp = true;
        // }        
        // if(map == 2){
        const rapidjson::Value::ConstMemberIterator& xd = navmesh.FindMember("ramp");
        if (xd != navmesh.MemberEnd()) {
            if (xd->value.GetBool())
                hasramp = true;
        }
        // }
        // bool hasramp = navmesh["ramp"].GetBool();

        //Creamos NavMesh BBox
        double scalex{}, scalez{};

        scalex = (max.x() - min.x());
        scalez = (max.z() - min.z());
        BBox b(vecnodes[0], vec3d{ scalex, (max.y() - min.y()) + 0.7, scalez });
        //BBox b{ min,max };
        navs.boundingnavmesh.push_back(b);
        //Creamos NavMesh
        Navmesh nav{ .box = b,.ramp = hasramp };
        //Rellenamos los nodos
        for (auto& n : vecnodes) {
            auto pair = std::make_pair(navs.num_nodes, n);

            //si es el centro, se guarda
            if (n.x() == vecnodes[0].x() && n.z() == vecnodes[0].z()) {
                //Se añade al navinfo
                nav.centerpoint = pair;
                navs.centers.insert(pair);
            }
            nav.nodes.insert(pair);
            navs.nodes.insert(pair);
            navs.num_nodes++;
        }
        navs.NavMeshes.push_back(nav);
        // Conection c6{1, }; auxconex.push_back(c6);
        // Conection c7{1, }; auxconex.push_back(c7);
        // Conection c8{1, }; auxconex.push_back(c8);
        //Creamos conexiones de el navmesh
        if (nav.ramp == false) {
            for (auto it = nav.nodes.begin(); it != std::prev(nav.nodes.end()); ++it) {
                auto& currentNode = it->second;
                auto& nextNode = std::next(it)->second;

                // Comprobamos que no se repitan y que no hayan conexiones que pasen por puntos medios
                if (navs.insert_ids(it->first, std::next(it)->first) &&
                    navs.checkmidpoints(currentNode, nextNode)) {
                    Conection c{ 1, it->first, std::next(it)->first };
                    navs.conexiones.push_back(c);
                    //debug
                    auto pair = std::make_pair(it->second, std::next(it)->second);
                    navs.conexpos.insert(pair);
                }
            }
        }
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////7
    // Guardamos info 
    // std::vector<Conection> auxconex;
    // auxconex.push_back(Conection{ 1,27,7 });
    // auxconex.push_back(Conection{ 1,7,26 });
    // auxconex.push_back(Conection{ 1,26,45 });
    // auxconex.push_back(Conection{ 1,45,50 });
    // auxconex.push_back(Conection{ 1,10,26 });
    // auxconex.push_back(Conection{ 1,0,26 });
    // auxconex.push_back(Conection{ 1,44,0 });
    // auxconex.push_back(Conection{ 1,54,50 });
    // auxconex.push_back(Conection{ 1,59,54 });
    // auxconex.push_back(Conection{ 1,98,59 });
    // auxconex.push_back(Conection{ 1,116,98 });
    // auxconex.push_back(Conection{ 1,124,116 });
    // auxconex.push_back(Conection{ 1,123,124 });
    // auxconex.push_back(Conection{ 1,134,123 });
    // auxconex.push_back(Conection{ 1,242,134 });
    // auxconex.push_back(Conection{ 1,468,473 });
    // auxconex.push_back(Conection{ 1,583,572 });
    // auxconex.push_back(Conection{ 1,473,583 });
    // auxconex.push_back(Conection{ 1,637,816 });
    // auxconex.push_back(Conection{ 1,637,639 });
    // auxconex.push_back(Conection{ 1,639,643 });
    // auxconex.push_back(Conection{ 1,643,837 });
    // auxconex.push_back(Conection{ 1,837,846 });
    // auxconex.push_back(Conection{ 1,639,620 });
    // auxconex.push_back(Conection{ 1,620,639 });
    // auxconex.push_back(Conection{ 1,846,895 });
    // auxconex.push_back(Conection{ 1,816,827 });
    // auxconex.push_back(Conection{ 1,822,827 });
    // auxconex.push_back(Conection{ 1,288,297 });
    // auxconex.push_back(Conection{ 1,297,320 });
    // auxconex.push_back(Conection{ 1,320,333 });

    // for (auto& c : auxconex) {
    //     navs.conexiones.push_back(c);
    //     //debug
    //     auto pair = std::make_pair(getNodeVec3d(c.fromNode, navs.nodes), getNodeVec3d(c.toNode, navs.nodes));
    //     navs.conexpos.insert(pair);
    //     //nodes
    //     for (auto& [n, vec] : navs.nodes) {
    //         // if(n == 583){
    //         //     vec = vec3d{vec.x(),vec.y(),vec.z()+10.0};
    //         // }
    //         if (n == c.toNode) {
    //             navs.selectednodes.insert(std::make_pair(n, vec));
    //         }
    //         if (n == c.fromNode) {
    //             navs.selectednodes.insert(std::make_pair(n, vec));
    //         }
    //     }
    // }

    // auto& li = em.getSingleton<LevelInfo>();
    // li.level_graph.createGraph(navs.conexiones, navs.selectednodes);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////7
    // Crear conexiones de navmesh con otros con los que colisiona
    // for (auto it = navs.NavMeshes.begin(); it != navs.NavMeshes.end(); ++it) {
    //     for (auto nextnavmesh = std::next(it); nextnavmesh != navs.NavMeshes.end(); ++nextnavmesh) {
    //         if(!it->ramp && !nextnavmesh->ramp){
    //             auto& currentbbox = it->box;
    //             auto& nextbbox = nextnavmesh->box;

    //             // Conexiones con navmeshes colisionables
    //             if (currentbbox.intersects(nextbbox)) {
    //                 // Obtenemos el centro (primer nodo) del primer navmesh
    //                 const auto& currentNode = it->nodes.begin();

    //                 // Obtenemos el centro (primer nodo) del segundo navmesh
    //                 const auto& nextNode = nextnavmesh->nodes.begin();

    //                 // Comprobamos que no se repitan y que no hayan conexiones que pasen por puntos medios
    //                 if (navs.insert_ids(currentNode->first, nextNode->first)) {
    //                     Conection c{ 1, currentNode->first, nextNode->first };
    //                     navs.conexiones.push_back(c);
    //                     // debug
    //                     auto pair = std::make_pair(currentNode->second, nextNode->second);
    //                     navs.conexpos.insert(pair);
    //                 }
    //             }
    //         }
    //     }
    // }

    //Crear conexiones de navmesh con otros con los que colisiona
    for (auto it = navs.NavMeshes.begin(); it != navs.NavMeshes.end(); ++it) {
        for (auto nextnavmesh = std::next(it); nextnavmesh != navs.NavMeshes.end(); ++nextnavmesh) {
            if (!it->ramp && !nextnavmesh->ramp) {
                auto& currentbbox = it->box;
                auto& nextbbox = nextnavmesh->box;
                //auto& currentnodes = it->nodes;
                auto& currentcenter = it->centerpoint;

                // Conexiones con navmeshes colisionables
                if (currentbbox.intersects(nextbbox)) {

                    // Recorremos nodos del primer navmesh
                    for (auto it2 = it->nodes.begin(); it2 != it->nodes.end(); ++it2) {
                        const auto& currentNode = it2;

                        // Solo consideramos nodos en el borde de la intersección
                        if (!nextbbox.intersects(currentNode->second)
                            && currentNode != it->nodes.begin()) continue;

                        // Recorremos nodos del segundo navmesh
                        for (auto it3 = nextnavmesh->nodes.begin(); it3 != nextnavmesh->nodes.end(); ++it3) {
                            const auto& nextNode = it3;

                            //Solo consideramos nodos en el borde de la intersección
                            if (!currentbbox.intersects(nextNode->second)
                                && nextNode != nextnavmesh->nodes.begin()) continue;

                            // Comprobamos que no se repitan y que no hayan conexiones que pasen por puntos medios
                            //(navs.checkcousinsnodes())
                            //navs.insert_ids(currentNode->first, nextNode->first
                            //navs.checkcousins(nextNode->second.x(),nextNode->second.z(),it->nodes)
                            if (navs.insert_ids(currentNode->first, nextNode->first)) {
                                Conection c{ 1, currentcenter.first, nextNode->first };
                                navs.conexiones.push_back(c);
                                // debug
                                auto pair = std::make_pair(currentcenter.second, nextNode->second);
                                navs.conexpos.insert(pair);
                            }
                        }
                    }
                }
            }
        }
    }
    //Conexiones a mano de las rampas del nivel 2
    //rampa muñeco izquierda
    std::vector<Conection> auxconex;
    if (map == 2) {
        auxconex.push_back(Conection{ 1,2,18 });
        auxconex.push_back(Conection{ 1,5,18 });
        auxconex.push_back(Conection{ 1,26,18 });
        auxconex.push_back(Conection{ 1,18,53 });
        auxconex.push_back(Conection{ 1,53,50 });
        auxconex.push_back(Conection{ 1,44,0 });
        auxconex.push_back(Conection{ 1,14,2 });
        auxconex.push_back(Conection{ 1,27,14 });
        auxconex.push_back(Conection{ 1,21,18 });
        // rampa derecha muñeco
        auxconex.push_back(Conection{ 1,63,59 });
        auxconex.push_back(Conection{ 1,98,63 });
        auxconex.push_back(Conection{ 1,64,63 });
        auxconex.push_back(Conection{ 1,65,63 });
        // rampa mas abajo
        auxconex.push_back(Conection{ 1,124,116 });
        auxconex.push_back(Conection{ 1,121,116 });
        auxconex.push_back(Conection{ 1,119,116 });
        //
        auxconex.push_back(Conection{ 1,576,572 });
        auxconex.push_back(Conection{ 1,572,567 });
        auxconex.push_back(Conection{ 1,567,536 });
        auxconex.push_back(Conection{ 1,620,572 });
        auxconex.push_back(Conection{ 1,583,572 });
        auxconex.push_back(Conection{ 1,470,576 });
        auxconex.push_back(Conection{ 1,473,576 });
        auxconex.push_back(Conection{ 1,484,473 });
        //
        auxconex.push_back(Conection{ 1,333,346 });
        auxconex.push_back(Conection{ 1,303,333 });
        //
        auxconex.push_back(Conection{ 1,643,843 });
        auxconex.push_back(Conection{ 1,646,843 });
        auxconex.push_back(Conection{ 1,652,843 });
        auxconex.push_back(Conection{ 1,843,837 });
        auxconex.push_back(Conection{ 1,647,648 });
        auxconex.push_back(Conection{ 1,630,639 });
        //
        auxconex.push_back(Conection{ 1,801,808 });
        auxconex.push_back(Conection{ 1,808,819 });
        auxconex.push_back(Conection{ 1,742,801 });
        auxconex.push_back(Conection{ 1,799,801 });
        auxconex.push_back(Conection{ 1,731,801 });
        auxconex.push_back(Conection{ 1,736,801 });
        auxconex.push_back(Conection{ 1,745,801 });
        auxconex.push_back(Conection{ 1,792,742 });
        auxconex.push_back(Conection{ 1,800,742 });
        auxconex.push_back(Conection{ 1,644,808 });
        auxconex.push_back(Conection{ 1,632,639 });
    }
    else {
        auxconex.push_back(Conection{ 1,457,448 });
        auxconex.push_back(Conection{ 1,448,419 });
        auxconex.push_back(Conection{ 1,419,421 });
    }

    if (auxconex.size() != 0) {
        for (auto& c : auxconex) {
            navs.conexiones.push_back(c);
            //debug
            auto pair = std::make_pair(getNodeVec3d(c.fromNode, navs.nodes), getNodeVec3d(c.toNode, navs.nodes));
            navs.conexpos.insert(pair);
            //nodes
            // for (auto& [n, vec] : navs.nodes) {
            //     // if(n == 583){
            //     vec = vec3d{ vec.x(),vec.y(),vec.z() + 10.0 };
            //     // }
            //     if (n == c.toNode) {
            //         navs.selectednodes.insert(std::make_pair(n, vec));
            //     }
            //     if (n == c.fromNode) {
            //         navs.selectednodes.insert(std::make_pair(n, vec));
            //     }
            // }
        }
    }
    //Crear conexiones entre los centros
    // Crear conexiones entre los centros
    // for (auto it = navs.centers.begin(); it != navs.centers.end(); ++it) {
    //     auto currentCenter = it->first;
    //     auto currentcenterpos = it->second;
    //     // Recorrer todos los centros nuevamente para encontrar conexiones
    //     for (auto nextCenter = std::next(it); nextCenter != navs.centers.end(); ++nextCenter) {
    //         double distance = currentcenterpos.distance(nextCenter->second);

    //         // Establecer la distancia máxima para crear una conexión (ajusta el valor según sea necesario)
    //         double maxDistance = 60.0; // Por ejemplo, 10 unidades

    //         // Si la distancia entre los centros es menor o igual a la distancia máxima, crear una conexión
    //         if (distance <= maxDistance) {
    //             // Crea la conexión si no existe ya
    //             // if (navs.insert_ids(currentCenter, nextCenter->first)) {
    //                 Conection c{ 1, currentCenter, nextCenter->first };
    //                 navs.conexiones.push_back(c);
    //                 auto pair = std::make_pair(currentcenterpos, nextCenter->second);
    //                 navs.conexpos.insert(pair);
    //             // }
    //        }
    //     }
    // }
}

vec3d MapManager::rotateY(const vec3d& v, double angle) {
    double cosTheta = cos(angle);
    double sinTheta = sin(angle);
    return vec3d{
        cosTheta * v.x() - sinTheta * v.z(),
        v.y(),
        sinTheta * v.x() + cosTheta * v.z()
    };
}

vec3d MapManager::rotateAroundPoint(const vec3d& point, const vec3d& pivot, double angle) {
    // Translate point back to origin
    vec3d p = point - pivot;

    // Rotate point
    vec3d rotated = rotateY(p, angle);

    // Translate point back
    return rotated + pivot;
}

vec3d MapManager::rotateScale(const vec3d& v, double angle) {
    double cosTheta = cos(angle);
    double sinTheta = sin(angle);
    return vec3d{
        std::abs(cosTheta * v.x() + sinTheta * v.z()),
        v.y(),
        std::abs(-sinTheta * v.x() + cosTheta * v.z())
    };
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
    for (auto [num, bbox] : zchi.zoneBounds)
        if (bbox.intersects(c.bbox))
        {
            zchi.insertZone(num, type);

            if (e.hasComponent<OneUseComponent>())
            {
                auto& ouc = em.getComponent<OneUseComponent>(e);
                ouc.zones.insert(num);
            }
        }
}

void MapManager::spawnReset(EntityManager& em, Ia_man&)
{
    using CMPS = MP::TypeList<PhysicsComponent, LifeComponent, AIComponent>;
    using TAGS = MP::TypeList<EnemyTag>;
    //auto& li = em.getSingleton<LevelInfo>();

    em.forEach<CMPS, TAGS>([&](Entity& e, PhysicsComponent& phy, LifeComponent& lic, AIComponent& aic)
    {
        if (e.hasTag<EnemyDeathTag>())
            em.destroyTag<EnemyDeathTag>(e);
        lic.life = lic.maxLife;
        lic.markedForDeletion = false;
        lic.onDeathAnim = false;
        phy.position = aic.initialPos;
    });
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
    li.npcflee = false;
    li.door_open = false;
    state = LoadState::LOAD_CHUNKS;
    boatParts = 0;
    zchi.clearSets();
    zchi.zoneBounds.clear();
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

void MapManager::generateCheats(EntityManager& em)
{
    auto& cheatPositions = em.getSingleton<CheatsInfo>().cheatPositions;
    if (!cheatPositions.empty())
        return;

    mapType cheatsKaiwa = loadMap("assets/cheats.kaiwa");
    const valueType& levels = cheatsKaiwa["Niveles"];
    for (mapSizeType i = 0; i < levels.Size(); i++)
    {
        uint8_t levelNum = static_cast<uint8_t>(levels[i]["Nombre"].GetUint());
        auto& positions = levels[i]["Puntos"];
        for (mapSizeType j = 0; j < positions.Size(); j++)
        {
            std::string positionName = positions[j]["Nombre"].GetString();
            auto& positionLabel = positions[j]["Posición"];
            vec3d position = { positionLabel[0].GetDouble(), positionLabel[1].GetDouble(), positionLabel[2].GetDouble() };
            cheatPositions[cheatPositions.size()] = std::make_tuple(levelNum, positionName, position);
        }
    }
}