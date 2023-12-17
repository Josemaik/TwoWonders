#pragma once
#include <array>
#include <cstdint>
#include <utils/vec3D.hpp>
struct BehaviourTree_t;
struct ShootPlayerComponent
{
    float detect_radius{ 15.f };
    bool playerdetected{ false };
    bool shoot{false};
    float countdown_change_position{5.0f}, elapsed_change_position{1.0f};
    float countdown_shoot_rap{2.0f}, elapsed_shoot_rap{1.0f};
    void dec_countdown_change_pos(float deltaTime) { elapsed_change_position += deltaTime; };
    void dec_countdown_shoot_rap(float deltaTime) { elapsed_shoot_rap += deltaTime; };
    float Xmin{},Xmax{},Zmin{},Zmax{};
    BehaviourTree_t* behaviourTree{nullptr};
};
