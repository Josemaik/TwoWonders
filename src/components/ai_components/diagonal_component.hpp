#pragma once
#include <array>
#include <cstdint>
#include <utils/vec3D.hpp>
#include <random>
struct DiagonalComponent : AIComponent
{
    static constexpr vec3f up_left =    vec3f{ -0.25f, 0.0f, -0.25f };
    static constexpr vec3f up_right =   vec3f{ 0.25f, 0.0f, -0.25f };
    static constexpr vec3f down_left =  vec3f{ -0.25f, 0.0f, 0.25f };
    static constexpr vec3f down_right = vec3f{ 0.25f, 0.0f, 0.25f };
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
    bool stoped{false}, moving{false};
    vec3f oldvel{0.25f,0.0f,0.0f};
    float countdown_change_dir{ 1.5f },countdown_stop{3.5f},countdown_moving{2.0f}; //segundos
    float elapsed_stop{1.0f},elapsed_change_dir{1.0f},elapsed_moving{1.0f}; //segundos
    void dec_countdown_change_dir(float deltaTime) { elapsed_change_dir += deltaTime; }; // delta time
    void dec_countdown_stop(float deltaTime) { elapsed_stop += deltaTime; }; 
    void dec_countdown_moving(float deltaTime) { elapsed_moving += deltaTime; };
    float Xmin{},Xmax{},Zmin{},Zmax{};
};
