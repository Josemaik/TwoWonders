#pragma once
#include <cstdint>
#include <array>
#include <random>
#include <utils/path.hpp>

// Forwarding
struct BehaviourTree_t;

struct AIComponent
{
    // Type of shoots
    enum class TypeShoot : uint8_t { OneShootonDir, OneShoottoPlayer, TripleShoot, Melee, SpiderWeb, Air_attack, CrusherAttack };

    // TypeShoot currentshoot{};
    // Default velocity
    double SPEED_AI = 0.6;
    double arrival_radius{ 1.0 };
    bool arrived{ false };
    // uint8_t nexttarget{ 1 };
    // Flags
    bool detectplayer{ false }, ghost{ false };
    // Before velocity
    vec3d oldvel{ 0.25, 0.0, 0.0 };
    // Data for detect player
    double detect_radius{ 15.0 };
    double attack_radius{ 6.0 };
    bool on_attack_radius{ false };
    bool playerdetected{ false };
    double field_of_view{ 100 };
    //Estado de alerta
    bool alert_state{ false }, show_icon{ true }, listen_steps{ false };
    float endangle{ 0.0f }, increase_angle{ 0.0f };
    //Ataque
    bool ready_attack{ false };
    // posicion objetivo
    double tx{ 0 }, tz{ 0 };
    //tiempor para llegar al objetivo
    double time2arrive{ 0.5 };
    //activar o no comportamiento
    bool tactive{ false };
    //perception time ( couldown )
    float perceptionTime{ 0.1f }; // Frequency inverse
    float accumulated_dt{ 0.0f };
    //Target Entity
    std::size_t teid{};
    //heal
    bool healbeforedie{ false };
    double slimex{}, slimez{};
    std::size_t slimetarget{};
    //flocking
    bool ispushed{false};
    
    //area
    bool attackbeforedie{ false };
    //PATH
    Path_t<4> path{ };
    Path_t<4>::iterator pathIt{ };
    bool path_initialized{ false };
    bool followpatrol{true};
    // SB behaviour {SB::Arrive};
    double txp{}, tzp{};
    bool target_obtained{ false };
    //velocity boofer
    bool boofedvelocity{ false };
    // Timers fase 1 //spawn 0.5 heal 2.0
        //    fase 2 // spawn 0.25 heal 1.0 shield 0.4
    bool chargeattack{ false };
    double countdown_change_dir{ 1.5 }, countdown_stop{ 0.8 }, countdown_shoot{ 0.5 }, countdown_change_position{ 3.0 }
        , countdown_fleeing{ 3.0 }, countdown_perception{ 0.5 }, couldown_spawning{ 0.5 }, countdown_heal{ 1.5 }, countdown_shield{ 0.7 },
        countdown_air_attack{ 1.0 }, countdown_show_icon{ 7.0 }; // seconds
    double elapsed_change_position{ 1.0 }, elapsed_stop{ 1.0 }, elapsed_change_dir{ 1.0 }, elapsed_shoot{ 0.0 },
        elapsed_fleeing{ 1.0 }, elapsed_perception{ 1.0 }, elapsed_spawning{ 1.0 }, elapsed_heal{ 1.0 }, elapsed_shield{ 1.0 },
        elapsed_air_attack{ 1.0 }, elapsed_show_icon{ 1.0 };

    void plusDeltatime(float deltaTime, double& elapsed) { elapsed += deltaTime; };
    // Behaviour trees
    const char* bh{};
    BehaviourTree_t* behaviourTree{ nullptr };
};
