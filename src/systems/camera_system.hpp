#pragma once
#ifndef CAMERA_SYSTEM
#define CAMERA_SYSTEM

#include "../utils/types.hpp"
#include "../managers/game_engine.hpp"

struct CameraSystem
{
    void update(EntityManager& em, ENGI::GameEngine& ge);
};

#endif