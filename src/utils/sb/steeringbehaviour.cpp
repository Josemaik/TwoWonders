#include "steeringbehaviour.hpp"
#include <printf.h>
#include <cmath>
#include <numbers>
#include <algorithm>
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
        
        auto tvelocity    = std::clamp( tdist / ai.time2arrive, -phy.kMaxVLin, phy.kMaxVLin );
        phy.a_linear = std::clamp( tvelocity - phy.v_linear , -phy.kMaxAlin, phy.kMaxAlin ) ;
        
        //velocidad angular 
        auto torien { std::atan2(vtz,vtx) };
        if( torien < 0 ) torien += 2*PI;
        auto vang { torien - phy.orientation };
        if      ( vang >  PI ) vang -= 2*PI;
        else if ( vang < -PI ) vang += 2*PI;
        phy.v_angular = std::clamp(vang/ai.time2arrive, -phy.kMaxVAng,phy.kMaxVAng);
        return false;
  }
  void STBH::Seek(AIComponent& ai,PhysicsComponent& phy){
        phy.v_angular = phy.a_linear = 0;

        // Linear distance to target
        auto vtx { ai.tx - phy.position.x() };
        auto vtz { ai.tz - phy.position.z() };
        
        
        // target orientation 
        auto target_orientation { std::atan2(vtz,vtx) };
        if( target_orientation < 0 ) target_orientation += 2*PI;
        // target angular velocity
        auto angular_distance { target_orientation - phy.orientation };
        if      ( angular_distance >  PI ) angular_distance -= 2*PI;
        else if ( angular_distance < -PI ) angular_distance += 2*PI;
        auto angular_velocity {angular_distance/ai.time2arrive};
        phy.v_angular = std::clamp(angular_velocity, -phy.kMaxVAng,phy.kMaxVAng);
         

        // Calculate target linear acceleration based on angular distance
        auto angular_velocity_size { std::fabs(angular_velocity) };
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
        auto angular_distance { target_orientation - phy.orientation };
        if      ( angular_distance >  PI ) angular_distance -= 2*PI;
        else if ( angular_distance < -PI ) angular_distance += 2*PI;
        auto angular_velocity {angular_distance/ai.time2arrive};
        phy.v_angular = std::clamp(angular_velocity, -phy.kMaxVAng,phy.kMaxVAng);
         

        // Calculate target linear acceleration based on angular distance
        auto angular_velocity_size { std::fabs(angular_velocity) };
        auto acceleration { phy.kMaxVLin / (1 + angular_velocity_size) };
        phy.a_linear = std::clamp( acceleration/ai.time2arrive , -phy.kMaxAlin, phy.kMaxAlin ) ;
  }