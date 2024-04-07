#include "steeringbehaviour.hpp"
#include <cmath>
#include <numbers>
#include <algorithm>

// double calculatePointDistance(vec3d const& target, vec3d const& origin) {
//         auto dx{ target.x() - origin.x() };
//         auto dz{ target.z() - origin.z() };
//         return std::sqrt(dx * dx + dz * dz);
// }

// void adjustAnglePiMinusPi(double& angle){
//       while      ( angle >  K_PI ) angle -= 2*K_PI;
//       while      ( angle < -K_PI ) angle += 2*K_PI;
// }

// Steer_t velocity_match(PhysicsComponent const& phy,double const target_v, double const time2arrive) {
//         Steer_t steering;
//         auto acceleration { (target_v - phy.v_linear) / time2arrive };
//         steering.linear = std::clamp(acceleration , -phy.kMaxAlin, phy.kMaxAlin ) ;
//         return steering;
// }

// Steer_t align(PhysicsComponent const& phy, double const target_orientation, const double time2arrive){
//         Steer_t steering;
//         // Adjust angular distance
//         auto angular_distance { target_orientation - phy.orientation };
//         adjustAnglePiMinusPi(angular_distance);
//         //Adjusted angular velocity
//         steering.angular = std::clamp(angular_distance/time2arrive, -phy.kMaxVAng,phy.kMaxVAng);
//         return steering;
// }

// Steer_t face(PhysicsComponent const& phy, vec3d target){
//         Steer_t steering;
//        // Calculate target orientation
//         auto distx { target.x() - phy.position.x() };
//         auto distz { target.z() - phy.position.z() };
//         auto target_orientation { std::atan2(distz,distx) };
//         if( target_orientation < 0 ) target_orientation += 2*K_PI;

//         // Align and Set angular velocity 
//         auto angular_distance = target_orientation - phy.orientation;
//         adjustAnglePiMinusPi(angular_distance);

//         steering.orientation =std::clamp(angular_distance, -phy.kMaxVAng,phy.kMaxVAng);
//         return steering;
// }

Steer_t STBH::Arrive(PhysicsComponent const& phy, vec3d const& target, double const arrivalRadious) {
        Steer_t steering;
        //check if i'm on target
        steering.arrived = false;
        auto tdist{ target.calculatePointDistance({phy.position.x(),0.0,phy.position.z()})};
        //auto tdist{ calculatePointDistance(target, {phy.position.x(),0.0,phy.position.z()}) };
        if (tdist < arrivalRadious)
                steering.arrived = true;
        // face target
        //steering.orientation =  face(phy,target).orientation;

        // accelerate to arrive to the point
        //Target linear velocity
        // auto tvelocity    = std::clamp( tdist / time2arrive, -phy.kMaxVLin, phy.kMaxVLin );
        // auto lin_steer { velocity_match(phy,tvelocity,time2arrive) };
        steering.v_x = std::clamp((target.x() - phy.position.x()), -phy.max_speed, phy.max_speed);
        steering.v_z = std::clamp((target.z() - phy.position.z()), -phy.max_speed, phy.max_speed);

        return steering;
}
Steer_t STBH::Seek(PhysicsComponent const& phy, vec3d const& target) {
        Steer_t steering;


        // auto const distance = target - phy.position;

        //Face the target
        //steering.orientation =  face(phy,target).orientation;
       // vec3d direction = target - phy.position;
        // Set the orientation to face the direction
       //steering.orientation = atan2(direction.z(), direction.x());
        // auto asteer { face(phy,target,time2arrive) };
        // steering.angular = std::clamp(asteer.angular,-phy.kMaxVAng,phy.kMaxVAng);

        // Calculate target linear acceleration based on angular distance
        // auto angular_velocity_size { std::fabs(phy.v_angular) };
        // auto acceleration { phy.kMaxVLin / (1 + angular_velocity_size) };
        //Calculate linear velocity based on euclidean distance
        steering.v_x = std::clamp((target.x() - phy.position.x()), -phy.max_speed, phy.max_speed);
        steering.v_z = std::clamp((target.z() - phy.position.z()), -phy.max_speed, phy.max_speed);
        // Evade nearest enemies
        
        // Cambiamos la orientación del objeto para que mire hacia el objetivo
        vec3d dir = target - phy.position;
        steering.orientation = atan2(dir.x(), dir.z());

        // steering.v_x = std::clamp(distance.x(), -phy.max_speed, phy.max_speed);
        // steering.v_z = std::clamp(distance.z(), -phy.max_speed, phy.max_speed);
        return steering;
}
Steer_t STBH::Flee(PhysicsComponent const& phy, vec3d const& enemy) {
        //Calcular pnto opuesto
        vec3d target{ 2 * phy.position.x() - enemy.x(),0.0,2 * phy.position.z() - enemy.z() };
        // //Seek al punto opuesto
        return Seek(phy, target);
}

Steer_t STBH::Pursue(PhysicsComponent const& phyTarget, PhysicsComponent const& phyPursuer) {
        //Calculate distance
        vec3d target{ phyTarget.position.x(), 0.0 ,   phyTarget.position.z() };
        vec3d avoider{ phyPursuer.position.x(), 0.0 ,   phyPursuer.position.z() };
        auto distance{ target.calculatePointDistance(avoider) };
        auto minimaltime{ distance / phyPursuer.max_speed };
        vec3d predicted_target{
                phyTarget.position.x() + phyTarget.velocity.x() * minimaltime,
                0.0                                                          ,
                phyTarget.position.z() + phyTarget.velocity.z() * minimaltime
        };
        return Seek(phyPursuer, predicted_target);
}
Steer_t STBH::Avoid(PhysicsComponent const& phyTarget, PhysicsComponent const& phyAvoider) {
        //Calculate distance
        vec3d target{ phyTarget.position.x(), 0.0 ,   phyTarget.position.z() };
        vec3d pursuer{ phyAvoider.position.x(), 0.0 ,   phyAvoider.position.z() };
        auto distance{ target.calculatePointDistance(pursuer) };
        auto minimaltime{ distance / phyAvoider.kMaxVLin };
        vec3d predicted_avoider{
                phyTarget.position.x() + phyTarget.velocity.x() * minimaltime,
                0.0                                                          ,
                phyTarget.position.z() + phyTarget.velocity.z() * minimaltime
        };
        // Invert the direction to avoid the predicted avoider position
        vec3d avoidDirection = target - predicted_avoider;
        return Seek(phyTarget, target + avoidDirection);
}
Steer_t STBH::Evade(PhysicsComponent const& phyEvader,PhysicsComponent const& phytoevade){
        Steer_t steering;
        //Calculamos vector de la pos a evadir hacia el player

        // Calcula la dirección hacia la que queremos evadirnos
        vec3d evadeDirection = phytoevade.position - phyEvader.position;
        // Normaliza la dirección de la evasión
        // vec3d normalizedEvadeDirection = evadeDirection.normalized();
        // Calcula el vector director para evadirse (invertido)
        evadeDirection.normalize();
        
        vec3d evadeVector = evadeDirection * -1;

        // Asigna la dirección del vector de evasión a la orientación
        steering.orientation = atan2(evadeVector.x(), evadeVector.z());
        // Calcula la distancia entre el evasor y el objetivo a evadir
        double distance = evadeDirection.length();

        // double distance = phyEvader.position.distance(postoevade);
        // double separationForce = (1.0 / distance) * 15;
        //double separationForce = 0.5;
        // Calcula la fuerza de evasión en función de la distancia
        double maxSeparationForce = 1.0; // Ajusta este valor según sea necesario
        double separationForce = std::clamp((maxSeparationForce / distance), 0.0, maxSeparationForce);

        // Asigna la velocidad máxima al vector de evasión
        steering.v_x = std::clamp(evadeVector.x() * separationForce, -phyEvader.max_speed, phyEvader.max_speed);
        steering.v_z = std::clamp(evadeVector.z() * separationForce, -phyEvader.max_speed, phyEvader.max_speed);

        return steering;
}
