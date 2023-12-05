#include "input_system.hpp"

void InputSystem::update(EntityManager& em)
{
    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, PhysicsComponent& phy, InputComponent& in)
    {
        // Resetear la velocidad
        phy.velocity = {};
        auto& vel = phy.velocity;
        // Actualizar la velocidad
        if (IsKeyDown(in.right)){
            vel.setX(vel.x() + INP_SPEED);
            in.last_key = in.right;
        }
        if (IsKeyDown(in.left)){
            vel.setX(vel.x() - INP_SPEED);
            in.last_key = in.left;
        } 
        if (IsKeyDown(in.up)){
            vel.setZ(vel.z() - INP_SPEED);
            in.last_key = in.up;
        } 
        if (IsKeyDown(in.down)){
            vel.setZ(vel.z() + INP_SPEED);
            in.last_key = in.down;
        } 

        // Codigo para el ataque
        if(IsKeyDown(in.space) && e.hasComponent<AttackComponent>())
            em.getComponent<AttackComponent>(e).attack(); 

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