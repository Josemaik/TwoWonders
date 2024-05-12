#pragma once
#ifndef INPUT_MANAGER
#define INPUT_MANAGER
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

private:
    float elapsedLockInLimit{ 1.0f }, elapsedLockIn{ elapsedLockInLimit };
    float elapsedParticleLimit{ 1.5f }, elapsedParticle{ elapsedParticleLimit };
};

#endif // !INPUT_MANAGER
