#pragma once

#include "../utils/types.hpp"

struct ParticleSystem
{
    // Se van a buscar las entidad que tengan estos componentes y tags
    using SYSCMPs = MP::TypeList<PhysicsComponent, ParticleMakerComponent>;
    using SYSTAGs = MP::TypeList<>;

    void update(EntityManager& em);
};