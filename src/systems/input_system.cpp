#include "input_system.hpp"

void InputSystem::update(EntityManager& em)
{
    auto& li = em.getSingleton<LevelInfo>();
    auto& inpi = em.getSingleton<InputInfo>();
    auto& bb = em.getSingleton<BlackBoard_t>();
    auto* playerEn = em.getEntityByID(li.playerID);

    // DEBUG PHYSICS
    if (IsKeyReleased(KEY_F1))
    {
        inpi.debugPhy = !inpi.debugPhy;
        inpi.debugAI1 = false;
        inpi.debugAI2 = false;
        return;
    }

    //DEBUG AI - Stop Game
    if (IsKeyReleased(KEY_F2))
    {
        inpi.debugAI1 = !inpi.debugAI1;
        inpi.debugPhy = false;
        inpi.debugAI2 = false;
        return;
    }

    //DEBUG AI - Real Time
    if (IsKeyReleased(KEY_F3))
    {
        inpi.debugAI2 = !inpi.debugAI2;
        inpi.debugPhy = false;
        inpi.debugAI1 = false;
        return;
    }

    if (!playerEn->hasTag<PlayerTag>() && IsKeyReleased(KEY_ENTER))
    {
        li.resetGame = true;
        return;
    }

    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, PhysicsComponent& phy, InputComponent& in)
    {
        // Resetear la velocidad
        phy.velocity = {};
        phy.a_linear = phy.v_angular = 0;
        auto& vel = phy.velocity;

        // Actualizar la velocidad
        int keysPressed = 0;

        if(IsKeyDown(in.air_attack) && e.hasComponent<AttackComponent>()){
            em.getComponent<AttackComponent>(e).pos_respawn_air_attack = phy.position;
            em.getComponent<AttackComponent>(e).attack(AttackType::AirAttack);
        }
        // if(phy.blockXZ){
        //     if (IsKeyDown(in.up)) {
        //         vel.setY(vel.y() + INP_SPEED);
        //         in.last_key = in.up;
        //         keysPressed++;
        //     }
        //     if (IsKeyDown(in.down)) {
        //         vel.setY(vel.y() - INP_SPEED);
        //         in.last_key = in.down;
        //         keysPressed++;
        //     }
        // }else{
        if (IsKeyDown(in.right)) {

            if (!li.cameraChange)
            {
                vel.setX(vel.x() + INP_SPEED);
                vel.setZ(vel.z() - INP_SPEED);
            }
            else
            {
                vel.setX(vel.x() + INP_SPEED);
                vel.setZ(vel.z() + INP_SPEED);
            }

            in.last_key = in.right;
            keysPressed++;
        }
        if (IsKeyDown(in.left)) {

            if (!li.cameraChange)
            {
                vel.setX(vel.x() - INP_SPEED);
                vel.setZ(vel.z() + INP_SPEED);

            }
            else
            {
                vel.setX(vel.x() - INP_SPEED);
                vel.setZ(vel.z() - INP_SPEED);
            }

            in.last_key = in.left;
            keysPressed++;
        }
        if (IsKeyDown(in.up)) {

            if (!li.cameraChange)
            {
                vel.setX(vel.x() - INP_SPEED);
                vel.setZ(vel.z() - INP_SPEED);
            }
            else
            {
                vel.setX(vel.x() + INP_SPEED);
                vel.setZ(vel.z() - INP_SPEED);
            }

            in.last_key = in.up;
            keysPressed++;
        }
        if (IsKeyDown(in.down)) {

            if (!li.cameraChange)
            {
                vel.setX(vel.x() + INP_SPEED);
                vel.setZ(vel.z() + INP_SPEED);
            }
            else
            {
                vel.setX(vel.x() - INP_SPEED);
                vel.setZ(vel.z() + INP_SPEED);
            }

            in.last_key = in.down;
            keysPressed++;
        }
        // }


        if (keysPressed == 2)
        {
            vel.normalize();
            vel *= INP_SPEED;
        }

        if (IsKeyReleased(in.lockIn)) {
            // em.addComponent<AttackComponent>(e, AttackComponent{});
            // if(e.hasComponent<AttackComponent>()){
            //      em.getComponent<AttackComponent>(e).attack(AttackType::AreaAttack);
            // }
            inpi.lockOn = !inpi.lockOn;
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

        auto& plfi = em.getSingleton<PlayerInfo>();

        // Codigo para el ataque
        if (IsKeyDown(in.space) && e.hasComponent<AttackComponent>())
            em.getComponent<AttackComponent>(e).attack(AttackType::AttackPlayer);
        else if (plfi.mana < plfi.max_mana)
        {
            plfi.mana += .1;

            if (plfi.mana > plfi.max_mana)
                plfi.mana = plfi.max_mana;
        }

        if (IsKeyPressed(in.interact))
        {
            inpi.interact = true;
        }
        else if (IsKeyReleased(in.interact))
        {
            inpi.interact = false;
        }

        // Codigo para la bomba
        if (IsKeyDown(KEY_B) && e.hasComponent<AttackComponent>())
            em.getComponent<AttackComponent>(e).attack(AttackType::Bomb);

        // Codigo para cambiar de tipo de ataque
        if (IsKeyReleased(KEY_Q) && e.hasComponent<TypeComponent>())
            em.getComponent<TypeComponent>(e).changeType();

        // Codigo para curarse // DEBUG
        if (IsKeyDown(KEY_Z) && e.hasComponent<LifeComponent>())
            em.getComponent<LifeComponent>(e).increaseLife();

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
