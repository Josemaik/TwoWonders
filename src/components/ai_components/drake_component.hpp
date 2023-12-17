#pragma once
#include <array>
#include <cstdint>
#include <utils/vec3D.hpp>
struct BehaviourTree_t;
struct DrakeComponent
{
    //patrol enemy
    static constexpr uint8_t max_patrol{ 10 };
     std::array<vec3f, max_patrol> patrol{};
    static constexpr vec3f invalid{};
    uint8_t current{};
    float arrival_radius{ 1.f };
    bool arrived{ false };
    uint8_t nexttarget{ 1 };
    float countdown_shoot{2.0f}, elapsed_shoot{1.0f};
    void dec_countdown_shoot(float deltaTime) { elapsed_shoot += deltaTime; };
    BehaviourTree_t* behaviourTree{nullptr};
};
