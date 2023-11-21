#include "physics_system.hpp"

void PhysicsSystem::update(EntityManager<PhysicsComponent, RenderComponent, Entity>& em)
{
    em.forAll([](Entity& e)
    {
        e.physics.position.x += e.physics.velocity.x;
        e.physics.position.y += e.physics.velocity.y;
        e.physics.position.z += e.physics.velocity.z;
        std::cout << e.physics.position.x << std::endl;
        e.physics.direction = { e.physics.velocity.x, e.physics.velocity.y ,e.physics.velocity.z };
    });
}