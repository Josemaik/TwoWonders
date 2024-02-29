#pragma once
#ifndef INPUT_MANAGER
#define INPUT_MANAGER
#include <raylib.h>
#include "../utils/types.hpp"
#include "../managers/game_engine.hpp"

constexpr float INP_SPEED = 5.f;

struct InputSystem
{
    // Se van a buscar las entidad que tengan estos componentes y tags
    using SYSCMPs = MP::TypeList<PhysicsComponent, InputComponent>;
    using SYSTAGs = MP::TypeList<PlayerTag>;
    void update(EntityManager& em, GameEngine& ge);

    bool pressEnter(GameEngine& ge);
};

#endif // !INPUT_MANAGER
