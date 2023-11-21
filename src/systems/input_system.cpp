#include "input_system.hpp"

void InputSystem::update(EntityManager<PhysicsComponent, RenderComponent, Entity>& em)
{
    em.forAll([](Entity& e)
    {
        e.physics.velocity = {};
        if (IsKeyDown(KEY_D)) e.physics.velocity.x += .4f;
        if (IsKeyDown(KEY_A)) e.physics.velocity.x -= .4f;
        if (IsKeyDown(KEY_W)) e.physics.velocity.z -= .4f;
        if (IsKeyDown(KEY_S)) e.physics.velocity.z += .4f;
    });
}