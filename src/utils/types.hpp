#pragma once

#include "../components/collider_component.hpp"
#include "../components/physics_component.hpp"
#include "../components/render_component.hpp"
#include "../components/input_component.hpp"
#include "../components/life_component.hpp"
#include "../components/ai.hpp"
#include "../managers/entity_manager.hpp"
#include "../utils/meta_program.hpp"

// Forward Declaration de GameEngine
namespace ENGI { struct GameEngine; }

// Tags - Colocar en TL
struct PlayerTag {};
struct EnemyTag {};

using CL = MP::TypeList<PhysicsComponent, RenderComponent, InputComponent, LifeComponent, ColliderComponent,AIComponent>;
using TL = MP::TypeList<PlayerTag, EnemyTag>;
using EntityManager = ETMG::EntityManager<CL, TL>;
using Entity = EntityManager::Entity;
using GameEngine = ENGI::GameEngine;