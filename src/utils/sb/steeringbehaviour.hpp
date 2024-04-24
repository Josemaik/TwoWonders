#pragma once
#include <utils/types.hpp>

struct Steer_t {
    double v_x{ 0.0 };
    double v_z{ 0.0 };
    double orientation{ 0.0 };
    bool arrived{ false };
    //   constexpr bool operator==(Steer_t const&) const noexcept = default;
};

namespace STBH {
    Steer_t Arrive(PhysicsComponent const& phy, vec3d const& target, double const arrivalRadious);
    Steer_t Seek(PhysicsComponent const& phy, vec3d const& target);
    Steer_t Flee(PhysicsComponent const& phy, vec3d const& enemy);
    Steer_t Pursue(PhysicsComponent const& phyTarget, PhysicsComponent const& phyPursuer,double maxspeed);
    Steer_t Avoid(PhysicsComponent const& phyTarget, PhysicsComponent const& phyPursuer);
    Steer_t Evade(PhysicsComponent const& phyEvader,PhysicsComponent const& phytoevade,double maxSeparationForce);
} //STBH
