#pragma once
#include <utils/path.hpp>
#include <array>

enum struct NPCType : uint8_t
{
    NOMAD,
    INVESTIGATOR,
};

struct NPCComponent
{
    //path tamaño 4
    Path_t<4> path{ };
    Path_t<4>::iterator pathIt{ };
    bool path_initialized{ false };
    bool path_finaliced{ false };
    //path para investigador
    // Path_t<3> path_investigador{ vec3d{-57.6809,14,-109.053},
    // vec3d{-98.2402,14,-68.1948},
    // vec3d{-82.4083,14,-67.723} };
    // Path_t<3>::iterator pathIt_investigadorIt{ };

    Path_t<7> path_investigador{ vec3d{-57.6809,14,-109.053},
        vec3d{-98.2402,14,-68.1948},
        vec3d{-79.1414,5,-31.3527},
        vec3d{-83.5963,5,12.6932},
        vec3d{-80.0, 5.0, 45.0},
        vec3d{-93.282, 5.0, 45.0},
        vec3d{ -93.282, 5.0, 42.465 } };

    Path_t<7>::iterator pathIt_inestigador{ };
    // Path_t<5> path_investigadorfinal{ vec3d{-83.1301,6.5,-38.8823},
    // vec3d{-82.1499,6.5,-3.6431},
    // vec3d{-89.528,6.5,-2.66291},
    // vec3d{-94.429,6.5,26.1783},
    // vec3d{-110.859,6.5,26.1783}};
    // Path_t<5>::iterator path_investigadorfinalIt{};

    bool path_investigador_finalized{ false };
    bool goto_boat{false};
    //para nomada
    bool tp{ false };

    NPCType type{ NPCType::NOMAD };
    const char* bh{};
    BehaviourTree_t* behaviourTree{ nullptr };
    double arrival_radius{ 1.0 };
};
