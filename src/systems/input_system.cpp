#include "input_system.hpp"
#include <math.h>

void InputSystem::update(EntityManager& em)
{
    em.forEach<SYSCMPs, SYSTAGs>([&](Entity&, PhysicsComponent& phy, InputComponent& in)
    {
        // Resetear la velocidad
        phy.velocity = {};

        // Actualizar la velocidad
        if (IsKeyDown(in.right)) phy.velocity.x += SPEED;
        if (IsKeyDown(in.left)) phy.velocity.x -= SPEED;
        if (IsKeyDown(in.up)) phy.velocity.z -= SPEED;
        if (IsKeyDown(in.down)) phy.velocity.z += SPEED;

        // Normalizar la velocidad si se está moviendo en diagonal
        if (phy.velocity.x != 0.0f && phy.velocity.z != 0.0f)
        {
            normalize(phy.velocity);
        }
    });
}

void InputSystem::normalize(Vector3& vel)
{
    float length = sqrt(vel.x * vel.x + vel.z * vel.z);
    vel.x /= length;
    vel.z /= length;

    // Aplicar la velocidad constante
    vel.x *= SPEED;
    vel.z *= SPEED;
}