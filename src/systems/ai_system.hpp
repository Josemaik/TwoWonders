#pragma once
#ifndef AI_SYSTEM
#define AI_SYSTEM
#include "../utils/types.hpp"
#include "../utils/vec3D.hpp"
#include "../utils/bt/behaviourtree.hpp"
#include <iostream>
//behaviour tree , node sequece and node selector
#include "../utils/bt/behaviourtree.hpp"
#include "../utils/bt/nodesequence.hpp"
#include "../utils/bt/nodeselector.hpp"
//actions
#include "../utils/bt/action_patrol.hpp"
#include "../utils/bt/action_randommovement.hpp"
#include "../utils/bt/action_changeposition.hpp"
#include "../utils/bt/action_diagonalmovement.hpp"
#include "../utils/bt/action_shoot.hpp"
#include "../utils/bt/action_arrive.hpp"
#include "../utils/bt/action_seek.hpp"
#include "../utils/bt/action_flee.hpp"
#include "../utils/bt/action_pursue.hpp"
#include "../utils/bt/action_avoid.hpp"
//decision
#include "../utils/bt/decision_playerdetected.hpp"
// constexpr float SPEED_AI = 0.2f;

struct AISystem
{
    // Se van a buscar las entidad que tengan estos componentes y tags
    using SYSCMPs = MP::TypeList<PhysicsComponent,RenderComponent, AIComponent,LifeComponent>;
    using SYSTAGs = MP::TypeList<>;

    void update(EntityManager& em, float dt);
};

#endif // !AI_SYSTEM
