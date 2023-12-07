#pragma once

// Components - Colocar en CL
#include "../components/collider_component.hpp"
#include "../components/physics_component.hpp"
#include "../components/render_component.hpp"
#include "../components/attack_component.hpp"
#include "../components/input_component.hpp"
#include "../components/life_component.hpp"
#include "../components/ai_component.hpp"
#include "../components/levelinfo.hpp"
#include "../components/projectile_component.hpp"
#include "../components/object_component.hpp"
#include "../components/information_component.hpp"
#include "../managers/entity_manager.hpp"
#include "../utils/meta_program.hpp"

// Forward Declarations
namespace ENGI { struct GameEngine; }

// Tags - Colocar en TL
struct PlayerTag {};
struct EnemyTag {};
struct HitPlayer {};
struct GroundTag {};
struct WaterTag {};
struct Object {};

using CL = MP::TypeList<PhysicsComponent, RenderComponent, InputComponent, LifeComponent, ColliderComponent, AIComponent, AttackComponent, ProjectileComponent, ObjectComponent, InformationComponent>;
using TL = MP::TypeList<PlayerTag, EnemyTag, HitPlayer, GroundTag, WaterTag, Object>;
using SCL = MP::TypeList<LevelInfo>;
using EntityManager = ETMG::EntityManager<CL, SCL, TL>;
using Entity = EntityManager::Entity;
using GameEngine = ENGI::GameEngine;