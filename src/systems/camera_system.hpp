#pragma once
#ifndef CAMERA_SYSTEM
#define CAMERA_SYSTEM

#include "../utils/types.hpp"
#include "../managers/game_engine.hpp"

struct CameraSystem
{
    void update(EntityManager& em, ENGI::GameEngine& ge, double dt);

private:
    vec3d cameraPos{}, cameraTar{};
    float cameraFovy{ 20.f };
    double transitionTime{ 0. }, transitionLimit{ 0.4 }, viewPointTime{ 0. }, viewPointLimit{ 2.7 };
};

#endif