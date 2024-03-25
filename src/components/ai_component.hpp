#pragma once
#include <cstdint>
#include <array>
#include <random>
#include <utils/path.hpp>

// Forwarding
struct BehaviourTree_t;

// enum class SB : uint8_t {
//     Arrive,
//     Seek,
//     Flee,
//     Pursue,
//     Avoid,
//     followPath
// };
struct AIComponent
{
    // Type of shoots
    enum class TypeShoot : uint8_t { OneShootonDir, OneShoottoPlayer, TripleShoot, Melee, SpiderWeb, Air_attack, CrusherAttack };

    // TypeShoot currentshoot{};
    // Default velocity
    double SPEED_AI = 0.6;
    // Data for patrol behaviour
    static constexpr uint8_t max_patrol{ 10 };
    std::array<vec3d, max_patrol> patrol{};
    static constexpr vec3d invalid{};
    uint8_t current{};
    double arrival_radius{ 1.0 };
    bool arrived{ false };
    uint8_t nexttarget{ 1 };
    // Directions and functions
    static constexpr vec3d left = vec3d{ -0.25, 0.0, 0.0 };
    static constexpr vec3d right = vec3d{ 0.25, 0.0, 0.0 };
    static constexpr vec3d up = vec3d{ 0.0, 0.0, -0.25 };
    static constexpr vec3d down = vec3d{ 0.0, 0.0, 0.25 };
    static constexpr vec3d up_left = vec3d{ -0.25, 0.0, -0.25 };
    static constexpr vec3d up_right = vec3d{ 0.25, 0.0, -0.25 };
    static constexpr vec3d down_left = vec3d{ -0.25, 0.0, 0.25 };
    static constexpr vec3d down_right = vec3d{ 0.25, 0.0, 0.25 };
    // Called from collision system
    vec3d getRandomDirection(vec3d dir1, vec3d dir2, vec3d dir3);
    vec3d getRandomDirectionWithDiagonals(vec3d dir1, vec3d dir2, vec3d dir3, vec3d dir4, vec3d dir5);
    vec3d getRandomDir();
    vec3d getRandomDirwithDiagonals();
    // Flags
    bool detectplayer{ false }, ghost{ false };
    // Before velocity
    vec3d oldvel{ 0.25, 0.0, 0.0 };
    // Ranges
    double Xmin{}, Xmax{}, Zmin{}, Zmax{}, visible{ true };
    bool isInDesiredRange(const vec3d& direction, double xmin, double xmax, double zmin, double zmax);
    vec3d getRandomPosinRange(double xmin, double xmax, double zmin, double zmax);
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
    // data for steering behaviour
    // SB behaviour { SB::Arrive };
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
    //area
    bool attackbeforedie{ false };
    //PATH
    Path_t<4> path{ };
    Path_t<4>::iterator pathIt{ };
    bool path_initialized{ false };
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
    double elapsed_change_position{ 1.0 }, elapsed_stop{ 1.0 }, elapsed_change_dir{ 1.0 }, elapsed_shoot{ 1.0 },
        elapsed_fleeing{ 1.0 }, elapsed_perception{ 1.0 }, elapsed_spawning{ 1.0 }, elapsed_heal{ 1.0 }, elapsed_shield{ 1.0 },
        elapsed_air_attack{ 1.0 }, elapsed_show_icon{ 1.0 };

    void plusDeltatime(float deltaTime, double& elapsed) { elapsed += deltaTime; };
    // Behaviour trees
    const char* bh{};
    BehaviourTree_t* behaviourTree{ nullptr };
};
