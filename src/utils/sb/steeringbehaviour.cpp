#include "steeringbehaviour.hpp"
#include <printf.h>
#include <cmath>
#include <numbers>
#include <algorithm>

struct Steer_t {
      double linear { 0.0 };
      double angular {0.0 };
};

void adjustAnglePiMinusPi(double& angle){
      while      ( angle >  PI ) angle -= 2*PI;
      while      ( angle < -PI ) angle += 2*PI;
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

bool STBH::Arrive(AIComponent& ai,PhysicsComponent& phy,Entity& ent){
        phy.v_angular = phy.a_linear = 0;
        // Linear distance to target
        auto vtx { ai.tx - phy.position.x() };
        auto vtz { ai.tz - phy.position.z() };
        auto tdist { std::sqrt(vtx*vtx + vtz*vtz) };
        if(tdist < ai.arrival_radius){
            ai.tactive = false;
            std::printf("[%d] He llegado! \n",static_cast<int>(ent.getID()));
            return true;
        }
        //Target linear velocity
        auto tvelocity    = std::clamp( tdist / ai.time2arrive, -phy.kMaxVLin, phy.kMaxVLin );
        phy.a_linear = std::clamp( tvelocity - phy.v_linear , -phy.kMaxAlin, phy.kMaxAlin ) ;
        
        //Target orientation
        auto torien { std::atan2(vtz,vtx) };
        if( torien < 0 ) torien += 2*PI;

        // target angular velocity
        auto angular_st { align(phy,torien,ai.time2arrive) };
        phy.v_angular = angular_st.angular;
        return false;
}
void STBH::Seek(AIComponent& ai,PhysicsComponent& phy){
        phy.v_angular = phy.a_linear = 0;
        //Face the target
        auto asteer { face(phy,{ai.tx,0.0,ai.tz},ai.time2arrive) };
        phy.v_angular = std::clamp(asteer.angular,-phy.kMaxVAng,phy.kMaxVAng);
        
        // Calculate target linear acceleration based on angular distance
        auto angular_velocity_size { std::fabs(phy.v_angular) };
        auto acceleration { phy.kMaxVLin / (1 + angular_velocity_size) };
        phy.a_linear = std::clamp( acceleration/ai.time2arrive , -phy.kMaxAlin, phy.kMaxAlin ) ;
}
void STBH::Flee(AIComponent& ai,PhysicsComponent& phy){
       phy.v_angular = phy.a_linear = 0;

        // Linear distance to target
        auto vtx {- ai.tx + phy.position.x() };
        auto vtz {- ai.tz + phy.position.z() };
        
        // target orientation 
        auto target_orientation { std::atan2(vtz,vtx) };
        if( target_orientation < 0 ) target_orientation += 2*PI;

        // target angular velocity
        auto asteer { align(phy,target_orientation,ai.time2arrive) };
        phy.v_angular = asteer.angular;

        // Calculate target linear acceleration based on angular distance
        auto angular_velocity_size { std::fabs(phy.v_angular) };
        auto acceleration { phy.kMaxVLin / (1 + angular_velocity_size) };
        phy.a_linear = std::clamp( acceleration/ai.time2arrive , -phy.kMaxAlin, phy.kMaxAlin ) ;
}