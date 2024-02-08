#pragma once

#include <cstdint>
#include <utils/path.hpp>
#include <utils/vec3D.hpp>

//Estructura para almacenar información de una IA
struct Info {
    vec3d position{};
    int life{};
};

struct BlackBoard_t {
    double tx { 0.0 } , tz { 0.0 };
    bool tactive { false };
    bool playerhunted{false};
    //Target Entity
    std::size_t teid{};
    //centinela para crear súbditos
    uint16_t tam_subditos_tocreate{0};
    //Posicion Boss Final
    vec3d boss_position{};
    //Path
    // Path_t<4> path { vec3d{8.0, 0.0, 4.0} , {3.0,0.0,4.0} , {3.0,0.0,5.0}, {8.0,0.0,5.0} };
    //Actualizar información IA slimes
    void updateInfo (std::size_t id,vec3d position, int life, uint16_t type){
        Info EntInfo(position,life);
        // si el tipo es 0 es slime, si es 1 es subdito
        if(type == 0){
            slimeData[id] = EntInfo;
        }else{
            if(type == 1){
                //std::cout << "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE \n";
                subditosData[id] = EntInfo;
            }else{
                boss_position = position;
            }
        }

    }
    std::unordered_map<std::size_t, Info> slimeData;
    std::unordered_map<std::size_t, Info> subditosData;

};
