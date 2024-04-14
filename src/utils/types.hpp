#pragma once

// Components - Colocar en CL
#include "../components/collider_component.hpp"
#include "../components/physics_component.hpp"
#include "../components/render_component.hpp"
#include "../components/attack_component.hpp"
#include "../components/input_component.hpp"
#include "../components/life_component.hpp"
#include "../components/type_component.hpp"
#include "../components/ramp_component.hpp"
#include "../components/chest_component.hpp"
#include "../components/listener_component.hpp"
#include "../components/destructible_component.hpp"
#include "../components/interactive_component.hpp"
#include "../components/subject_component.hpp"
#include "../components/angrybush_component.hpp"
#include "../components/dispatcher_component.hpp"
#include "../components/oneuse_component.hpp"
#include "../components/ladder_component.hpp"
#include "../components/message_component.hpp"
#include "../components/boat_component.hpp"
//ia
#include "../components/ai_component.hpp"
#include "../components/navmesh_component.hpp"
#include "../components/npc_component.hpp"
#include "../components/projectile_component.hpp"
#include "../components/object_component.hpp"
#include "../components/zone_component.hpp"
#include "../components/shield_component.hpp"
#include "../managers/entity_manager.hpp"
#include "../utils/meta_program.hpp"
#include "../utils/Item.hpp"

//singelton
#include "./sngtn/player_info.hpp"
#include "./sngtn/blackboard.hpp"
#include "./sngtn/debug_singleton.hpp"
#include "./sngtn/level_info.hpp"
#include "./sngtn/input_info.hpp"
#include "./sngtn/text_info.hpp"
#include "./sngtn/navmesh_info.hpp"
#include "./sngtn/zonecheck_info.hpp"

// GameData
#include "../utils/sngtn/GameData.hpp"

// External Libraries
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include "../systems/sound_system.hpp"

// Constants
static constexpr double K_PI = 3.14159265358979323846;
static constexpr double DEGTORAD = static_cast<double>(K_PI / 180.0);
static constexpr float timeStep30 = 1.0f / 30.0f;  // Actualiza el juego 30 veces por segundo
static constexpr float timeStep = 1.0f / 60.0f;  // Actualiza el juego 60 veces por segundo
static constexpr float timeStep120 = 1.0f / 120.0f;  // Actualiza el juego 120 veces por segundo
static constexpr float timeStep240 = 1.0f / 240.0f;  // Actualiza el juego 240 veces por segundo
static constexpr float timeStep360 = 1.0f / 360.0f;  // Actualiza el juego 360 veces por segundo
static constexpr float timeStep480 = 1.0f / 480.0f;  // Actualiza el juego 480 veces por segundo
static constexpr double timeStepDouble = 1.0 / 60.0;  // Actualiza el juego 60 veces por segundo
static constexpr double timeStepDouble240 = 1.0 / 240.0;  // Actualiza el juego 240 veces por segundo
// Forward Declarations
namespace ENGI { struct GameEngine; }

// Tags - Colocar en TL
struct PlayerTag {};
struct EnemyTag {};
struct HitPlayerTag {};
struct GroundTag {};
struct WallTag {};
struct ObjectTag {};
struct ZoneTag {};
struct DoorTag {};
struct RampTag {};
struct SlimeTag {};
struct SnowmanTag {};
struct GolemTag {};
struct SpiderTag {};
struct BossFinalTag {};
struct DummyTag {};
struct AngryBushTag {};
struct AngryBushTag2 {};
struct CrusherTag {};
struct CrusherAttackTag {};
struct SubjectTag {};
struct DestructibleTag {};
struct ChestTag {};
struct SpawnTag {};
struct Chunk0Tag {};
struct Chunk1Tag {};
struct Chunk2Tag {};
struct Chunk3Tag {};
struct Chunk4Tag {};
struct Chunk5Tag {};
struct Chunk6Tag {};
struct NoDamageTag {};
struct SeparateModelTag {};
struct BarricadeTag {};
struct LeverTag {};
struct NoKeyTag {};
struct NPCTag {};
struct LevelChangeTag {};
struct CoinTag {};
struct WaterBombTag {};
struct NomadTag {};
struct LavaTag {};
struct LadderTag {};
struct SignTag {};
struct TableTag {};
struct InvestigatorTag {};
struct MissionObjTag {};
struct BoatTag {};
struct ObstacleTag {};
struct FireBallTag {};

//PatrolComponent, ShootPlayerComponent, RandomShootComponent, DiagonalComponent, DrakeComponent,
using CL = MP::TypeList <
    PhysicsComponent,
    RenderComponent,
    InputComponent,
    LifeComponent,
    ColliderComponent,
    RampComponent,
    AIComponent,
    AttackComponent,
    ProjectileComponent,
    ObjectComponent,
    ZoneComponent,
    ShieldComponent,
    TypeComponent,
    ChestComponent,
    ListenerComponent,
    DestructibleComponent,
    InteractiveComponent,
    SubjectComponent,
    AngryBushComponent,
    DispatcherComponent,
    Navmesh,
    OneUseComponent,
    NPCComponent,
    LadderComponent,
    MessageComponent,
    BoatComponent
> ;
using TL = MP::TypeList <
    PlayerTag,
    EnemyTag,
    HitPlayerTag,
    GroundTag,
    WallTag,
    ObjectTag,
    ZoneTag,
    DoorTag,
    RampTag,
    SlimeTag,
    SnowmanTag,
    GolemTag,
    SpiderTag,
    BossFinalTag,
    SubjectTag,
    DestructibleTag,
    ChestTag,
    SpawnTag,
    Chunk0Tag,
    Chunk1Tag,
    Chunk2Tag,
    Chunk3Tag,
    Chunk4Tag,
    Chunk5Tag,
    Chunk6Tag,
    NoDamageTag,
    SeparateModelTag,
    DummyTag,
    AngryBushTag,
    AngryBushTag2,
    CrusherTag,
    BarricadeTag,
    LeverTag,
    NoKeyTag,
    NPCTag,
    LevelChangeTag,
    CoinTag,
    WaterBombTag,
    FireBallTag,
    NomadTag,
    LavaTag,
    LadderTag,
    SignTag,
    TableTag,
    InvestigatorTag,
    MissionObjTag,
    BoatTag,
    ObstacleTag
> ;
using SCL = MP::TypeList<LevelInfo, BlackBoard_t, Debug_t, InputInfo, PlayerInfo, TextInfo, ZoneCheckInfo, GameData, NavmeshInfo, SoundSystem>;
using EntityManager = ETMG::EntityManager<CL, SCL, TL>;
using Entity = EntityManager::Entity;
using GameEngine = ENGI::GameEngine;
using deathSet = std::set<std::size_t, std::greater<std::size_t>>;
using mapType = rapidjson::Document;
using valueType = rapidjson::Value;
using mapSizeType = rapidjson::SizeType;
