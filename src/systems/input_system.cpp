#include "input_system.hpp"

void InputSystem::update(EntityManager& em, GameEngine& ge)
{
    auto& li = em.getSingleton<LevelInfo>();
    auto& inpi = em.getSingleton<InputInfo>();
    auto& bb = em.getSingleton<BlackBoard_t>();
    auto& plfi = em.getSingleton<PlayerInfo>();
    auto& player = *em.getEntityByID(li.playerID);
    auto& gami = em.getSingleton<GameData>();

    if (li.isCharging())
        return;

    if (li.replay)
        gami.update();

    // Si no hay jugador, no hacemos nada
    if (li.isDead)
    {
        if (ge.isKeyReleased(KEY_ENTER) || ge.isGamepadButtonReleased(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))
            li.resetFromDeath = true;
        return;
    }

    // PAUSE
    if ((ge.isKeyReleased(KEY_ESCAPE) || ge.isGamepadButtonReleased(0, GAMEPAD_BUTTON_MIDDLE_RIGHT)) && li.currentScreen == GameScreen::GAMEPLAY)
    {
        if (li.currentScreen != GameScreen::CONTROLS)
            inpi.pause = !inpi.pause;
        inpi.debugAI1 = false;
        inpi.debugAI2 = false;
        inpi.debugPhy = false;
        inpi.inventory = false;
        return;
    }

    // INVENTORY
    if ((ge.isKeyReleased(KEY_I) || ge.isGamepadButtonReleased(0, GAMEPAD_BUTTON_MIDDLE) || ge.isGamepadButtonReleased(0, GAMEPAD_BUTTON_MIDDLE_LEFT)) && li.currentScreen == GameScreen::GAMEPLAY && !inpi.pause)
    {
        inpi.inventory = !inpi.inventory;
        inpi.debugAI1 = false;
        inpi.debugAI2 = false;
        inpi.debugPhy = false;
        return;
    }

    // // DEBUG PHYSICS
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

    // DEBUG AI - Real Time
    if (ge.isKeyReleased(KEY_F3))
    {
        inpi.debugAI2 = !inpi.debugAI2;
        inpi.debugPhy = false;
        inpi.debugAI1 = false;
        inpi.pause = false;
        return;
    }

    // // DEBUG NAVMESH
    if (ge.isKeyReleased(KEY_F4))
    {
        inpi.pathfind = !inpi.pathfind;
        inpi.debugPhy = false;
        inpi.debugAI1 = false;
        inpi.debugAI2 = false;
        inpi.pause = false;
        return;
    }

    // Sacamos las físicas y el input del jugador
    auto& phy = em.getComponent<PhysicsComponent>(player);
    if (phy.stopped && phy.elapsed_afterStop >= phy.countdown_afterStop)
        return;

    auto& in = em.getComponent<InputComponent>(player);

    // Actualizar la velocidad
    int keysPressed = 0;

    if (ge.isKeyDown(in.air_attack) && player.hasComponent<AttackComponent>()) {
        em.getComponent<AttackComponent>(player).pos_respawn_air_attack = phy.position;
        em.getComponent<AttackComponent>(player).attack(AttackType::AirAttack);
    }

    // Código de movimiento
    // std::chrono::duration<float, std::milli> velTime{};
    if (!phy.notMove)
    {
        // Resetear la velocidad
        phy.velocity = {};
        auto& vel = phy.velocity;

        if (!li.replay)
        {
            if (!plfi.onLadder)
            {

                if (ge.isKeyDown(in.right))
                {
                    vel.setX(vel.x() - INP_SPEED);
                    vel.setZ(vel.z() + INP_SPEED);

                    keysPressed++;
                }
                if (ge.isKeyDown(in.left))
                {
                    vel.setX(vel.x() + INP_SPEED);
                    vel.setZ(vel.z() - INP_SPEED);

                    keysPressed++;
                }
                if (ge.isKeyDown(in.up))
                {
                    vel.setX(vel.x() + INP_SPEED);
                    vel.setZ(vel.z() + INP_SPEED);

                    keysPressed++;
                }
                if (ge.isKeyDown(in.down))
                {

                    vel.setX(vel.x() - INP_SPEED);
                    vel.setZ(vel.z() - INP_SPEED);

                    keysPressed++;
                }

                if (ge.isGamepadAvailable(0))
                {
                    // Obtén el movimiento del joystick
                    auto& joystick_x = in.m_joystickX;
                    auto& joystick_y = in.m_joystickY;

                    joystick_x = ge.getGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X) * -1;
                    joystick_y = ge.getGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);

                    float deadzone = 0.3f;
                    float slowzone = 0.8f;

                    // Deadzone
                    if (joystick_x < deadzone && joystick_x > -deadzone)
                        joystick_x = 0.0f;

                    if (joystick_y < deadzone && joystick_y > -deadzone)
                        joystick_y = 0.0f;

                    // Ajusta la velocidad basándose en el movimiento del joystick
                    float speed = (std::abs(joystick_x) > slowzone || std::abs(joystick_y) > slowzone) ? INP_SPEED : INP_SPEED / 4;

                    vel.setX(vel.x() + (-joystick_y + joystick_x) * speed);
                    vel.setZ(vel.z() + (-joystick_y - joystick_x) * speed);
                }
            }
            else
            {
                if (ge.isKeyDown(in.up))
                {
                    vel.setY(vel.y() + INP_SPEED);
                    keysPressed++;
                }
                if (ge.isKeyDown(in.down))
                {
                    vel.setY(vel.y() - INP_SPEED);
                    keysPressed++;
                }

                if (ge.isGamepadAvailable(0))
                {
                    // Obtén el movimiento del joystick
                    auto& joystick_x = in.m_joystickX;
                    auto& joystick_y = in.m_joystickY;

                    joystick_x = ge.getGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X) * -1;
                    joystick_y = ge.getGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);

                    float deadzone = 0.3f;
                    float slowzone = 0.8f;

                    // Deadzone
                    if (joystick_x < deadzone && joystick_x > -deadzone)
                        joystick_x = 0.0f;

                    if (joystick_y < deadzone && joystick_y > -deadzone)
                        joystick_y = 0.0f;

                    // Ajusta la velocidad basándose en el movimiento del joystick
                    float speed = (std::abs(joystick_x) > slowzone || std::abs(joystick_y) > slowzone) ? INP_SPEED : INP_SPEED / 4;

                    vel.setY(vel.y() + (-joystick_y) * speed);
                }
            }

            // Guardamos el registro de la velocidad
            gami.addMovementEvent(vel);
        }
        else
            gami.setVel(vel);
    }

    // if (ge.isKeyReleased(KEY_Z))
    // {
    //     Spell spell{ "Pompa de agua", "Disparas una potente concentración de agua que explota al impacto", Spells::WaterBomb, 20.0, 4 };
    //     plfi.addSpell(spell);
    // }

    // if (ge.isKeyReleased(KEY_X))
    // {
    //     Spell spell{ "Pompa de fuego", "Disparas una potente concentración de fuego que explota al impacto", Spells::WaterDash, 20.0, 4 };
    //     plfi.addSpell(spell);
    // }

    // if (ge.isKeyReleased(KEY_C))
    // {
    //     Spell spell{ "Pompa de aire", "Disparas una potente concentración de aire que explota al impacto", Spells::FireBall, 20.0, 4 };
    //     plfi.addSpell(spell);
    // }

    // if (ge.isKeyReleased(KEY_V))
    // {
    //     plfi.hasStaff = true;
    // }

    // Código para el lock-in
    if ((ge.isKeyReleased(in.lockIn) || ge.getGamepadAxisMovement(0, in.m_lockIn) > 0.5) && elapsedLockIn >= elapsedLockInLimit)
    {
        inpi.lockOn = !inpi.lockOn;
        elapsedLockIn = 0.f;
    }
    elapsedLockIn += timeStep;

    bb.tx = phy.position.x();
    bb.tz = phy.position.z();
    bb.tactive = true;
    bb.teid = player.getID();

    // Codigo para el ataque
    if (player.hasComponent<AttackComponent>())
    {
        if ((ge.isKeyDown(in.space) || ge.getGamepadAxisMovement(0, in.m_space) > 0.1))
        {
            inpi.melee = true;
            em.getComponent<AttackComponent>(player).attack(AttackType::AttackPlayer);
        }

        auto& atc = em.getComponent<AttackComponent>(player);
        if (atc.isAttackReady()) {
            if ((ge.isKeyDown(in.spell1) || ge.isGamepadButtonPressed(0, in.m_spell1)) && plfi.spellSlots[0] != plfi.noSpell)
            {
                inpi.spell1 = true;
                em.getComponent<AttackComponent>(player).attack(AttackType::AttackPlayer);
            }
            else if ((ge.isKeyDown(in.spell2) || ge.isGamepadButtonPressed(0, in.m_spell2)) && plfi.spellSlots[1] != plfi.noSpell)
            {
                inpi.spell2 = true;
                em.getComponent<AttackComponent>(player).attack(AttackType::AttackPlayer);
            }
            else if ((ge.isKeyDown(in.spell3) || ge.isGamepadButtonPressed(0, in.m_spell3)) && plfi.spellSlots[2] != plfi.noSpell)
            {
                inpi.spell3 = true;
                em.getComponent<AttackComponent>(player).attack(AttackType::AttackPlayer);
            }
        }

        if (plfi.mana < plfi.max_mana)
        {
            if (!plfi.hasHat)
                plfi.mana += .07;
            else plfi.mana += .20;

            if (plfi.mana > plfi.max_mana)
                plfi.mana = plfi.max_mana;
        }
    }

    if (ge.isKeyPressed(in.interact) || ge.isGamepadButtonPressed(0, in.m_interact))
        inpi.interact = true;
    else if (ge.isKeyReleased(in.interact) || ge.isGamepadButtonReleased(0, in.m_interact))
        inpi.interact = false;

    if (ge.isGamepadButtonPressed(0, in.m_right) || ge.isKeyPressed(KEY_RIGHT))
        inpi.right = true;
    else
        inpi.right = false;

    if (ge.isGamepadButtonPressed(0, in.m_left) || ge.isKeyPressed(KEY_LEFT))
        inpi.left = true;
    else
        inpi.left = false;

    if (ge.isGamepadButtonPressed(0, in.m_up) || ge.isKeyPressed(KEY_UP))
        inpi.up = true;
    else
        inpi.up = false;

    if (ge.isGamepadButtonPressed(0, in.m_down) || ge.isKeyPressed(KEY_DOWN))
        inpi.down = true;
    else
        inpi.down = false;

    // Codigo para cambiar de tipo de ataque
    // if (ge.isKeyReleased(KEY_Q))
    //     plfi.changeCurrentSpell();

    // Codigo para curarse // DEBUG
    if (ge.isKeyDown(KEY_Z) && player.hasComponent<LifeComponent>())
        em.getComponent<LifeComponent>(player).increaseLife();

    if (ge.isKeyReleased(KEY_F12))
    {
        if (phy.gravity == 0)
            phy.gravity = 1.0;
        else
            phy.gravity = 0;
    }
    if (ge.isKeyDown(KEY_M)) {
        li.npcflee = true;
    }

}

bool InputSystem::pressEnter(GameEngine& ge) {
    return ge.isKeyReleased(KEY_ENTER) || ge.isGamepadButtonReleased(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
}