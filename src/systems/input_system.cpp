#include "input_system.hpp"
#include <math.h>
#include "../managers/entity_manager.hpp"


void InputSystem::update(EntityManager& em)
{
    em.forEach<SYSCMPs, SYSTAGs>([&](Entity&, PhysicsComponent& phy, InputComponent& in)
    {
        // Resetear la velocidad
        phy.velocity = {};
        auto& vel = phy.velocity;
        // Actualizar la velocidad
        if (IsKeyDown(in.right)) vel.setX(vel.x() + SPEED);
        if (IsKeyDown(in.left)) vel.setX(vel.x() - SPEED);
        if (IsKeyDown(in.up)) vel.setZ(vel.z() - SPEED);
        if (IsKeyDown(in.down)) vel.setZ(vel.z() + SPEED);

        // Código pa correr
        //
        // if (IsKeyDown(in.space))
        // {
        //     if (vel.x() != 0.0f || vel.z() != 0.0f)
        //     {
        //         vel *= (SPEED * 3.5f);
        //     }
        // }


        // Normalizar la velocidad si se está moviendo en diagonal
        if (vel.x() != 0.0f && vel.z() != 0.0f)
        {
            vel.normalize();
        }
    });
}