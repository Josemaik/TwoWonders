#pragma once
#include "node.hpp"
#include <cmath>
#include <numbers>
#include <algorithm>
#include <utils/sb/steeringbehaviour.hpp>

static double radius_evade { 3.0 };
static double radius_centro_masas { 6.0 };
// Peso para la influencia de la dirección hacia el jugador
//double playerWeight = 0.8; 
// Peso para la influencia de la dirección hacia el centro de masas
static double centerOfMassWeight = 0.2;

struct BTAction_Seek : BTNode_t {
    // BTActionPatrol() = default;
   

    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        if (!ectx.ai->tactive) {

            return BTNodeStatus_t::fail;
        }
        ectx.ai->bh = "seeking";
        //Seek
        Steer_t steering = STBH::Seek(ectx.phy, { ectx.ai->tx,0.0,ectx.ai->tz });
        //Calcule Flocking
        // Comprobamos si tiene enemigos en un radio cercano
        double suma_cm_x{};
        double suma_cm_z{};
        double numenemigos_cm{};
        vec3d cm{};
        auto& bb = ectx.em.getSingleton<BlackBoard_t>();
        for(auto& pos : bb.positions){
            //check enemies to evade
            if(ectx.phy.position.calculatePointDistance(pos) < radius_evade){
                // Evade
                //auto xd = ectx.phy.position.calculatePointDistance(pos);
                Steer_t steeringEvade = STBH::Evade(ectx.phy,pos);
                steering.v_x += steeringEvade.v_x;
                steering.v_z += steeringEvade.v_z;
            }
            //Calculate mass center
            if(ectx.phy.position.calculatePointDistance(pos) < radius_centro_masas){
                suma_cm_x += pos.x();
                suma_cm_z += pos.z();
                numenemigos_cm++;
            }
        }
        // add center of mass direction to toplayer direction
        cm = vec3d{suma_cm_x/numenemigos_cm,0.0,suma_cm_z/numenemigos_cm};
        vec3d dirToCenterOfMass = cm - ectx.phy.position;
        // Dirección final, combinando las direcciones hacia el jugador y hacia el centro de masas
        // Dirección final, combinando las direcciones hacia el jugador y hacia el centro de masas
        steering.v_x += std::clamp(dirToCenterOfMass.x() * centerOfMassWeight, -ectx.phy.max_speed, ectx.phy.max_speed);
        steering.v_z += std::clamp(dirToCenterOfMass.z() * centerOfMassWeight, -ectx.phy.max_speed, ectx.phy.max_speed);
        //Almacenar posiciones de eenemigos en un radio mas grande
        //Para radio pequeño-> evade de estos y restarselo al vector velocidad ( evitar)
        //Para radio grande -> calcular centro de masas y sumar a la velocidad (cohesionar)
        

        ectx.phy.velocity = vec3d{ steering.v_x, 0.0, steering.v_z };
        ectx.phy.orientation = steering.orientation;
        return BTNodeStatus_t::success;
    }
};
