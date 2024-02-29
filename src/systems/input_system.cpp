#include "input_system.hpp"

void InputSystem::update(EntityManager& em, GameEngine& ge)
{
    auto& li = em.getSingleton<LevelInfo>();
    auto& inpi = em.getSingleton<InputInfo>();
    auto& bb = em.getSingleton<BlackBoard_t>();
    auto* playerEn = em.getEntityByID(li.playerID);

    // PAUSE
    if (ge.isKeyReleased(KEY_ESCAPE) && li.currentScreen == GameScreen::GAMEPLAY)
    {
        inpi.pause = !inpi.pause;
        inpi.debugAI1 = false;
        inpi.debugAI2 = false;
        inpi.debugPhy = false;
        inpi.inventory = false;
        return;
    }

    // INVENTORY
    if (ge.isKeyReleased(KEY_I) && li.currentScreen == GameScreen::GAMEPLAY && !inpi.pause)
    {
        inpi.inventory = !inpi.inventory;
        inpi.debugAI1 = false;
        inpi.debugAI2 = false;
        inpi.debugPhy = false;
        return;
    }

    // DEBUG PHYSICS
    if (ge.isKeyReleased(KEY_F1))
    {
        inpi.debugPhy = !inpi.debugPhy;
        inpi.debugAI1 = false;
        inpi.debugAI2 = false;
        inpi.pause = false;
        inpi.inventory = false;
        return;
    }

    //DEBUG AI - Stop Game
    if (ge.isKeyReleased(KEY_F2))
    {
        inpi.debugAI1 = !inpi.debugAI1;
        inpi.debugPhy = false;
        inpi.debugAI2 = false;
        inpi.pause = false;
        inpi.inventory = false;
        return;
    }

    //DEBUG AI - Real Time
    if (ge.isKeyReleased(KEY_F3))
    {
        inpi.debugAI2 = !inpi.debugAI2;
        inpi.debugPhy = false;
        inpi.debugAI1 = false;
        inpi.pause = false;
        return;
    }

    if (!playerEn->hasTag<PlayerTag>() && ge.isKeyReleased(KEY_ENTER))
    {
        li.resetGame = true;
        return;
    }

    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, PhysicsComponent& phy, InputComponent& in)
    {
        // Resetear la velocidad
        phy.velocity = {};
        auto& vel = phy.velocity;

        // Actualizar la velocidad
        int keysPressed = 0;

        if (ge.isKeyDown(in.air_attack) && e.hasComponent<AttackComponent>()) {
            em.getComponent<AttackComponent>(e).pos_respawn_air_attack = phy.position;
            em.getComponent<AttackComponent>(e).attack(AttackType::AirAttack);
        }
        // if(phy.blockXZ){
        //     if (ge.isKeyDown(in.up)) {
        //         vel.setY(vel.y() + INP_SPEED);
        //         in.last_key = in.up;
        //         keysPressed++;
        //     }
        //     if (ge.isKeyDown(in.down)) {
        //         vel.setY(vel.y() - INP_SPEED);
        //         in.last_key = in.down;
        //         keysPressed++;
        //     }
        // }else{

        // Código de movimiento
        if (ge.isKeyDown(in.right))
        {
            vel.setX(vel.x() + INP_SPEED);
            vel.setZ(vel.z() - INP_SPEED);

            keysPressed++;
        }
        if (ge.isKeyDown(in.left))
        {
            vel.setX(vel.x() - INP_SPEED);
            vel.setZ(vel.z() + INP_SPEED);

            keysPressed++;
        }
        if (ge.isKeyDown(in.up))
        {
            vel.setX(vel.x() - INP_SPEED);
            vel.setZ(vel.z() - INP_SPEED);

            keysPressed++;
        }
        if (ge.isKeyDown(in.down))
        {

            vel.setX(vel.x() + INP_SPEED);
            vel.setZ(vel.z() + INP_SPEED);

            keysPressed++;
        }

        if (ge.isGamepadAvailable(0))
        {
            // Obtén el movimiento del joystick
            float joystick_x = ge.getGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X) * -1;
            float joystick_y = ge.getGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);

            float deadzone = 0.3f;

            // Deadzone
            if (joystick_x < deadzone && joystick_x > -deadzone)
                joystick_x = 0.0f;

            if (joystick_y < deadzone && joystick_y > -deadzone)
                joystick_y = 0.0f;

            // Ajusta la velocidad basándote en el movimiento del joystick
            vel.setX(vel.x() + (joystick_y - joystick_x) * INP_SPEED);
            vel.setZ(vel.z() + (joystick_y + joystick_x) * INP_SPEED);
        }

        if (keysPressed == 2)
        {
            vel.normalize();
            vel *= INP_SPEED;
        }

        if (ge.isKeyReleased(in.lockIn) || ge.isGamepadButtonReleased(0, in.m_lockIn))
            inpi.lockOn = !inpi.lockOn;

        bb.tx = phy.position.x();
        bb.tz = phy.position.z();
        bb.tactive = true;
        bb.teid = e.getID();

        auto& plfi = em.getSingleton<PlayerInfo>();

        // Codigo para el ataque
        if ((ge.isKeyDown(in.space) || ge.isGamepadButtonPressed(0, in.m_space)) && e.hasComponent<AttackComponent>())
            em.getComponent<AttackComponent>(e).attack(AttackType::AttackPlayer);
        else if (plfi.mana < plfi.max_mana)
        {
            plfi.mana += .1;

            if (plfi.mana > plfi.max_mana)
                plfi.mana = plfi.max_mana;
        }

        if (ge.isKeyPressed(in.interact) || ge.isGamepadButtonPressed(0, in.m_interact))
            inpi.interact = true;
        else if (ge.isKeyReleased(in.interact) || ge.isGamepadButtonReleased(0, in.m_interact))
            inpi.interact = false;

        // Codigo para la bomba
        if (ge.isKeyDown(KEY_B) && e.hasComponent<AttackComponent>())
            em.getComponent<AttackComponent>(e).attack(AttackType::Bomb);

        // Codigo para cambiar de tipo de ataque
        if (ge.isKeyReleased(KEY_Q))
            plfi.changeCurrentSpell();

        // Codigo para curarse // DEBUG
        if (ge.isKeyDown(KEY_Z) && e.hasComponent<LifeComponent>())
            em.getComponent<LifeComponent>(e).increaseLife();

        // Normalizar la velocidad si se está moviendo en diagonal
        if (vel.x() != 0.0f && vel.z() != 0.0f)
        {
            vel.normalize();
        }
    });
}

bool InputSystem::pressEnter(GameEngine& ge) {
    return ge.isKeyReleased(KEY_ENTER);
}
