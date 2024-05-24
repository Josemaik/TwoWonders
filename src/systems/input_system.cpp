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
        if (ge.isKeyReleased(D_KEY_ENTER) || ge.isGamepadButtonReleased(0, GLFW_GAMEPAD_BUTTON_A))
            li.resetFromDeath = true;
        return;
    }
    // PAUSE
    if ((ge.isKeyReleased(D_KEY_ESCAPE) || ge.isGamepadButtonReleased(0, GLFW_GAMEPAD_BUTTON_START)) && li.currentScreen == GameScreen::GAMEPLAY)
    {
        if (li.currentScreen != GameScreen::CONTROLS)
        {
            inpi.pause = !inpi.pause;
            em.getSingleton<SoundSystem>().seleccion_menu();
            if (em.getSingleton<SoundSystem>().music_started)
            {
                em.getSingleton<SoundSystem>().sonido_unpause(li.mapID);
            }
        }

        inpi.debugAI1 = false;
        inpi.debugAI2 = false;
        inpi.debugPhy = false;
        inpi.inventory = false;
        return;
    }

    // INVENTORY
    if ((ge.isKeyReleased(D_KEY_I) || ge.isGamepadButtonReleased(0, GLFW_GAMEPAD_BUTTON_BACK)) && li.currentScreen == GameScreen::GAMEPLAY && !inpi.pause)
    {
        inpi.inventory = !inpi.inventory;
        inpi.debugAI1 = false;
        inpi.debugAI2 = false;
        inpi.debugPhy = false;
        return;
    }

    // DEBUG PHYSICS
    if (ge.isKeyReleased(D_KEY_F1))
    {
        inpi.debugPhy = !inpi.debugPhy;
        inpi.debugAI1 = false;
        inpi.debugAI2 = false;
        inpi.pause = false;
        inpi.inventory = false;
        return;
    }

    //DEBUG AI - Stop Game
    if (ge.isKeyReleased(D_KEY_F2))
    {
        inpi.debugAI1 = !inpi.debugAI1;
        inpi.debugPhy = false;
        inpi.debugAI2 = false;
        inpi.pause = false;
        inpi.inventory = false;
        return;
    }

    //DEBUG AI - Real Time
    if (ge.isKeyReleased(D_KEY_F3))
    {
        inpi.debugAI2 = !inpi.debugAI2;
        inpi.debugPhy = false;
        inpi.debugAI1 = false;
        inpi.pause = false;
        return;
    }

    if (ge.isKeyReleased(D_KEY_F4))
    {
        inpi.pathfind = !inpi.pathfind;
        inpi.debugPhy = false;
        inpi.debugAI1 = false;
        inpi.debugAI2 = false;
        inpi.pause = false;
        return;
    }

    // Dibujar por consola el arbol de la escena
    if (ge.isKeyReleased(D_KEY_F7))
        ge.dmeg.DrawTree();

    if (ge.isKeyReleased(D_KEY_F9)) {
        inpi.cheats = !inpi.cheats;
    }

    if (ge.isKeyReleased(D_KEY_F10))
        ge.dmeg.SetWindowSize(800, 600);

    if (ge.isKeyReleased(D_KEY_F11))
        ge.setWindowFullScreen();

    if (ge.isKeyReleased(D_KEY_F12)) {
        ge.dmeg.ToggleShaderCartoon();
        ge.toggleLights();
    }

    // Pruebas Shader Cartoon
    /*
    if(ge.dmeg.IsKeyDown(D_KEY_1))
        ge.dmeg.SetDepthThresholdValue(ge.dmeg.GetDepthThresholdValue() + 0.01f);
    if(ge.dmeg.IsKeyDown(D_KEY_2))
        ge.dmeg.SetDepthThresholdValue(ge.dmeg.GetDepthThresholdValue() - 0.01f < 0 ? 0 : ge.dmeg.GetDepthThresholdValue() - 0.01f);

    if(ge.dmeg.IsKeyDown(D_KEY_3))
        ge.dmeg.SetNormalThresholdValue(ge.dmeg.GetNormalThresholdValue() + 0.01f);
    if(ge.dmeg.IsKeyDown(D_KEY_4))
        ge.dmeg.SetNormalThresholdValue(ge.dmeg.GetNormalThresholdValue() - 0.01f < 0 ? 0 : ge.dmeg.GetNormalThresholdValue() - 0.01f);

    if(ge.dmeg.IsKeyDown(D_KEY_5))
        ge.dmeg.SetOutlineColor(ge.dmeg.GetOutlineColor() + glm::vec4({0.0f, 0.0f, 0.0f, 0.01f}));
    if(ge.dmeg.IsKeyDown(D_KEY_6))
        ge.dmeg.SetOutlineColor(ge.dmeg.GetOutlineColor() - glm::vec4({0.0f, 0.0f, 0.0f, 0.01f}));

    std::cout << "-------------------------\n";
    std::cout << "DepthThreshold: " << ge.dmeg.GetDepthThresholdValue() << "\n";
    std::cout << "NormalThreshold: " << ge.dmeg.GetNormalThresholdValue() << "\n";
    */

    // Sacamos las físicas y el input del jugador
    auto& phy = em.getComponent<PhysicsComponent>(player);
    if (phy.stopped && phy.elapsed_afterStop >= phy.countdown_afterStop)
        return;

    if (ge.isKeyPressed(D_KEY_0))
        plfi.hasKey = true;

    if (ge.isKeyPressed(D_KEY_1))
    {
        if (li.mapID == 0)
            phy.position = { -116.0, 6.0, 111.0 };
        else if (li.mapID == 1)
            phy.position = { 30.0, 13.0, 213.0 };
    }

    auto& in = em.getComponent<InputComponent>(player);

    // Actualizar la velocidad
    int keysPressed = 0;

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

                    joystick_x = ge.getGamepadAxisMovement(0, D_GAMEPAD_AXIS_LEFT_X) * -1;
                    joystick_y = ge.getGamepadAxisMovement(0, D_GAMEPAD_AXIS_LEFT_Y);

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

                    joystick_x = ge.getGamepadAxisMovement(0, D_GAMEPAD_AXIS_LEFT_X) * -1;
                    joystick_y = ge.getGamepadAxisMovement(0, D_GAMEPAD_AXIS_LEFT_Y);

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

    if (ge.isKeyReleased(D_KEY_Z))
    {
        Spell spell{ "Pompa de agua", "Disparas una potente concentración de agua que explota al impacto", AttackType::WaterBombShot };
        plfi.addSpell(spell);
    }

    if (ge.isKeyReleased(D_KEY_X))
    {
        Spell spell{ "Pompa de fuego", "Disparas una potente concentración de fuego que explota al impacto", AttackType::WaterDashArea };
        plfi.addSpell(spell);
    }

    if (ge.isKeyReleased(D_KEY_C))
    {
        Spell spell{ "Pompa de aire", "Disparas una potente concentración de aire que explota al impacto", AttackType::FireBallShot };
        plfi.addSpell(spell);
    }

    if (ge.isKeyReleased(D_KEY_V))
    {
        plfi.hasStaff = true;
    }

    // Código para el lock-in
    if ((ge.isKeyReleased(in.lockIn) || ge.getGamepadAxisMovement(0, in.m_lockIn) > 0.5) && elapsedLockIn >= elapsedLockInLimit)
    {
        inpi.lockOn = !inpi.lockOn;
        elapsedLockIn = 0.f;
    }
    elapsedLockIn += timeStep30;

    bb.tx = phy.position.x();
    bb.tz = phy.position.z();
    bb.tactive = true;
    bb.teid = player.getID();

    // Codigo para el ataque
    auto& lif = em.getComponent<LifeComponent>(player);
    if (player.hasComponent<AttackerComponent>() && !phy.notMove && !lif.onDeathAnim)
    {
        auto& anc = em.getComponent<AnimationComponent>(player);
        if ((ge.isKeyDown(in.space) || ge.getGamepadAxisMovement(0, in.m_space) > 0.1))
        {
            plfi.currentSpell = plfi.noSpell;
            anc.animToPlay = static_cast<std::size_t>(PlayerAnimations::MELEE_ATTACK);
            phy.stopped = true;
        }

        auto& atc = em.getComponent<AttackerComponent>(player);
        auto& pmc = em.getComponent<ParticleMakerComponent>(player);
        playerSpelled = false;
        if (atc.isAttackReady()) {
            if ((ge.isKeyDown(in.spell1) || ge.isGamepadButtonPressed(0, in.m_spell1)) && plfi.spellSlots[0] != plfi.noSpell)
            {
                if (plfi.spellSlots[0].cost > plfi.mana)
                {
                    em.getSingleton<SoundSystem>().sonido_no_mana();
                    return;
                }
                pmc.active = true;
                playerSpelled = true;
                plfi.currentSpell = plfi.spellSlots[0];
                em.getComponent<AttackerComponent>(player).attack(plfi.spellSlots[0].atkType);
            }
            else if ((ge.isKeyDown(in.spell2) || ge.isGamepadButtonPressed(0, in.m_spell2)) && plfi.spellSlots[1] != plfi.noSpell)
            {
                if (plfi.spellSlots[1].cost > plfi.mana)
                {
                    em.getSingleton<SoundSystem>().sonido_no_mana();
                    return;
                }
                pmc.active = true;
                playerSpelled = true;
                plfi.currentSpell = plfi.spellSlots[1];
                em.getComponent<AttackerComponent>(player).attack(plfi.spellSlots[1].atkType);
            }
            else if ((ge.isKeyDown(in.spell3) || ge.isGamepadButtonPressed(0, in.m_spell3)) && plfi.spellSlots[2] != plfi.noSpell)
            {
                if (plfi.spellSlots[2].cost > plfi.mana)
                {
                    em.getSingleton<SoundSystem>().sonido_no_mana();
                    return;
                }
                pmc.active = true;
                playerSpelled = true;
                plfi.currentSpell = plfi.spellSlots[2];
                em.getComponent<AttackerComponent>(player).attack(plfi.spellSlots[2].atkType);
            }

            // Restamos el maná
            if (playerSpelled)
            {
                anc.animToPlay = static_cast<std::size_t>(PlayerAnimations::RANGED_ATTACK);
                if (!(plfi.currentSpell.atkType & AttackType::WaterDashArea))
                    phy.stopped = true;

                plfi.mana -= plfi.currentSpell.cost;

                if (plfi.mana < 0.0)
                    plfi.mana = 0;
            }
        }

        if (pmc.active) {
            elapsedParticle -= ge.getFrameTime();
            if (elapsedParticle < 0) {
                pmc.active = false;
                pmc.particles.clear();
                elapsedParticle = elapsedParticleLimit;
            }
        }

        // Vamos sumando maná poco a poco
        if (!playerSpelled && plfi.mana < plfi.max_mana)
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

    if (ge.isGamepadButtonPressed(0, in.m_right) || ge.isKeyPressed(D_KEY_RIGHT))
        inpi.right = true;
    else
        inpi.right = false;

    if (ge.isGamepadButtonPressed(0, in.m_left) || ge.isKeyPressed(D_KEY_LEFT))
        inpi.left = true;
    else
        inpi.left = false;

    if (ge.isGamepadButtonPressed(0, in.m_up) || ge.isKeyPressed(D_KEY_UP))
        inpi.up = true;
    else
        inpi.up = false;

    if (ge.isGamepadButtonPressed(0, in.m_down) || ge.isKeyPressed(D_KEY_DOWN))
        inpi.down = true;
    else
        inpi.down = false;

    // Codigo para cambiar de tipo de ataque
    // if (ge.isKeyReleased(KEY_Q))
    //     plfi.changeCurrentSpell();

    // Codigo para curarse // DEBUG
    if (ge.isKeyDown(D_KEY_Z) && player.hasComponent<LifeComponent>())
        em.getComponent<LifeComponent>(player).increaseLife();

    if (ge.isKeyDown(D_KEY_M)) {
        li.npcflee = true;
    }
}

bool InputSystem::pressEnter(GameEngine& ge) {
    return ge.isKeyReleased(D_KEY_ENTER) || ge.isGamepadButtonReleased(0, GLFW_GAMEPAD_BUTTON_A);
}