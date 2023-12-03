#pragma once
#ifndef AI_SYSTEM
#define AI_SYSTEM
#include "../utils/types.hpp"
#include "../utils/vec3D.hpp"
#include <iostream>

constexpr float SPEED_AI = .2f;

struct AISystem
{
    // Se van a buscar las entidad que tengan estos componentes y tags
    using SYSCMPs = MP::TypeList<PhysicsComponent, AIComponent>;
    using SYSTAGs = MP::TypeList<>;

    void update(EntityManager& em);
    [[nodiscard]] bool isPlayerDetected(EntityManager& EM, PhysicsComponent const& p, AIComponent const& ai) const noexcept;
    [[nodiscard]] vec3f getPlayerDistance(EntityManager& EM, PhysicsComponent const& p, AIComponent& ai) const noexcept;
    void FollowPatrol(AIComponent& ai,PhysicsComponent& p);
};

#endif // !AI_SYSTEM