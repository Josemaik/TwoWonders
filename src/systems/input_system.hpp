#pragma once
#ifndef INPUT_MANAGER
#define INPUT_MANAGER
#include <raylib.h>
#include "../utils/types.hpp"

constexpr float SPEED = .4f;

struct InputSystem
{
    // Se van a buscar las entidad que tengan estos componentes y tags
    using SYSCMPs = MP::TypeList<PhysicsComponent, InputComponent>;
    using SYSTAGs = MP::TypeList<PlayerTag>;
    void update(EntityManager& em);
};

#endif // !INPUT_MANAGER