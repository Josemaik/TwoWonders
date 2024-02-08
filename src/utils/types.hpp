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
#include "../components/lock_component.hpp"
//ia
#include "../components/ai_component.hpp"
#include "../components/projectile_component.hpp"
#include "../components/object_component.hpp"
#include "../components/zone_component.hpp"
#include "../components/shield_component.hpp"
#include "../managers/entity_manager.hpp"
#include "../utils/meta_program.hpp"
//singelton
#include "../utils/sngtn/player_info.hpp"
#include "../utils/sngtn/blackboard.hpp"
#include "../utils/sngtn/debug_singleton.hpp"
#include "../utils/sngtn/levelinfo.hpp"
#include "../utils/sngtn/input_info.hpp"
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

// Constants
static constexpr float DEGTORAD = static_cast<float>(M_PI / 180.0);

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
struct SubditoTag {};

//PatrolComponent, ShootPlayerComponent, RandomShootComponent, DiagonalComponent, DrakeComponent,
using CL = MP::TypeList<PhysicsComponent, RenderComponent, InputComponent, LifeComponent, ColliderComponent, RampComponent, AIComponent, AttackComponent, ProjectileComponent, ObjectComponent, ZoneComponent, ShieldComponent, TypeComponent, LockComponent>;
using TL = MP::TypeList<PlayerTag, EnemyTag, HitPlayerTag, GroundTag, WaterTag, WallTag, ObjectTag, ZoneTag, DoorTag, RampTag, SlimeTag, SnowmanTag, GolemTag, SpiderTag,BossFinalTag,SubditoTag>;
using SCL = MP::TypeList<LevelInfo, BlackBoard_t, Debug_t, InputInfo, PlayerInfo>;
using EntityManager = ETMG::EntityManager<CL, SCL, TL>;
using Entity = EntityManager::Entity;
using GameEngine = ENGI::GameEngine;
using deathSet = std::set<std::size_t, std::greater<std::size_t>>;
using mapType = rapidjson::Document;
