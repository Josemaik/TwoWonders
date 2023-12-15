#pragma once 
#ifndef SHIELD_SYSTEM
#define SHIELD_SYSTEM
#include "../utils/types.hpp"

struct ShieldSystem
{
    using SYSCMPs = MP::TypeList<ShieldComponent>;
    using SYSTAGs = MP::TypeList<>;

    void update(EntityManager& em);
};

#endif // !SHIELD_SYSTEM