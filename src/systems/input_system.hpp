#pragma once
#ifndef INPUT_MANAGER
#define INPUT_MANAGER
#include <raylib.h>
#include "../utils/types.hpp"

constexpr float SPEED = .4f;

struct InputSystem
{
    // Se van a buscar las entidad que tengan estos componentes y tags
    using SYSCMPs = MP::TypeList<PhysicsComponent>;
    using SYSTAGs = MP::TypeList<PlayerTag>;
    void update(EntityManager& em);
    void normalize(Vector3& vel);
};

#endif // !INPUT_MANAGER