#pragma once
#include <utils/types.hpp>

namespace STBH{
    bool Arrive(AIComponent& ai,PhysicsComponent& phy,Entity& ent);
    void Seek(AIComponent& ai,PhysicsComponent& phy);
    void Flee(AIComponent& ai,PhysicsComponent& phy);

} //STBH