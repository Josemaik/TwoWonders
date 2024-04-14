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
    Path_t<5> path_investigador{ vec3d{-57.6809,14,-109.053},
        vec3d{-98.2402,14,-68.1948},
        vec3d{-79.1414,5,-31.3527},
        vec3d{-83.5963,5,12.6932},
        vec3d{-110.859,5,25.9958} };
    Path_t<5>::iterator pathIt_inestigador{ };
    bool path_investigador_finalized{ false };
    bool tp{ false };


    NPCType type{ NPCType::NOMAD };
    const char* bh{};
    BehaviourTree_t* behaviourTree{ nullptr };
    double arrival_radius{ 1.0 };
};
