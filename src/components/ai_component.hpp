#pragma once
#include <cstdint>
#include <array>
#include <random>
#include <utils/path.hpp>

// Forwarding
struct BehaviourTree_t;

// enum class SB {
//       Arrive
//     , Seek
//     , Flee
//     , Avoid
// }
struct AIComponent
{
    // Type of shoots
    enum class TypeShoot : uint8_t { OneShootonDir, OneShoottoPlayer, TripleShoot };
    // TypeShoot currentshoot{};
    // Default velocity
    double SPEED_AI = 0.2;
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
    bool playerdetected{ false };
    // data for cinematic test
    // posicion objetivo
    double tx { 0 }, tz { 0 };
    //tiempor para llegar al objetivo
    double time2arrive { 0.5 };
    //activar o no comportamiento
    bool tactive { false };
    //perception time ( couldown )
    float perceptionTime { 0.1f }; // Frequency inverse
    float accumulated_dt { 0.0f };
    //Target Entity
    std::size_t teid{};
    //PATH
    Path_t<4>::iterator pathIt {};
    // SB behaviour {SB::Arrive};
    // Timers
    double countdown_change_dir{ 1.5 }, countdown_stop{ 3.5 }, countdown_shoot{ 2.0 }, countdown_change_position{ 3.0 }; // seconds
    double elapsed_change_position{ 1.0 }, elapsed_stop{ 1.0 }, elapsed_change_dir{ 1.0 }, elapsed_shoot{ 1.0 };
    void dec_countdown_change_pos(double deltaTime) { elapsed_change_position += deltaTime; };
    void dec_countdown_change_dir(double deltaTime) { elapsed_change_dir += deltaTime; }; // delta time
    void dec_countdown_stop(double deltaTime) { elapsed_stop += deltaTime; };
    void dec_countdown_shoot(double deltaTime) { elapsed_shoot += deltaTime; };
    // Behaviour trees
    BehaviourTree_t* behaviourTree{ nullptr };
};
