#pragma once
#include <cstdint>
#include <array>
#include <random>
#include "utils/vec3D.hpp"

//fordwarding
struct BehaviourTree_t;

struct AIComponent
{
    //type of shoots
    enum class TypeShoot: uint8_t{OneShootonDir,OneShoottoPlayer,TripleShoot};
    // TypeShoot currentshoot{};
    //default velocity
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
    //se llaman desde collision system
    vec3f getRandomDirection(vec3f dir1,vec3f dir2,vec3f dir3);
    vec3f getRandomDirectionWithDiagonals(vec3f dir1,vec3f dir2,vec3f dir3,vec3f dir4,vec3f dir5);
    vec3f getRandomDir();
    vec3f getRandomDirwithDiagonals();
    //Flags
    bool  detectplayer{false},ghost{false};
    //before velocity
    vec3f oldvel{0.25f,0.0f,0.0f};
    //Rangos
    float Xmin{},Xmax{},Zmin{},Zmax{},visible{true};
    bool isInDesiredRange(const vec3f& direction, float xmin, float xmax, float zmin, float zmax);
    vec3f getRandomPosinRange(float xmin, float xmax, float zmin, float zmax);
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
