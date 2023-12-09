#pragma once
#include <array>
#include <cstdint>
#include <utils/vec3D.hpp>
struct AIComponent
{
    enum struct AI_type { PatrolEnemy, PatrolFollowEnemy, ShoterEnemy, ShoterEnemy2,RandomEnemy };
    AI_type current_type{};
    static constexpr vec3f invalid{};
    static constexpr uint8_t max_patrol{ 10 };
    std::array<vec3f, max_patrol> patrol{};
    uint8_t current{};
    float arrival_radius{ 1.f };
    float detect_radius{ 5.f };
    bool playerdetected{ false };
    bool shooting{ false };
    bool arrived{ false };
    int contador_stop{ 50 };
    int contador_change_position{ 70 };
    uint8_t nexttarget{ 1 };
    enum struct AI_shoot_states { shooting, moving };
    AI_shoot_states current_state{ AI_shoot_states::shooting };
    //random
    bool stoped{false}, shoot{false};
    vec3f oldvel{0.5f,0.0f,0.0f};
    float countdown_change_dir{ 2.0f },countdown_stop{5.0f},countdown_shoot{2.0f}; //segundos
    float elapsed_stop{1.0f},elapsed_change_dir{1.0f},elapsed_shoot{1.0f}; //segundos
    void dec_countdown_change_dir(float deltaTime) { elapsed_change_dir += deltaTime; }; // delta time
    void dec_countdown_stop(float deltaTime) { elapsed_stop += deltaTime; }; 
    void dec_countdown_shoot(float deltaTime) { elapsed_shoot += deltaTime; };
};
