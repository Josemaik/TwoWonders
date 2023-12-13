#pragma once
#include <array>
#include <cstdint>
#include <utils/vec3D.hpp>
struct RandomShootComponent
{
    bool stoped{false}, shoot{false};
    vec3f oldvel{0.25f,0.0f,0.0f};
    float countdown_change_dir{ 1.5f },countdown_stop{3.5f},countdown_shoot{2.0f}; //segundos
    float elapsed_stop{1.0f},elapsed_change_dir{1.0f},elapsed_shoot{1.0f}; //segundos
    void dec_countdown_change_dir(float deltaTime) { elapsed_change_dir += deltaTime; }; // delta time
    void dec_countdown_stop(float deltaTime) { elapsed_stop += deltaTime; }; 
    void dec_countdown_shoot(float deltaTime) { elapsed_shoot += deltaTime; };
    float Xmin{},Xmax{},Zmin{},Zmax{};
};
