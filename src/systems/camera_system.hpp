#pragma once
#ifndef CAMERA_SYSTEM
#define CAMERA_SYSTEM

#include "../utils/types.hpp"
#include "../managers/game_engine.hpp"
#include "../managers/event_manager.hpp"

struct CameraSystem
{
    void update(EntityManager& em, GameEngine& ge, EventManager& evm, float alpha);

private:
    void updateFrustum(EntityManager& em, GameEngine& ge, vec3d& cameraPos, vec3d& cameraTar, float cameraFovy);

    vec3d cameraPos{}, cameraTar{};
    float cameraFovy{ 20.f }, transitionTime{ 0.f }, transitionLimit{ 0.4f }, viewPointTime{ 0.f }, viewPointLimit{ 1.35f };
    float elapsedFrutum{ 0.0f }, limitFrustum{ 0.4f };
};

#endif