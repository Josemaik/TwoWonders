#pragma once
#ifndef EVENT_SYSTEM
#define EVENT_SYSTEM
#include "../utils/types.hpp"

struct EventSystem
{
    void update(EventManager& evm, ObjectSystem& os, EntityManager& em) { evm.dispatchEvents(em, os); };
};

#endif // !EVENT_SYSTEM
