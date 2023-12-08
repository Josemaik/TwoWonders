#pragma once
#ifndef AI_SYSTEM
#define AI_SYSTEM
#include "../utils/types.hpp"
#include "../utils/vec3D.hpp"
#include <iostream>

constexpr float SPEED_AI = 0.2f;

struct AISystem
{
    // Se van a buscar las entidad que tengan estos componentes y tags
    using SYSCMPs = MP::TypeList<PhysicsComponent, AIComponent>;
    using SYSTAGs = MP::TypeList<>;

    void update(EntityManager& em,float dt);
    //Obtain player data
    [[nodiscard]] bool isPlayerDetected(EntityManager& EM, PhysicsComponent const& p, AIComponent const& ai) const noexcept;
    [[nodiscard]] vec3f getPlayerDistance(EntityManager& EM, PhysicsComponent const& p, AIComponent& ai) const noexcept;
    // AI functions
    vec3f FollowPatrol(AIComponent& ai, PhysicsComponent& p);
    void FollowPatrolandShoot(AIComponent& ai, PhysicsComponent& p, EntityManager& em, Entity& e,float dt);
    void ShotandMove(AIComponent& ai, PhysicsComponent& p, EntityManager& em, Entity& e,float dt);
    void RandomAI(AIComponent& ai,PhysicsComponent& p,EntityManager& em,float dt);
    //Operations
    void setVelocity(PhysicsComponent& p, AIComponent& ai, vec3f distance,float dt);
    vec3f getVelocityonDirecion(vec3f vector);
    
};

#endif // !AI_SYSTEM