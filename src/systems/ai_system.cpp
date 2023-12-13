#include "ai_system.hpp"
#include <cmath>
#include <random>

[[nodiscard]] bool AISystem::isPlayerDetected(EntityManager& EM, PhysicsComponent const& p, ShootPlayerComponent const& spc) const noexcept {
    auto& li = EM.getSingleton<LevelInfo>();
    auto* playerEn = EM.getEntityByID(li.playerID);
    if (not playerEn) return false;
    auto& plphy = EM.getComponent<PhysicsComponent>(*playerEn);
    auto const distance = (p.position - plphy.position).lengthSQ();
    return  distance < (spc.detect_radius * spc.detect_radius);
}

[[nodiscard]] vec3f AISystem::getPlayerDistance(EntityManager& EM, PhysicsComponent const& p,  ShootPlayerComponent& spc) const noexcept {
    auto& li = EM.getSingleton<LevelInfo>();
    auto* playerEn = EM.getEntityByID(li.playerID);
    if (not playerEn) { spc.playerdetected = false; return vec3f{}; };
    auto& plphy = EM.getComponent<PhysicsComponent>(*playerEn);
    auto const distance = plphy.position - p.position;
    return  distance;
}

void AISystem::setVelocity(PhysicsComponent& p, vec3f distance) {
    if (distance != vec3f{ 0,0,0 }) {
        //Normalizo la distancia y se la asigno a la velocidad
        p.velocity = distance.normalize() * SPEED_AI;
    }
}

vec3f AISystem::FollowPatrol(PhysicsComponent& p,PatrolComponent& pc) {
    //Do patrol
    //si la pos actual es >= que el maximo patron vuelvo al principio
    if (pc.current >= pc.max_patrol) {
        pc.current = 0;
    }

    // Set del objetivo, next position
    auto const& target = pc.patrol[pc.current];
    if (target == pc.invalid) {
        pc.current = 0;
        pc.nexttarget = 0;
        return vec3f{ 0,0,0 };
    }
    //    std::cout << p.position.x() << ", " << p.position.y() << ", " << p.position.z() << "\n";
        //calculo la distancia
    auto const distance = target - p.position;
    //std::cout << distance.x() << ", " << distance.y() << ", " << distance.z() << "\n";
    // Si la distancia es < que el radio de llegada paso a la siguiente
    if (distance.length() < pc.arrival_radius) {
        pc.current++;
        pc.arrived = true;
    }
    return distance;
}
vec3f AISystem::getRandomPosinRange(float xmin, float xmax, float zmin, float zmax) {
    //Semilla para generar numeros aleatorios
    std::random_device rd;
    std::mt19937 gen(rd());
    // creo rangos
    std::uniform_real_distribution<float> rangoX(xmin, xmax);
    std::uniform_real_distribution<float> rangoZ(zmin, zmax);
    // obtengo x y z aleatoria
    float x = rangoX(gen);
    float z;
    do {
        z = rangoZ(gen);
    } while (z >= -18.0f && z <= -13.0f);

    //devuelvo vector
    return vec3f{ x,0.0f,z };

}
void AISystem::ShotandMove(ShootPlayerComponent& spc, PhysicsComponent& p, EntityManager& em, Entity& ent,float dt) {
    //cada x segundos cambia de posicion
    if(!spc.shoot){
        if(spc.elapsed_change_position>=spc.countdown_change_position){
            //before change position go visible
            auto& rend = em.getComponent<RenderComponent>(ent);
            rend.visible = true;
            //Set random position
            vec3f randomPos = getRandomPosinRange(spc.Xmin,spc.Xmax,spc.Zmin,spc.Zmax);
            p.position.setX(randomPos.x());
            p.position.setZ(randomPos.z());
            //Attack
            spc.shoot = true;
            spc.elapsed_change_position = 0;
        }
        spc.dec_countdown_change_pos(dt);
    }else{
        if(spc.elapsed_shoot_rap>=spc.countdown_shoot_rap){
            //set entitites invisible
            auto& rend1 = em.getComponent<RenderComponent>(ent);
            rend1.visible = false;
            if (isPlayerDetected(em, p, spc)) {
                //Attack
                auto& att = em.getComponent<AttackComponent>(ent);
                auto old_vel = (getPlayerDistance(em, p, spc)).normalized() * SPEED_AI;
                att.vel = old_vel;
                att.attack(AttackType::Ranged);
            }
            spc.shoot = false;
            spc.elapsed_shoot_rap = 0;
        }
        spc.dec_countdown_shoot_rap(dt);
    }
}
// Function to check if the direction is in the desired range

bool AISystem::isInDesiredRange(const vec3f& direction, float xmin, float xmax, float zmin, float zmax) {

    // Define your desired range here
    // float minX = 23.1f; // minimum X coordinate
    // float maxX = 34.9f; // maximum X coordinate
    // float minZ = -6.5f; // minimum Z coordinate
    // float maxZ = 6.5f;  // maximum Z coordinate

    // Check if the direction results in a position within the desired range
    return direction.x() >= xmin && direction.x() <= xmax &&

        direction.z() >= zmin && direction.z() <= zmax;

}
vec3f AISystem::getRandomDir() {
    // Genero direccion aleatoria
    switch (std::rand() % 4) {
    case 0:  return { 0.25f, 0.0f, 0.0f }; break;
    case 1:  return { -0.25f, 0.0f, 0.0f }; break;
    case 2:  return { 0.0f, 0.0f, 0.25f }; break;
    case 3:  return { 0.0f, 0.0f, -0.25f }; break;
    default: return { -0.25f, 0.0f, 0.0f }; break;
    }
}
 void AISystem::RandomAI(RandomShootComponent& rsc,PhysicsComponent& p,EntityManager& em,Entity& e,float dt){
    vec3f direction{};
    //check change direction when not shooting
    if(!rsc.stoped){
        if (rsc.elapsed_change_dir >= rsc.countdown_change_dir) {
            //set random dir
            direction = getRandomDir();
            rsc.oldvel = direction;
            rsc.elapsed_change_dir = 0;
        }
        rsc.dec_countdown_change_dir(dt);
    }
    // //check if ai have to stops
    if(!rsc.shoot){
        if(rsc.elapsed_stop>=rsc.countdown_stop){
            rsc.stoped = true;
            rsc.shoot = true;
            rsc.elapsed_stop = 0;
            rsc.elapsed_change_dir = 0;
            //shoot one time
            auto& att = em.getComponent<AttackComponent>(e);
            att.vel = rsc.oldvel;
            att.attack(AttackType::Ranged);
        }
        rsc.dec_countdown_stop(dt);
    }
    // auto& rend = em.getComponent<RenderComponent>(e);
    // rend.visible = false;
    //time while shooting
    if(rsc.shoot){
        if(rsc.elapsed_shoot >= rsc.countdown_shoot){
            //Shoot
            rsc.shoot = false;
            rsc.stoped = false;
            rsc.elapsed_shoot = 0;
        }
        rsc.dec_countdown_shoot(dt);
    }
    // //Set velocity
    if(!rsc.stoped){
        //Range Control
        if(!isInDesiredRange(p.position+rsc.oldvel,rsc.Xmin,rsc.Xmax,rsc.Zmin,rsc.Zmax)){
                rsc.oldvel *= -1.0f;
        }
        p.velocity = rsc.oldvel;
    }else{
        p.velocity = {};
    }

}
void AISystem::update(EntityManager& em, float dt)
{
    em.forEach<SYSCMPs_Patrol, SYSTAGs>([&,dt](Entity& e,PhysicsComponent& phy,PatrolComponent& pc)
    {
           (void)e;
            vec3f distance = FollowPatrol(phy, pc);
            setVelocity(phy,distance);
    });    
    em.forEach<SYSCMPs_ShootPlayer, SYSTAGs>([&,dt](Entity& ent,PhysicsComponent& phy,ShootPlayerComponent& spc){
         ShotandMove(spc,phy, em, ent,dt);
    });
     em.forEach<SYSCMPs_RandomShoot, SYSTAGs>([&,dt](Entity& ent,PhysicsComponent& phy,RandomShootComponent& rsc){
         RandomAI(rsc,phy,em,ent,dt);
    });
}