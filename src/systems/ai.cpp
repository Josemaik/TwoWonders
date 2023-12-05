#include "ai.hpp"
#include <cmath>

[[nodiscard]] bool AISystem::isPlayerDetected(EntityManager& EM, PhysicsComponent const& p, AIComponent const& ai) const noexcept {
    auto& li = EM.getSingleton<LevelInfo>();
    auto* playerEn = EM.getEntityByID(li.playerID);
    if (not playerEn) return false;
    auto& plphy = EM.getComponent<PhysicsComponent>(*playerEn);
    auto const distance = (p.position - plphy.position).lengthSQ();
    return  distance < (ai.detect_radius * ai.detect_radius);
}
[[nodiscard]] vec3f AISystem::getPlayerDistance(EntityManager& EM, PhysicsComponent const& p, AIComponent& ai) const noexcept {
    auto& li = EM.getSingleton<LevelInfo>();
    auto* playerEn = EM.getEntityByID(li.playerID);
    if (not playerEn) { ai.playerdetected = false; return vec3f{}; };
    auto& plphy = EM.getComponent<PhysicsComponent>(*playerEn);
    auto const distance = plphy.position - p.position;
    return  distance;
}
void AISystem::setVelocity(PhysicsComponent& p,vec3f distance){
    if(distance != vec3f{0,0,0}){
         //Normalizo la distancia y se la asigno a la velocidad
        p.velocity = distance.normalized() * SPEED_AI;
    }
}
vec3f AISystem::FollowPatrol(AIComponent& ai, PhysicsComponent& p) {
    //Do patrol
    //si la pos actual es >= que el maximo patron vuelvo al principio
    if (ai.current >= ai.max_patrol) {
        ai.current = 0;
    }

    // Set del objetivo, next position
    auto const& target = ai.patrol[ai.current];
    if (target == ai.invalid) {
        ai.current = 0;
        ai.nexttarget = 0;
        return vec3f{0,0,0};
    }
    //calculo la distancia
    auto const distance = target - p.position;
    // Si la distancia es < que el radio de llegada paso a la siguiente
    if (distance.length() < ai.arrival_radius) {
        ai.current++;
        ai.arrived = true;
    }
    return distance;
}
void AISystem::FollowPatrolandShoot(AIComponent& ai, PhysicsComponent& p, EntityManager& em, Entity& ent) {
    if (ai.shooting == false) {
        //Do patrol
        vec3f distance = FollowPatrol(ai,p);
        setVelocity(p,distance);
        std::printf("%i,%i\n", ai.current, ai.nexttarget);
        //Check when ai should stop
        if (ai.current == ai.nexttarget) { // 0
            if (static_cast<int>(distance.length()) == 2) {
                // Detener y disparar
                ai.shooting = true;
                // ai.contador_stop = stop_value;
                ai.nexttarget = ai.current + 1;
                // Almacenar la velocidad original para poder restablecerla más tarde
                auto old_vel = p.velocity;
                p.velocity = vec3f{};  // Velocidad a cero
                //disparar
                if (ent.hasComponent<AttackComponent>()) {
                    auto& att = em.getComponent<AttackComponent>(ent);
                    att.createAttack = true;
                    att.vel = old_vel;
                }
                else
                    em.addComponent<AttackComponent>(ent, AttackComponent{ .createAttack = true, .countdown = 50, .vel = old_vel });
                return;
            }
        }
    }
    if (ai.shooting) {
        ai.contador_stop -= 1;
        if (ai.contador_stop <= 0) {
            ai.shooting = false;
            ai.contador_stop = 50;
        }
        return;
    }
}
void AISystem::ShotandMove(AIComponent& ai, PhysicsComponent& p,EntityManager& em,Entity& ent){
        //Compruebo si ha llegado al siguiente point
        if(ai.arrived){
            p.velocity = {0,0,0};
            ai.contador_change_position -= 1;
            if(ai.contador_change_position == 0){
                ai.contador_change_position = 70;
                ai.arrived = false;
            }
             //Attack
             if(isPlayerDetected(em,p,ai))  {
                auto& att = em.getComponent<AttackComponent>(ent);
                auto old_vel = (getPlayerDistance(em,p,ai)).normalized() * SPEED_AI;
                att.vel = old_vel;
                att.attack();
             }
        }else{
             vec3f distance = FollowPatrol(ai,p);
             setVelocity(p,distance);
        }
       
}
void AISystem::update(EntityManager& em)
{
    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, PhysicsComponent& phy, AIComponent& ai)
    {
         switch (ai.current_type)
        {
            case AIComponent::AI_type::PatrolEnemy:
                {
                    vec3f distance = FollowPatrol(ai, phy);
                    setVelocity(phy, distance);
                }
                break;

            case AIComponent::AI_type::PatrolFollowEnemy:
                {
                    // Player detection
                    ai.playerdetected = this->isPlayerDetected(em, phy, ai);
                    if (ai.playerdetected) {
                        auto const& distance = getPlayerDistance(em, phy, ai);
                        phy.velocity = distance.normalized() * SPEED_AI;
                    }
                    else {
                        vec3f distance = FollowPatrol(ai, phy);
                        setVelocity(phy, distance);
                    }
                }
                break;

            case AIComponent::AI_type::ShoterEnemy:
                {
                    FollowPatrolandShoot(ai, phy, em, e);
                }
                break;
            case AIComponent::AI_type::ShoterEnemy2:
                {
                    ShotandMove(ai,phy,em,e);
                }
                break;
            default:
                break;
        }
    });
}







































































































































































































































































































































