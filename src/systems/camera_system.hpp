#pragma once
#ifndef CAMERA_SYSTEM
#define CAMERA_SYSTEM

#include "../utils/types.hpp"
#include "../managers/game_engine.hpp"
#include "../managers/event_manager.hpp"

struct CameraSystem
{
    void update(EntityManager& em, GameEngine& ge, EventManager& evm);

private:
    vec3d cameraPos{}, cameraTar{};
    float cameraFovy{ 20.f }, transitionTime{ 0.f }, transitionLimit{ 0.4f }, viewPointTime{ 0.f }, viewPointLimit{ 1.35f };
};

#endif