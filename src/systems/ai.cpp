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
void AISystem::FollowPatrol(AIComponent& ai,PhysicsComponent& p){
        //local Variables
        auto& pos = p.position;
        auto& vel = p.velocity;
        //Do patrol
        //si la pos actual es >= que el maximo patron vuelvo al principio
        if (ai.current >= ai.max_patrol) {
            ai.current = 0;
        }

        // Set del objetivo, next position
        auto const& target = ai.patrol[ai.current];
        if (target == ai.invalid) {
            ai.current = 0;
            return;
        }
        //calculo la distancia 
        auto const distance = target - pos;
        // Si la distancia es < que el radio de llegada paso a la siguiente
        if (distance.length() < ai.arrival_radius) {
            ai.current++;
        }
        //Normalizo la distancia y se la asigno a la velocidad
        vel = distance.normalized() * SPEED_AI;
}
void AISystem::FollowPatrolandShoot(AIComponent& ai, PhysicsComponent& p,EntityManager& em,Entity& ent) {
   //local Variables
        auto& pos = p.position;
        auto& vel = p.velocity;
        if(ai.shooting == false){
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
                return;
            }
            //calculo la distancia 
            auto const distance = target - pos;
            // Si la distancia es < que el radio de llegada paso a la siguiente
            if (distance.length() < ai.arrival_radius) {
                ai.current++;
            }
            //Normalizo la distancia y se la asigno a la velocidad
            vel = distance.normalized() * SPEED_AI;
            // Si la distancia es menor o igual a 2 unidades, detener y disparar
            std::printf("%i,%i\n",ai.current,ai.nexttarget);
            if(ai.current == ai.nexttarget){ // 0
                if (static_cast<int>(distance.length()) == 2) {
                // Detener y disparar
                ai.shooting = true;
                ai.contador_stop = 50;
                ai.nexttarget = ai.current+1;
                // Almacenar la velocidad original para poder restablecerla más tarde
                // ai.originalVelocity = vel;
                // ai.originalcurrent = ai.current;
                auto old_vel = vel;
                vel = vec3f{};  // Velocidad a cero
                //disparar
                auto& e{ em.newEntity() };
                em.addTag<HitPlayer>(e);
                /*auto& r = */em.addComponent<RenderComponent>(e, RenderComponent{ .position = { 0.0f, 0.0f, 0.0f }, .scale = { 0.5f, 0.5f, 0.5f }, .color = BLACK });
                /*auto& p = */em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position = em.getComponent<PhysicsComponent>(ent).position, .velocity = old_vel, .gravity = 0 });
                //em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::ATTACK });
                return;
                }
            }
        }
    if (ai.shooting) {
        ai.contador_stop -= 1;
        if (ai.contador_stop <= 0) {
            ai.shooting = false;
            ai.contador_stop = 0;

            // Restablecer la velocidad para reanudar la patrulla
            // vel = ai.originalVelocity;

            // // Asegurarse de que la patrulla continúe avanzando
            // ai.current = ai.originalcurrent;
        }
        return;
    }
}

void AISystem::update(EntityManager& em)
{
    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, PhysicsComponent& phy, AIComponent& ai)
    {
        if(e.hasTag<PatrolEnemy>()==true){
            FollowPatrol(ai,phy);
        }
        if(e.hasTag<PatrolFollowEnemy>()==true){
            //Player detection
            ai.playerdetected = this->isPlayerDetected(em, phy, ai);
            if (ai.playerdetected) {
                auto const& distance = getPlayerDistance(em, phy, ai);
                phy.velocity = distance.normalized() * SPEED_AI;
                return;
            }
            FollowPatrol(ai,phy);
        }
        if(e.hasTag<ShoterEnemy>()==true){
            FollowPatrolandShoot(ai,phy,em,e);
        }
    });
}







































































































































































































































































































































