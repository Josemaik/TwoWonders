#pragma once
#include <cstdint>
//IA
#include <components/ai_component.hpp>
//physics y entitymanager
#include <components/physics_component.hpp>
#include <managers/entity_manager.hpp>


struct EntityContext_t {
    //Referencia a EntityManager
    EntityManager& em;
    Entity& ent;
    //AI
    AIComponent* ai;
    // RandomShootComponent& rsc;
    // ShootPlayerComponent& spc;
    // DiagonalComponent& dc;
    // DrakeComponent& drc;
    //Components
    NPCComponent* npc;
    PhysicsComponent& phy;
    RenderComponent& ren;
    LifeComponent* lifec;
    float deltaTime;
};
enum class BTNodeStatus_t : uint8_t {
    success
    , fail
    , running
};

struct BTNode_t {
    //Custom deleter
    struct Deleter { void operator()(BTNode_t* n) { n->~BTNode_t(); } };
    //Constructor por defecto y destructor virtual
    BTNode_t() = default;
    virtual ~BTNode_t() = default;
    //No permitir constructores copia ni asignación
    BTNode_t(const BTNode_t&) = delete;
    BTNode_t(BTNode_t&&) = delete;
    BTNode_t& operator=(const BTNode_t&) = delete;
    BTNode_t& operator=(BTNode_t&&) = delete;
    //Constructor virtual
    virtual BTNodeStatus_t run(EntityContext_t& ectx) noexcept = 0;

    //funciones auxiliares para los nodos
    // Obtengo la posición del player
    PhysicsComponent& getplayerphy(EntityContext_t& ectx) {
        auto& li = ectx.em.getSingleton<LevelInfo>();
        auto* playerEn = ectx.em.getEntityByID(li.playerID);
        //if (not playerEn) return vec3d{}; // No hay player
        // Si hay player
        auto& plphy = ectx.em.getComponent<PhysicsComponent>(*playerEn);
        return plphy;
    };
    //Obtengo el colision component del player
    ColliderComponent& getplayercollider(EntityContext_t& ectx) {
        auto& li = ectx.em.getSingleton<LevelInfo>();
        auto* playerEn = ectx.em.getEntityByID(li.playerID);
        //if (not playerEn) return ColliderComponent{}; // No hay player
        // Si hay player
        auto& col = ectx.em.getComponent<ColliderComponent>(*playerEn);
        return col;
    };
};
