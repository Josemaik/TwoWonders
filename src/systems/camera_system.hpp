#pragma once
#ifndef CAMERA_SYSTEM
#define CAMERA_SYSTEM

#include "../utils/types.hpp"
#include "../managers/game_engine.hpp"

struct CameraSystem
{
    void update(EntityManager& em, ENGI::GameEngine& ge, float dt);

private:
    vec3d cameraPos{};
    vec3d cameraTar{};
    float transitionTime{ 0.f };
    float transitionLimit{ 0.15f };
};

#endif