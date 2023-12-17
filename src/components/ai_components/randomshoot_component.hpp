#pragma once
#include <array>
#include <cstdint>
#include <utils/vec3D.hpp>
#include <random>
struct BehaviourTree_t;
struct RandomShootComponent
{
    static constexpr vec3f left = vec3f{ -0.25f, 0.0f, 0.0f };
    static constexpr vec3f right = vec3f{ 0.25f, 0.0f, 0.0f };
    static constexpr vec3f up = vec3f{ 0.0f, 0.0f, -0.25f };
    static constexpr vec3f down = vec3f{ 0.0f, 0.0f, 0.25f };
    vec3f getRandomDirection(vec3f dir1,vec3f dir2,vec3f dir3){
        switch (std::rand() % 3) {
            case 0:  return dir3; break;//derecha
            case 1:  return dir2; break;//izquieda
            case 2:  return dir1; break; //Abajo
            default: return dir2; break;
         }
    }
    bool stoped{false}, shoot{false};
    vec3f oldvel{0.25f,0.0f,0.0f};
    float countdown_change_dir{ 1.5f },countdown_stop{3.5f},countdown_shoot{2.0f}; //segundos
    float elapsed_stop{1.0f},elapsed_change_dir{1.0f},elapsed_shoot{1.0f}; //segundos
    void dec_countdown_change_dir(float deltaTime) { elapsed_change_dir += deltaTime; }; // delta time
    void dec_countdown_stop(float deltaTime) { elapsed_stop += deltaTime; }; 
    void dec_countdown_shoot(float deltaTime) { elapsed_shoot += deltaTime; };
    float Xmin{},Xmax{},Zmin{},Zmax{};
    BehaviourTree_t* behaviourTree{nullptr};
};
