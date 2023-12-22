#pragma once
#ifndef EVENT_SYSTEM
#define EVENT_SYSTEM

#include <managers/eventmanager.hpp>
#include "../utils/types.hpp"

struct EventSystem
{
    void update(Eventmanager& evm,EntityManager& em){ evm.dispatchEvents(em); };
};

#endif 