#pragma once
#include <cstdint>
//IAs
#include <components/ai_components/diagonal_component.hpp>
#include <components/ai_components/drake_component.hpp>
#include <components/ai_components/patrol_component.hpp>
#include <components/ai_components/randomshoot_component.hpp>
#include <components/ai_components/shootplayer_component.hpp>
//physics y entitymanager
#include <components/physics_component.hpp>
#include <managers/entity_manager.hpp>

struct EntityContext_t{
    //Referencia a EntityManager
    EntityManager& em;
    Entity& ent;
    //AI
    PatrolComponent& pc;
    // RandomShootComponent& rsc;
    // ShootPlayerComponent& spc;
    // DiagonalComponent& dc;
    // DrakeComponent& drc;
    //physics
    PhysicsComponent& phy;
};
enum class BTNodeStatus_t : uint8_t{
      success
    , fail
    , running
};

struct BTNode_t{
    //Custom deleter
    struct Deleter { void operator()(BTNode_t* n) {n->~BTNode_t(); } };
    //Constructor por defecto y destructor virtual
    BTNode_t() = default;
    virtual ~BTNode_t() = default;
    //No permitir constructores copia ni asignación
    BTNode_t(const BTNode_t&)            = delete;
    BTNode_t(BTNode_t&&)                 = delete;
    BTNode_t& operator=(const BTNode_t&) = delete;
    BTNode_t& operator=(BTNode_t&&)      = delete;
    //Constructor virtual
    virtual BTNodeStatus_t run(EntityContext_t& ectx) noexcept = 0;
};