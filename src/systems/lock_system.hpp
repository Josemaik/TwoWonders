#pragma once
#ifndef LOCK_SYSTEM_HPP
#define LOCK_SYSTEM_HPP

#include "../utils/types.hpp"

struct LockSystem
{
    using SYSCMPs = MP::TypeList<PhysicsComponent, ColliderComponent>;
    using SYSTAGs = MP::TypeList<>;

    void update(EntityManager& em);
    void reset() { enemies.clear(); }
private:
    std::vector<std::pair<std::size_t, double>> enemies{};
};

#endif // !LOCK_SYSTEM_HPP