#pragma once
#include <utils/types.hpp>
#include <cstdint>
#include <vector>
#include <random>
// #include "components/ai_components/diagonal_component.hpp"
// #include "components/ai_components/drake_component.hpp"
// #include "components/ai_components/patrol_component.hpp"
// #include "components/ai_components/shootplayer_component.hpp"
// #include "components/ai_components/randomshoot_component.hpp"
struct BehaviourTree_t;
struct AIComponent
{
    float SPEED_AI = 0.2f;
    //data for patrol behaviour
    static constexpr uint8_t max_patrol{ 10 };
     std::array<vec3f, max_patrol> patrol{};
    static constexpr vec3f invalid{};
    uint8_t current{};
    float arrival_radius{ 1.f };
    bool arrived{ false };
    uint8_t nexttarget{ 1 };
    //directions and functions
    static constexpr vec3f left = vec3f{ -0.25f, 0.0f, 0.0f };
    static constexpr vec3f right = vec3f{ 0.25f, 0.0f, 0.0f };
    static constexpr vec3f up = vec3f{ 0.0f, 0.0f, -0.25f };
    static constexpr vec3f down = vec3f{ 0.0f, 0.0f, 0.25f };
    static constexpr vec3f up_left =    vec3f{ -0.25f, 0.0f, -0.25f };
    static constexpr vec3f up_right =   vec3f{ 0.25f, 0.0f, -0.25f };
    static constexpr vec3f down_left =  vec3f{ -0.25f, 0.0f, 0.25f };
    static constexpr vec3f down_right = vec3f{ 0.25f, 0.0f, 0.25f };
    vec3f getRandomDirection(vec3f dir1,vec3f dir2,vec3f dir3){
        switch (std::rand() % 3) {
            case 0:  return dir3; break;//derecha
            case 1:  return dir2; break;//izquieda
            case 2:  return dir1; break; //Abajo
            default: return dir2; break;
         }
    }
     vec3f getRandomDirectionWithDiagonals(vec3f dir1,vec3f dir2,vec3f dir3,vec3f dir4,vec3f dir5){
        switch (std::rand() % 5) {
            case 0:  return dir3; break;
            case 1:  return dir5; break;
            case 2:  return dir1; break; 
            case 3:  return dir2; break; 
            case 4:  return dir4; break;
            default: return dir2; break;
         }
    }
    //Flags
    bool stoped{false}, shoot{false},moving{false};
    //before velocity
    vec3f oldvel{0.25f,0.0f,0.0f};
    //Rangos
    float Xmin{},Xmax{},Zmin{},Zmax{};
    //data for detect player
    float detect_radius{ 15.f };
    bool playerdetected{ false };
    //timers
    float countdown_change_dir{ 1.5f },countdown_stop{3.5f},countdown_shoot{2.0f},countdown_change_position{3.0f}; //segundos
    float elapsed_change_position{1.0f},elapsed_stop{1.0f},elapsed_change_dir{1.0f},elapsed_shoot{1.0f};
    void dec_countdown_change_pos(float deltaTime) { elapsed_change_position += deltaTime; };
    void dec_countdown_change_dir(float deltaTime) { elapsed_change_dir += deltaTime; }; // delta time
    void dec_countdown_stop(float deltaTime) { elapsed_stop += deltaTime; }; 
    void dec_countdown_shoot(float deltaTime) { elapsed_shoot += deltaTime; };
    //behaviour trees
    BehaviourTree_t* behaviourTree{nullptr};
};
