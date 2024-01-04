#include "steeringbehaviour.hpp"
#include <printf.h>
#include <cmath>
#include <numbers>
#include <algorithm>

double calculatePointDistance(vec3d const& target, vec3d const& origin){
        auto dx { target.x() - origin.x() };
        auto dz { target.z() - origin.z() };
        return std::sqrt(dx*dx + dz*dz);
}

void adjustAnglePiMinusPi(double& angle){
      while      ( angle >  PI ) angle -= 2*PI;
      while      ( angle < -PI ) angle += 2*PI;
}

Steer_t velocity_match(PhysicsComponent const& phy,double const target_v, double const time2arrive) {
        Steer_t steering;
        auto acceleration { (target_v - phy.v_linear) / time2arrive };
        steering.linear = std::clamp(acceleration , -phy.kMaxAlin, phy.kMaxAlin ) ;
        return steering;
}

Steer_t align(PhysicsComponent const& phy, double const target_orientation, const double time2arrive){
        Steer_t steering;
        // Adjust angular distance
        auto angular_distance { target_orientation - phy.orientation };
        adjustAnglePiMinusPi(angular_distance);
        //Adjusted angular velocity
        steering.angular = std::clamp(angular_distance/time2arrive, -phy.kMaxVAng,phy.kMaxVAng);
        return steering;
}

Steer_t face(PhysicsComponent const& phy, vec3d target, const double time2arrive){
      // Calculate target orientation
        auto distx { target.x() - phy.position.x() };
        auto distz { target.z() - phy.position.z() };
        auto target_orientation { std::atan2(distz,distx) };
        if( target_orientation < 0 ) target_orientation += 2*PI;

        // Set angular velocity
        return align(phy,target_orientation,time2arrive);
}

Steer_t STBH::Arrive(PhysicsComponent const& phy,vec3d const& target,double const time2arrive, double const arrivalRadious){
        //check if i'm on target
        auto tdist { calculatePointDistance(target, {phy.position.x(),0.0,phy.position.z()}) };
        if(tdist < arrivalRadious)
            return {};
        // face target
        auto ang_steer { face(phy,target,time2arrive) };

        // accelerate to arrive to the point
        //Target linear velocity
        auto tvelocity    = std::clamp( tdist / time2arrive, -phy.kMaxVLin, phy.kMaxVLin );
        auto lin_steer { velocity_match(phy,tvelocity,time2arrive) };

        return { lin_steer.linear,ang_steer.angular };
}
Steer_t STBH::Seek(PhysicsComponent const& phy,vec3d const& target, double const time2arrive){
        Steer_t steering;
        //Face the target
        auto asteer { face(phy,target,time2arrive) };
        steering.angular = std::clamp(asteer.angular,-phy.kMaxVAng,phy.kMaxVAng);
        
        // Calculate target linear acceleration based on angular distance
        auto angular_velocity_size { std::fabs(phy.v_angular) };
        auto acceleration { phy.kMaxVLin / (1 + angular_velocity_size) };
        steering.linear = std::clamp( acceleration/time2arrive , -phy.kMaxAlin, phy.kMaxAlin ) ;

        return steering;
}
Steer_t STBH::Flee(PhysicsComponent const& phy,vec3d const& enemy, double const time2flee){
        //Calcular pnto opuesto
        vec3d target { 2*phy.position.x() - enemy.x(),0.0,2*phy.position.z() - enemy.z() };
        //Seek al punto opuesto
        return Seek(phy,target,time2flee);
}