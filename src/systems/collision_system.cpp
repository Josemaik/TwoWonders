#include "collision_system.hpp"

void CollisionSystem::update(EntityManager<PhysicsComponent, RenderComponent, Entity>& em)
{
    em.forAll([](Entity& e)
    {
        Vector3 position = { 2, 0, -2 };
        Vector3 scale = { 1, 1, 1 };

        Vector3 min = { position.x - scale.x / 2, position.y - scale.y / 2, position.z - scale.z / 2 };
        Vector3 max = { position.x + scale.x / 2, position.y + scale.y / 2, position.z + scale.z / 2 };

        BoundingBox enemy = { min, max };

        BoundingBox b = e.render.getBoundingBox();
        std::cout << b.min.x << " " << -10.f - e.physics.direction.x << std::endl;

        if (b.min.x < -10.0f - e.physics.direction.x)
            e.physics.position.x = -10.0f + e.render.scale.x / 2;
        if (b.max.x > 10.0f - e.physics.direction.x)
            e.physics.position.x = 10.0f - e.render.scale.x / 2;
        if (b.min.z < -10.0f - e.physics.direction.z)
            e.physics.position.z = -10.0f + e.render.scale.z / 2;
        if (b.max.z > 10.0f - e.physics.direction.z)
            e.physics.position.z = 10.0f - e.render.scale.z / 2;


        if (CheckCollisionBoxes(b, enemy))
        {
            std::cout << "Colioname esta" << std::endl;
            std::cout << " bbbbb " << e.physics.position.x << std::endl;
            std::cout << enemy.min.x << " " << b.max.x << std::endl;

            if (b.max.x + e.physics.direction.x * 8 > enemy.min.x - e.physics.direction.x)
                e.physics.position.x -= e.physics.direction.x;
            if (b.min.x > enemy.max.x - e.physics.direction.x)
                e.physics.position.x += e.physics.direction.x;

            if (b.max.z > enemy.min.z - e.physics.direction.z)
                e.physics.position.z -= e.physics.direction.z;
            if (b.min.z > enemy.max.z - e.physics.direction.z)
                e.physics.position.z += e.physics.direction.z;


            // if (b.max.z + e.physics.direction.z * 8 > enemy.min.z - e.physics.direction.z)
            //     e.physics.position.z -= e.physics.direction.z;





        }
    });
}