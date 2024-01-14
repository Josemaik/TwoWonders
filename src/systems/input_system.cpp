#include "input_system.hpp"

void InputSystem::update(EntityManager& em)
{
    auto& li = em.getSingleton<LevelInfo>();
    auto& bb = em.getSingleton<BlackBoard_t>();
    auto* playerEn = em.getEntityByID(li.playerID);

    // DEBUG
    if (IsKeyReleased(KEY_F1))
        debugMode = !debugMode;

    if (debugMode)
        return;

    if (!playerEn->hasTag<PlayerTag>() && IsKeyReleased(KEY_ENTER))
        em.destroyAll();

    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, PhysicsComponent& phy, InputComponent& in)
    {
        // Resetear la velocidad
        phy.velocity = {};
        phy.a_linear = phy.v_angular = 0;
        auto& vel = phy.velocity;

        // Actualizar la velocidad
        int keysPressed = 0;
        if(phy.blockXZ){
          // std::cout << "BLOQUEADOS \n";
            if (IsKeyDown(in.up)) {
                //std::cout << "SUBIRRR";
                vel.setY(vel.y() + INP_SPEED);
                in.last_key = in.up;
                keysPressed++;
            }
            if (IsKeyDown(in.down)) {
                vel.setY(vel.y() - INP_SPEED);
                in.last_key = in.down;
                keysPressed++;
            }
        }else{
           // std::cout << "NO BLOQUEADOS \n";
             if (IsKeyDown(in.right)) {
                vel.setX(vel.x() + INP_SPEED);
                vel.setZ(vel.z() - INP_SPEED);
                in.last_key = in.right;
                keysPressed++;
            }
            if (IsKeyDown(in.left)) {
                vel.setX(vel.x() - INP_SPEED);
                vel.setZ(vel.z() + INP_SPEED);
                in.last_key = in.left;
                keysPressed++;
            }
            if (IsKeyDown(in.up)) {
                vel.setX(vel.x() - INP_SPEED);
                vel.setZ(vel.z() - INP_SPEED);
                in.last_key = in.up;
                keysPressed++;
            }
            if (IsKeyDown(in.down)) {
                vel.setX(vel.x() + INP_SPEED);
                vel.setZ(vel.z() + INP_SPEED);
                in.last_key = in.down;
                keysPressed++;
            }
        }


        if (keysPressed == 2)
        {
            vel.normalize();
            vel *= INP_SPEED;
        }

        // if(IsKeyDown(in.seek) && !bb.tactive){
        //    // bb = { phy.position.x() , phy.position.z(), true, e.getID() };
        //     bb.behaviour = SB::Seek;
        // }
        // if(IsKeyDown(in.arrive) && !bb.tactive){
        //    // bb = { phy.position.x() , phy.position.z(), true, e.getID() };
        //     bb.behaviour = SB::Arrive;
        // }
        // if(IsKeyDown(in.Flee) && !bb.tactive){
        //    // bb = { phy.position.x() , phy.position.z(), true, e.getID() };
        //     bb.behaviour = SB::Flee;
        // }
        // if(IsKeyDown(in.Pursue) && !bb.tactive){
        //    // bb = { phy.position.x() , phy.position.z(), true, e.getID() };
        //     bb.behaviour = SB::Pursue;
        // }
        // if(IsKeyDown(in.Avoid) && !bb.tactive){
        //    // bb = { phy.position.x() , phy.position.z(), true, e.getID() };
        //     bb.behaviour = SB::Avoid;
        // }
        // if(IsKeyDown(in.pathfollow) && !bb.tactive){
        //    // bb = { phy.position.x() , phy.position.z(), true, e.getID() };
        //     bb.behaviour = SB::followPath;
        //     bb.tactive = true;
        // }
        // if(bb.behaviour != SB::followPath && bb.behaviour != SB::Arrive){
        bb.tx = phy.position.x();
        bb.tz = phy.position.z();
        bb.tactive = true;
        bb.teid = e.getID();
        // }
        // if(bb.behaviour == SB::Arrive){
        //     bb.tx = 0.0;
        //     bb.tz = 0.0;
        //     bb.tactive = true;
        // }

        // Codigo para el ataque
        if (IsKeyDown(in.space) && e.hasComponent<AttackComponent>())
            em.getComponent<AttackComponent>(e).attack(AttackType::AttackPlayer);

        // Codigo para la bomba
        if (IsKeyDown(KEY_B) && e.hasComponent<AttackComponent>())
            em.getComponent<AttackComponent>(e).attack(AttackType::Bomb);

        // Codigo para cambiar de tipo de ataque
        if (IsKeyReleased(KEY_Q) && e.hasComponent<TypeComponent>())
            em.getComponent<TypeComponent>(e).changeType();

        // Codigo para curarse // DEBUG
        // if(IsKeyDown(KEY_Z) && e.hasComponent<LifeComponent>())
        //     em.getComponent<LifeComponent>(e).increaseLife();

        // Código pa correr
        //
        // if (IsKeyDown(in.space))
        // {
        //     if (vel.x() != 0.0f || vel.z() != 0.0f)
        //     {
        //         // Esto no funcionará si la velocidad del input es igual a la velocidad máxima del PhysicsSystem
        //         vel *= (INP_SPEED * 3.5f);
        //     }
        // }

        // Normalizar la velocidad si se está moviendo en diagonal
        if (vel.x() != 0.0f && vel.z() != 0.0f)
        {
            vel.normalize();
        }
    });
}

bool InputSystem::pressEnter() {
    return IsKeyReleased(KEY_ENTER);
}
