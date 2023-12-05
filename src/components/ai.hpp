#pragma once
#include <array>
#include <cstdint>
#include <utils/vec3D.hpp>
struct AIComponent
{
    enum struct AI_type{PatrolEnemy,PatrolFollowEnemy,ShoterEnemy,ShoterEnemy2};
    AI_type current_type{};
    static constexpr vec3f invalid{};
    static constexpr uint8_t max_patrol { 10 };
    std::array<vec3f,max_patrol> patrol{};
    uint8_t current{};
    float arrival_radius { 1.f };
    float detect_radius { 5.f };
    bool playerdetected {false};
    bool shooting {false};
    bool arrived{false};
    int contador_stop{50};
    int contador_change_position{70};
    uint8_t nexttarget{1};
    enum struct AI_shoot_states{shooting,moving};
    AI_shoot_states current_state {AI_shoot_states::shooting};
};
