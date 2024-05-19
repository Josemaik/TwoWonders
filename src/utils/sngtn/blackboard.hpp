#pragma once

#include <cstdint>
#include <utils/path.hpp>
#include <utils/vec3D.hpp>
#include <vector>
#include "../../components/subject_component.hpp"

//Estructura para almacenar información de una IA
struct InfoSub
{
    InfoSub() = default;
    InfoSub(vec3d position, int life, bool acshield) : position(position), life(life) { subComp.setShield(acshield); }

    vec3d position{};
    int life{};
    SubjectComponent subComp{};
};

struct InfoSlime {
    vec3d position{};
    int life{};
    void setPosition(vec3d pos) { position = pos; }
    void setLife(int li) { life = li; }
};

struct BlackBoard_t {
    double tx{ 0.0 }, tz{ 0.0 };
    bool tactive{ false };
    bool playerhunted{ false };
    //Target Entity
    std::size_t teid{};
    //centinela para crear súbditos
    bool create_subdito{ false };
    bool activate_shield{ false };
    //Posicion Boss Final
    vec3d boss_position{};
    uint16_t boss_fase{ 1 };
    double countdown_change_fase{ 8.0 }, elapsed_change_fase{ 1.0 };
    //apisonadora
    bool playerdamagebycrusher{ false };
    //Path
    // Path_t<4> path { vec3d{8.0, 0.0, 4.0} , {3.0,0.0,4.0} , {3.0,0.0,5.0}, {8.0,0.0,5.0} };
    //Actualizar información IA slimes

    void
        updateInfoSlime(std::size_t id, vec3d position, int life) {
            // Comprobar si la clave ya existe en slimeData
        auto it = slimeData.find(id);
        if (it != slimeData.end()) {
            // Si la clave existe, actualizar sus valores
            it->second.setPosition(position);
            it->second.setLife(life);
        } else {
            // Si la clave no existe, añadir un nuevo elemento
            InfoSlime EntInfo(position, life);
            slimeData[id] = EntInfo;
        }
        // // si el tipo es 0 es slime, si es 1 es subdito
        // // if (type == 0) {
        // InfoSlime EntInfo(position, life);
        // slimeData[id] = EntInfo;
        // // si el tipo es 0 es slime, si es 1 es subdito
        // // if (type == 0) {
        // InfoSlime EntInfo(position, life);
        // slimeData[id] = EntInfo;

        }
    void

        updateInfoSub(std::size_t id, vec3d position, int life, bool acshield) {
        InfoSub EntInfo(position, life, acshield);
        subditosData[id] = EntInfo;
        idsubditos.push_back(id);
    }
    void
        updateInfoBoss(vec3d position) {
        boss_position = position;
    }
    std::unordered_map<std::size_t, InfoSlime> slimeData;
    std::unordered_map<std::size_t, InfoSub> subditosData;
    std::vector<size_t> idsubditos;

    //ray
    vec3d position_origin{}, direction{};
    bool launched{ false };
    //cone
    std::pair<vec3d,double> conesnow{};
    std::pair<vec3d,double> conegolem{};
    std::pair<vec3d,double> conespider{};
    double horizontalFOV{250.0}, VerticalFOV{60.0}, orientation_enemy{};
    //store positions and ids of potencial targets to claculate flocking
    std::vector<std::pair<std::size_t, vec3d>> potencial_targets;
};
