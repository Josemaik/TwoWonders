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
//ia
#include "../components/ai_component.hpp"
#include "../components/projectile_component.hpp"
#include "../components/object_component.hpp"
#include "../components/zone_component.hpp"
#include "../components/shield_component.hpp"
#include "../managers/entity_manager.hpp"
#include "../utils/meta_program.hpp"
#include "../utils/Item.hpp"
//singelton
#include "../utils/sngtn/player_info.hpp"
#include "../utils/sngtn/blackboard.hpp"
#include "../utils/sngtn/debug_singleton.hpp"
#include "../utils/sngtn/level_info.hpp"
#include "../utils/sngtn/input_info.hpp"
#include "../utils/sngtn/text_info.hpp"
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include "../systems/sound_system.hpp"

// Constants
static constexpr double K_PI = 3.14159265358979323846;
static constexpr float DEGTORAD = static_cast<float>(K_PI / 180.0);
static constexpr float timeStep = 1.0f / 30.0f;  // Actualiza el juego 30 veces por segundo
static constexpr float timeStep60 = 1.0f / 60.0f;  // Actualiza el juego 60 veces por segundo

// Forward Declarations
namespace ENGI { struct GameEngine; }

// Tags - Colocar en TL
struct PlayerTag {};
struct EnemyTag {};
struct HitPlayerTag {};
struct GroundTag {};
struct WaterTag {};
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
struct NoDamageTag {};
struct SeparateModelTag {};
struct BarricadeTag {};

//PatrolComponent, ShootPlayerComponent, RandomShootComponent, DiagonalComponent, DrakeComponent,
using CL = MP::TypeList<PhysicsComponent, RenderComponent, InputComponent, LifeComponent, ColliderComponent, RampComponent, AIComponent, AttackComponent, ProjectileComponent, ObjectComponent, ZoneComponent, ShieldComponent, TypeComponent, ChestComponent, ListenerComponent, DestructibleComponent, InteractiveComponent, SubjectComponent, AngryBushComponent, DispatcherComponent>;
using TL = MP::TypeList<PlayerTag, EnemyTag, HitPlayerTag, GroundTag, WaterTag, WallTag, ObjectTag, ZoneTag, DoorTag, RampTag, SlimeTag, SnowmanTag, GolemTag, SpiderTag, BossFinalTag, SubjectTag, DestructibleTag, ChestTag, SpawnTag, Chunk0Tag, Chunk1Tag, Chunk2Tag, NoDamageTag, SeparateModelTag, DummyTag, AngryBushTag, AngryBushTag2, CrusherTag, BarricadeTag>;
using SCL = MP::TypeList<LevelInfo, BlackBoard_t, Debug_t, InputInfo, PlayerInfo, TextInfo, SoundSystem>;
using EntityManager = ETMG::EntityManager<CL, SCL, TL>;
using Entity = EntityManager::Entity;
using GameEngine = ENGI::GameEngine;
using deathSet = std::set<std::size_t, std::greater<std::size_t>>;
using mapType = rapidjson::Document;
