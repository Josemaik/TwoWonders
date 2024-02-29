#include "input_system.hpp"

void InputSystem::update(EntityManager& em)
{
    auto& li = em.getSingleton<LevelInfo>();
    auto& inpi = em.getSingleton<InputInfo>();
    auto& bb = em.getSingleton<BlackBoard_t>();
    auto* playerEn = em.getEntityByID(li.playerID);

    // PAUSE
    if (IsKeyReleased(KEY_ESCAPE) && li.currentScreen == GameScreen::GAMEPLAY)
    {
        inpi.pause = !inpi.pause;
        inpi.debugAI1 = false;
        inpi.debugAI2 = false;
        inpi.debugPhy = false;
        inpi.inventory = false;
        return;
    }

    // INVENTORY
    if (IsKeyReleased(KEY_I) && li.currentScreen == GameScreen::GAMEPLAY && !inpi.pause)
    {
        inpi.inventory = !inpi.inventory;
        inpi.debugAI1 = false;
        inpi.debugAI2 = false;
        inpi.debugPhy = false;
        return;
    }

    // DEBUG PHYSICS
    if (IsKeyReleased(KEY_F1))
    {
        inpi.debugPhy = !inpi.debugPhy;
        inpi.debugAI1 = false;
        inpi.debugAI2 = false;
        inpi.pause = false;
        inpi.inventory = false;
        return;
    }

    //DEBUG AI - Stop Game
    if (IsKeyReleased(KEY_F2))
    {
        inpi.debugAI1 = !inpi.debugAI1;
        inpi.debugPhy = false;
        inpi.debugAI2 = false;
        inpi.pause = false;
        inpi.inventory = false;
        return;
    }

    //DEBUG AI - Real Time
    if (IsKeyReleased(KEY_F3))
    {
        inpi.debugAI2 = !inpi.debugAI2;
        inpi.debugPhy = false;
        inpi.debugAI1 = false;
        inpi.pause = false;
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
        auto& vel = phy.velocity;

        // Actualizar la velocidad
        int keysPressed = 0;

        if (IsKeyDown(in.air_attack) && e.hasComponent<AttackComponent>()) {
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

        // Código de movimiento
        if (IsKeyDown(in.right))
        {
            vel.setX(vel.x() + INP_SPEED);
            vel.setZ(vel.z() - INP_SPEED);

            keysPressed++;
        }
        if (IsKeyDown(in.left))
        {
            vel.setX(vel.x() - INP_SPEED);
            vel.setZ(vel.z() + INP_SPEED);

            keysPressed++;
        }
        if (IsKeyDown(in.up))
        {
            vel.setX(vel.x() - INP_SPEED);
            vel.setZ(vel.z() - INP_SPEED);

            keysPressed++;
        }
        if (IsKeyDown(in.down))
        {

            vel.setX(vel.x() + INP_SPEED);
            vel.setZ(vel.z() + INP_SPEED);

            keysPressed++;
        }

        if (IsGamepadAvailable(0))
        {
            // Obtén el movimiento del joystick
            float joystick_x = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X) * -1;
            float joystick_y = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);

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

        if (IsKeyReleased(in.lockIn) || IsGamepadButtonReleased(0, in.m_lockIn))
            inpi.lockOn = !inpi.lockOn;

        bb.tx = phy.position.x();
        bb.tz = phy.position.z();
        bb.tactive = true;
        bb.teid = e.getID();

        auto& plfi = em.getSingleton<PlayerInfo>();

        // Codigo para el ataque
        if ((IsKeyDown(in.space) || IsGamepadButtonPressed(0, in.m_space)) && e.hasComponent<AttackComponent>())
            em.getComponent<AttackComponent>(e).attack(AttackType::AttackPlayer);
        else if (plfi.mana < plfi.max_mana)
        {
            plfi.mana += .1;

            if (plfi.mana > plfi.max_mana)
                plfi.mana = plfi.max_mana;
        }

        if (IsKeyPressed(in.interact) || IsGamepadButtonPressed(0, in.m_interact))
            inpi.interact = true;
        else if (IsKeyReleased(in.interact) || IsGamepadButtonReleased(0, in.m_interact))
            inpi.interact = false;

        // Codigo para la bomba
        if (IsKeyDown(KEY_B) && e.hasComponent<AttackComponent>())
            em.getComponent<AttackComponent>(e).attack(AttackType::Bomb);

        // Codigo para cambiar de tipo de ataque
        if (IsKeyReleased(KEY_Q))
            plfi.changeCurrentSpell();

        // Codigo para curarse // DEBUG
        if (IsKeyDown(KEY_Z) && e.hasComponent<LifeComponent>())
            em.getComponent<LifeComponent>(e).increaseLife();

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
