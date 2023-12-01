#include "input_system.hpp"

void InputSystem::update(EntityManager& em)
{
    em.forEach<SYSCMPs, SYSTAGs>([&](Entity&, PhysicsComponent& phy, InputComponent& in)
    {
        // Resetear la velocidad
        phy.velocity = {};
        auto& vel = phy.velocity;
        // Actualizar la velocidad
        if (IsKeyDown(in.right)) vel.setX(vel.x() + INP_SPEED);
        if (IsKeyDown(in.left)) vel.setX(vel.x() - INP_SPEED);
        if (IsKeyDown(in.up)) vel.setZ(vel.z() - INP_SPEED);
        if (IsKeyDown(in.down)) vel.setZ(vel.z() + INP_SPEED);

        // Codigo para el ataque
        //if(IsKeyDown(in.space) && e.hasComponent<AttackComponent>()){
        //    auto& a{ em.getComponent<AttackComponent>(e) };
        //    a.attack(em);
        //}   

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