#pragma once

#include <cstdint>
#include <utils/path.hpp>
#include <utils/vec3D.hpp>

//Estructura para almacenar información de una IA Slime
struct SlimeInfo {
    vec3d position{};
    int life{};
};
struct BlackBoard_t {
    double tx { 0.0 } , tz { 0.0 };
    bool tactive { false };
    bool playerhunted{false};
    //Target Entity
    std::size_t teid{};
    //Path
    // Path_t<4> path { vec3d{8.0, 0.0, 4.0} , {3.0,0.0,4.0} , {3.0,0.0,5.0}, {8.0,0.0,5.0} };
    //Actualizar información IA slimes
    void updateSlimeInfo (std::size_t slime_id,vec3d position, int life){
        SlimeInfo slimeInfo(position,life);
        slimeData[slime_id] = slimeInfo;
    }
    std::unordered_map<std::size_t, SlimeInfo> slimeData;

};
