#include "input_system.hpp"

void InputSystem::update(EntityManager& em)
{
    auto& li = em.getSingleton<LevelInfo>();
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
        if (IsKeyDown(in.right)) {
            // vel.setX(vel.x() + INP_SPEED);
            phy.v_angular = phy.kMaxVAng;
            in.last_key = in.right;
        }
        if (IsKeyDown(in.left)) {
            // vel.setX(vel.x() - INP_SPEED);
             phy.v_angular = -phy.kMaxVAng;
            in.last_key = in.left;
        }
        if (IsKeyDown(in.up)) {
            // vel.setZ(1);
            phy.a_linear = phy.kMaxAlin;
            in.last_key = in.up;

        }
        if (IsKeyDown(in.down)) {
            // vel.setZ(-1);
            phy.a_linear = -phy.kMaxAlin;
            in.last_key = in.down;
        }

        // Codigo para el ataque
        if (IsKeyDown(in.space) && e.hasComponent<AttackComponent>())
            em.getComponent<AttackComponent>(e).attack(AttackType::AttackPlayer);

        // Codigo para la bomba
        if (IsKeyDown(KEY_B) && e.hasComponent<AttackComponent>())
            em.getComponent<AttackComponent>(e).attack(AttackType::Bomb);

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