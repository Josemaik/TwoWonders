#pragma once

#include "../components/physics_component.hpp"
#include "../components/render_component.hpp"
#include "../managers/entity_manager.hpp"
#include "../utils/meta_program.hpp"

// Forward Declaration de GameEngine
namespace ENGI { struct GameEngine; }

// Tags - Colocar en TL
struct PlayerTag {};

using CL = MP::TypeList<PhysicsComponent, RenderComponent>;
using TL = MP::TypeList<PlayerTag>;
using EntityManager = ETMG::EntityManager<CL, TL>;
using Entity = EntityManager::Entity;
using GameEngine = ENGI::GameEngine;