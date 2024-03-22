#pragma once
#ifndef EVENT_SYSTEM
#define EVENT_SYSTEM
#include "../utils/types.hpp"

struct EventSystem
{
    void update(EntityManager& em, EventManager& evm, Ia_man& iam, MapManager& mm, ObjectSystem& os, SoundSystem& ss) { evm.dispatchEvents(em, mm, iam, os, ss); };
};

#endif // !EVENT_SYSTEM
