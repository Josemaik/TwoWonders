#include "ai_system.hpp"
#include <cmath>
#include <random>

//Funcion para saber si el player ha sido detectado o no
// [[nodiscard]] bool AISystem::isPlayerDetected(EntityManager& EM, PhysicsComponent const& p, ShootPlayerComponent const& spc) const noexcept {
//     auto& li = EM.getSingleton<LevelInfo>();
//     auto* playerEn = EM.getEntityByID(li.playerID);
//     if (not playerEn) return false;
//     auto& plphy = EM.getComponent<PhysicsComponent>(*playerEn);
//     auto const distance = (p.position - plphy.position).lengthSQ();
//     return  distance < (spc.detect_radius * spc.detect_radius);
// }
// // Obtener la distancia del enemigo con respecto al player
// [[nodiscard]] vec3f AISystem::getPlayerDistance(EntityManager& EM, PhysicsComponent const& p, ShootPlayerComponent& spc) const noexcept {
//     auto& li = EM.getSingleton<LevelInfo>();
//     auto* playerEn = EM.getEntityByID(li.playerID);
//     if (not playerEn) { spc.playerdetected = false; return vec3f{}; };
//     auto& plphy = EM.getComponent<PhysicsComponent>(*playerEn);
//     auto const distance = plphy.position - p.position;
//     return  distance;
// }
// // Poner la velocidad al compponente de físicas
// // void AISystem::setVelocity(PhysicsComponent& p, vec3f distance) {
// //     if (distance != vec3f{ 0,0,0 }) {
// //         //Normalizo la distancia y se la asigno a la velocidad
// //         p.velocity = distance.normalize() * SPEED_AI;
// //     }
// // }
// //FUncion para poner la Velocidad dado un rango
// template <typename CMP>
// void AISystem::setVelocityinRange(PhysicsComponent& p,CMP& cmp){
//     if (!cmp.stoped) {
//         //Range Control
//         if (!isInDesiredRange(p.position + cmp.oldvel, cmp.Xmin, cmp.Xmax, cmp.Zmin, cmp.Zmax)) {
//             cmp.oldvel *= -1.0f;
//         }
//         p.velocity = cmp.oldvel;
//     }
//     else {
//         p.velocity = {};
//     }
// }
// //función que devuelve una posición aleatoria dentro de un rango específico
// vec3f AISystem::getRandomPosinRange(float xmin, float xmax, float zmin, float zmax) {
//     //Semilla para generar numeros aleatorios
//     std::random_device rd;
//     std::mt19937 gen(rd());
//     // creo rangos
//     std::uniform_real_distribution<float> rangoX(xmin, xmax);
//     std::uniform_real_distribution<float> rangoZ(zmin, zmax);
//     // obtengo x y z aleatoria
//     float x = rangoX(gen);
//     float z;
//     do {
//         z = rangoZ(gen);
//     } while (z >= -18.0f && z <= -13.0f);

//     //devuelvo vector
//     return vec3f{ x,0.0f,z };
// }
// //Función para comprobar si la dirección está fuera de rango
// bool AISystem::isInDesiredRange(const vec3f& direction, float xmin, float xmax, float zmin, float zmax) {
//     return direction.x() >= xmin && direction.x() <= xmax &&
//         direction.z() >= zmin && direction.z() <= zmax;
// }
// // Devuelve una dirección aleatoria
// vec3f AISystem::getRandomDir() {
//     // Genero direccion aleatoria
//     switch (std::rand() % 4) {
//     case 0:  return { 0.25f, 0.0f, 0.0f }; break;//derecha
//     case 1:  return { -0.25f, 0.0f, 0.0f }; break; //izquieda
//     case 2:  return { 0.0f, 0.0f, 0.25f }; break; //Abajo
//     case 3:  return { 0.0f, 0.0f, -0.25f }; break; //Arriba
//     default: return { -0.25f, 0.0f, 0.0f }; break;
//     }
// }
// // Devuelve una dirección aleatoria incluida diagonales
// vec3f AISystem::getRandomDirwithDiagonals() {
//     // Genero direccion aleatoria
//     switch (std::rand() % 8) {
//     case 0:  return { 0.25f, 0.0f, 0.0f }; break; //derecha
//     case 1:  return { -0.25f, 0.0f, 0.0f }; break; //izquieda
//     case 2:  return { 0.0f, 0.0f, 0.25f }; break; //Abajo
//     case 3:  return { 0.0f, 0.0f, -0.25f }; break; //Arriba
//     case 4:  return { -0.25f, 0.0f, -0.25f}; break; //up-left
//     case 5:  return { 0.25f, 0.0f, 0.25f }; break; //down-right
//     case 6:  return { 0.25f, 0.0f, -0.25f }; break; //up-right
//     case 7:  return { -0.25f, 0.0f, 0.25f }; break; //down-left
//     default: return { -0.25f, 0.0f, 0.0f }; break;
//     }
// }
/////////////////////////////////////////////////////////////////
/// TIPOS DE IA /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////----------------------------------------------------------
//IA que sigue un patrón especifico
// template <typename CMP>
// vec3f AISystem::FollowPatrol(PhysicsComponent& p, CMP& cmp) {
//     //Do patrol
//     //si la pos actual es >= que el maximo patron vuelvo al principio
//     if (cmp.current >= cmp.max_patrol) {
//         cmp.current = 0;
//     }

//     // Set del objetivo, next position
//     auto const& target = cmp.patrol[cmp.current];
//     if (target == cmp.invalid) {
//         cmp.current = 0;
//         cmp.nexttarget = 0;
//         return vec3f{ 0,0,0 };
//     }
//     //    std::cout << p.position.x() << ", " << p.position.y() << ", " << p.position.z() << "\n";
//         //calculo la distancia
//     auto const distance = target - p.position;
//     //std::cout << distance.x() << ", " << distance.y() << ", " << distance.z() << "\n";
//     // Si la distancia es < que el radio de llegada paso a la siguiente
//     if (distance.length() < cmp.arrival_radius) {
//         cmp.current++;
//         cmp.arrived = true;
//     }
//     return distance;
// }
//IA del río ( dispara al player, se esconde y cambia de posición)
// void AISystem::ShotandMove(ShootPlayerComponent& spc, PhysicsComponent& p, EntityManager& em, Entity& ent, float dt) {
//     //cada x segundos cambia de posicion
//     if (!spc.shoot) {
//         if (spc.elapsed_change_position >= spc.countdown_change_position) {
//             //before change position go visible
//             auto& rend = em.getComponent<RenderComponent>(ent);
//             rend.visible = true;
//             //Set random position
//             vec3f randomPos = getRandomPosinRange(spc.Xmin, spc.Xmax, spc.Zmin, spc.Zmax);
//             p.position.setX(randomPos.x());
//             p.position.setZ(randomPos.z());
//             //Attack
//             spc.shoot = true;
//             spc.elapsed_change_position = 0;
//         }
//         spc.dec_countdown_change_pos(dt);
//     }
//     else {
//         if (spc.elapsed_shoot_rap >= spc.countdown_shoot_rap) {
//             //set entitites invisible
//             auto& rend1 = em.getComponent<RenderComponent>(ent);
//             rend1.visible = false;
//             if (isPlayerDetected(em, p, spc)) {
//                 //Attack
//                 auto& att = em.getComponent<AttackComponent>(ent);
//                 auto old_vel = (getPlayerDistance(em, p, spc)).normalized() * SPEED_AI;
//                 att.vel = old_vel;
//                 att.attack(AttackType::Ranged);
//             }
//             spc.shoot = false;
//             spc.elapsed_shoot_rap = 0;
//         }
//         spc.dec_countdown_shoot_rap(dt);
//     }
// }
//Ia que se mueve de forma aleatoria y realiza disparos en la dirección que mira
// void AISystem::RandomAI(RandomShootComponent& rsc, PhysicsComponent& p, EntityManager& em, Entity& e, float dt) {
//     vec3f direction{};
//     //check change direction when not shooting
//     if (!rsc.stoped) {
//         if (rsc.elapsed_change_dir >= rsc.countdown_change_dir) {
//             //set random dir
//             direction = getRandomDir();
//             rsc.oldvel = direction;
//             rsc.elapsed_change_dir = 0;
//         }
//         rsc.dec_countdown_change_dir(dt);
//     }
//     //check if ai have to stops
//     if (!rsc.shoot) {
//         if (rsc.elapsed_stop >= rsc.countdown_stop) {
//             rsc.stoped = true;
//             rsc.shoot = true;
//             rsc.elapsed_stop = 0;
//             rsc.elapsed_change_dir = 0;
//             //shoot one time
//             auto& att = em.getComponent<AttackComponent>(e);
//             att.vel = rsc.oldvel;
//             att.attack(AttackType::Ranged);
//         }
//         rsc.dec_countdown_stop(dt);
//     }
//     //time while shooting
//     if (rsc.shoot) {
//         if (rsc.elapsed_shoot >= rsc.countdown_shoot) {
//             rsc.shoot = false;
//             rsc.stoped = false;
//             rsc.elapsed_shoot = 0;
//         }
//         rsc.dec_countdown_shoot(dt);
//     }
//     //Set velocity
//     setVelocityinRange(p,rsc);
// }
// void AISystem::DiagonalAI(DiagonalComponent& dc, PhysicsComponent& p, float dt){
//     vec3f direction{};
//     //check change direction
//     if (!dc.stoped) {
//         if (dc.elapsed_change_dir >= dc.countdown_change_dir) {
//             //set random dir
//             direction = getRandomDirwithDiagonals();
//             dc.oldvel = direction;
//             dc.elapsed_change_dir = 0;
//         }
//         dc.dec_countdown_change_dir(dt);
//     }
//     //check if ai have to stops
//     if (!dc.moving) {
//         if (dc.elapsed_stop >= dc.countdown_stop) {
//             dc.stoped = true;
//             dc.moving = true;
//             dc.elapsed_stop = 0;
//             dc.elapsed_change_dir = 0;
//         }
//         dc.dec_countdown_stop(dt);
//     }
//     //time to return moving
//     if (dc.moving) {
//         if (dc.elapsed_moving >= dc.countdown_moving) {
//             //Shoot
//             dc.moving = false;
//             dc.stoped = false;
//             dc.elapsed_moving = 0;
//         }
//         dc.dec_countdown_moving(dt);
//     }
//     //Set velocity
//     setVelocityinRange(p,dc);
// }
// vec3f AISystem::DrakeAI(DrakeComponent& drc,PhysicsComponent& p,EntityManager& em, Entity& e,float dt){
//     if (drc.elapsed_shoot >= drc.countdown_shoot) {
//             drc.elapsed_shoot = 0;
//             //shoot one time
//             auto& att = em.getComponent<AttackComponent>(e);
//             att.attack(AttackType::TripleShot);
//     }
//     drc.dec_countdown_shoot(dt);
//     vec3f distance = FollowPatrol(p,drc);
//     return distance;
// }
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Actualizar las IA
void AISystem::update(EntityManager& em, float dt)
{
    em.forEach<SYSCMPs, SYSTAGs>([&, dt](Entity& e, PhysicsComponent& phy, AIComponent& ai)
    {
        if(ai.behaviourTree){
            ai.behaviourTree->run( {em,e,ai,phy,dt} );
            return;
        }
        //  if(rsc.behaviourTree){
        //     pc.behaviourTree->run( {em,e,pc,rsc,spc,dc,drc,phy} );
        //     return;
        // }
        //  if(spc.behaviourTree){
        //     pc.behaviourTree->run( {em,e,pc,rsc,spc,dc,drc,phy} );
        //     return;
        // }
        //  if(dc.behaviourTree){
        //     pc.behaviourTree->run( {em,e,pc,rsc,spc,dc,drc,phy} );
        //     return;
        // }
        //  if(drc.behaviourTree){
        //     pc.behaviourTree->run( {em,e,pc,rsc,spc,dc,drc,phy} );
        //     return;
        // }

        // vec3f distance = FollowPatrol(phy, pc);
        // setVelocity(phy, distance);
    });
    // em.forEach<SYSCMPs_ShootPlayer, SYSTAGs>([&, dt](Entity& ent, PhysicsComponent& phy, ShootPlayerComponent& spc) {
    //     ShotandMove(spc, phy, em, ent, dt);
    // });
    // em.forEach<SYSCMPs_RandomShoot, SYSTAGs>([&, dt](Entity& ent, PhysicsComponent& phy, RandomShootComponent& rsc) {
    //     RandomAI(rsc, phy, em, ent, dt);
    // });
    // em.forEach<SYSCMPs_Diagonal, SYSTAGs>([&, dt](Entity& ent, PhysicsComponent& phy, DiagonalComponent& dc) {
    //     (void)ent;
    //     DiagonalAI(dc, phy, dt);
    // });
    // em.forEach<SYSCMPs_Drake, SYSTAGs>([&, dt](Entity& ent, PhysicsComponent& phy, DrakeComponent& drc) {
    //     (void)ent;
    //     vec3f distance = DrakeAI(drc, phy, em ,ent,dt);
    //     setVelocity(phy, distance);
    // });
}