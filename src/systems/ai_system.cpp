#include "ai_system.hpp"
#include <cmath>
#include <random>

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

void AISystem::setVelocity(PhysicsComponent& p, AIComponent& ai, vec3f distance,float dt) {
    if (distance != vec3f{ 0,0,0 }) {
        //Normalizo la distancia y se la asigno a la velocidad
        p.velocity = distance.normalize() * SPEED_AI;
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
        return vec3f{ 0,0,0 };
    }
//    std::cout << p.position.x() << ", " << p.position.y() << ", " << p.position.z() << "\n";
    //calculo la distancia
    auto const distance = target - p.position;
    //std::cout << distance.x() << ", " << distance.y() << ", " << distance.z() << "\n";
    // Si la distancia es < que el radio de llegada paso a la siguiente
    if (distance.length() < ai.arrival_radius) {
        ai.current++;
        ai.arrived = true;
    }
    return distance;
}
vec3f AISystem::getVelocityonDirecion(vec3f vector){
    vector.normalized();

    float angle = atan2(vector.z(), vector.x());
    float degrees = angle * (180.0 / M_PI);

    if (degrees >= -45 && degrees < 45) {
        // Derecha
        return vec3f{0.5f,0.0f,0.0f};
    } else if (degrees >= 45 && degrees < 135) {
        // Arriba
         return vec3f{0.0f,0.0f,0.5f};
    } else if (degrees >= -135 && degrees < -45) {
        // Abajo
         return vec3f{0.0f,0.0f,-0.5f};
    } else {
        // Izquierda
        return vec3f{-0.5f,0.0f,0.0f};
    }
}
void AISystem::FollowPatrolandShoot(AIComponent& ai, PhysicsComponent& p, EntityManager& em, Entity& ent,float dt) {
    if (ai.shooting == false) {
        //Do patrol
        vec3f distance = FollowPatrol(ai, p);
        setVelocity(p, ai,distance,dt);
        // std::printf("%i,%i\n", ai.current, ai.nexttarget);
        //Check when ai should stop
        if (ai.current == ai.nexttarget) { // 0
            if (static_cast<int>(distance.length()) == 2) {
                // Detener y disparar
                ai.shooting = true;
                // ai.contador_stop = stop_value;
                ai.nexttarget = ai.current + 1;
                // Almacenar la velocidad original para poder restablecerla más tarde
                auto old_vel = getVelocityonDirecion(p.velocity);
                p.velocity = vec3f{};  // Velocidad a cero
                //disparar
                if (ent.hasComponent<AttackComponent>()) {
                    auto& att = em.getComponent<AttackComponent>(ent);
                    att.vel = old_vel;
                    att.attack(AttackType::Ranged);
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
//hacerlo con deltatime y que en vez de seguir un patron que cambie de posicion a una aleatoria dentro de un
//rango de x y z.
void AISystem::ShotandMove(AIComponent& ai, PhysicsComponent& p, EntityManager& em, Entity& ent,float dt) {
    //Compruebo si ha llegado al siguiente point
    if (ai.arrived) {
        p.velocity = { 0,0,0 };
        ai.contador_change_position -= 1;
        if (ai.contador_change_position == 0) {
            ai.contador_change_position = 70;
            ai.arrived = false;
        }
        //Attack
        if (isPlayerDetected(em, p, ai)) {
            auto& att = em.getComponent<AttackComponent>(ent);
            auto old_vel = (getPlayerDistance(em, p, ai)).normalized() * SPEED_AI;
            att.vel = old_vel;
            att.attack(AttackType::Ranged);
        }
    }
    else {
        vec3f distance = FollowPatrol(ai, p);
        setVelocity(p,ai, distance,dt);
    }

}
// Function to check if the direction is in the desired range
bool AISystem::isInDesiredRange(const vec3f& direction,PhysicsComponent& p) {
    // Define your desired range here
    float minX = 23.1f; // minimum X coordinate
    float maxX = 34.9f; // maximum X coordinate
    float minZ = -6.5f; // minimum Z coordinate
    float maxZ = 6.5f;  // maximum Z coordinate

    // Check if the direction results in a position within the desired range
    return direction.x() >= minX && direction.x() <= maxX &&
            direction.z() >= minZ && direction.z() <= maxZ;
}
vec3f AISystem::getRandomDir(){
        // Genero direccion aleatoria
        switch (std::rand() % 4) {
            case 0:  return {0.25f, 0.0f, 0.0f}; break;
            case 1:  return {-0.25f, 0.0f, 0.0f}; break;
            case 2:  return {0.0f, 0.0f, 0.25f}; break;
            case 3:  return {0.0f, 0.0f, -0.25f}; break;
            default: return {-0.25f, 0.0f, 0.0f}; break;
        }
}
 void AISystem::RandomAI(AIComponent& ai,PhysicsComponent& p,EntityManager& em,Entity& e,float dt){
    vec3f direction{};
    //check change direction when not shooting
    if(!ai.stoped){
        if (ai.elapsed_change_dir >= ai.countdown_change_dir) {
            //set random dir
            direction = getRandomDir();
            ai.oldvel = direction;
            ai.elapsed_change_dir = 0;
        }
        ai.dec_countdown_change_dir(dt);
    }
    // //check if ai have to stop
    if(!ai.shoot){
        if(ai.elapsed_stop>=ai.countdown_stop){
            ai.stoped = true;
            ai.shoot = true;
            ai.elapsed_stop = 0;
            ai.elapsed_change_dir = 0;
            //shoot one time
            auto& att = em.getComponent<AttackComponent>(e);
            att.vel = ai.oldvel;
            att.attack(AttackType::Ranged);
        }
        ai.dec_countdown_stop(dt);
    }
    // auto& rend = em.getComponent<RenderComponent>(e);
    // rend.visible = false;
    //time while shooting
    if(ai.shoot){
        if(ai.elapsed_shoot >= ai.countdown_shoot){
            //Shoot
            ai.shoot = false;
            ai.stoped = false;
            ai.elapsed_shoot = 0;
        }
        ai.dec_countdown_shoot(dt);
    }
    // //Set velocity
    if(!ai.stoped){
        //Range Control
        if(!isInDesiredRange(p.position+ai.oldvel,p)){
                ai.oldvel *= -1.0f;
        }
        p.velocity = ai.oldvel;
    }else{
        p.velocity = {};
    }
     
 }
void AISystem::update(EntityManager& em,float dt)
{
    em.forEach<SYSCMPs, SYSTAGs>([&,dt](Entity& e, PhysicsComponent& phy, AIComponent& ai)
    {
        switch (ai.current_type)
        {
        case AIComponent::AI_type::PatrolEnemy:
        {
            vec3f distance = FollowPatrol(ai, phy);
            setVelocity(phy,ai, distance,dt);
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
                setVelocity(phy,ai, distance,dt);
            }
        }
        break;

        case AIComponent::AI_type::ShoterEnemy:
        {
            FollowPatrolandShoot(ai, phy, em, e,dt);
        }
        break;

        case AIComponent::AI_type::ShoterEnemy2:
        {
            ShotandMove(ai, phy, em, e,dt);
        }
        break;

        case AIComponent::AI_type::RandomEnemy:
        {
            RandomAI(ai,phy,em,e,dt);
        }
        break;

        default:
            break;
        }
    });
}







































































































































































































































































































































