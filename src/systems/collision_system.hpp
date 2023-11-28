#pragma once
#ifndef COLLISION_MANAGER
#define COLLISION_MANAGER
#include <iostream>
#include <raylib.h>
#include "../managers/entity_manager.hpp"
#include "../managers/game_engine.hpp"

struct CollisionSystem
{
    using SYSCMPs = MP::TypeList<PhysicsComponent, RenderComponent>;
    using SYSTAGs = MP::TypeList<>;

    void update(EntityManager& em)
    {
        std::vector<std::pair<PhysicsComponent&, BoundingBox>> boxes;
        em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, PhysicsComponent& phy, RenderComponent& ren)
        {
            BoundingBox b = phy.getBoundingBox(ren.scale);
            boxes.push_back(std::make_pair(std::ref(phy), b));

            if (e.hasTag<PlayerTag>())
            {
                if (b.min.x < -10.0f)
                    phy.position.x = -10.0f + ren.scale.x / 2;
                if (b.max.x > 10.0f)
                    phy.position.x = 10.0f - ren.scale.x / 2;
                if (b.min.z < -10.0f)
                    phy.position.z = -10.0f + ren.scale.z / 2;
                if (b.max.z > 10.0f)
                    phy.position.z = 10.0f - ren.scale.z / 2;
            }
        });

        for (size_t i = 0; i < boxes.size(); ++i)
        {
            for (size_t j = i + 1; j < boxes.size(); ++j)
            {
                if (CheckCollisionBoxes(boxes[i].second, boxes[j].second))
                {
                    std::cout << "Colision detectada" << std::endl;

                    PhysicsComponent& phy = boxes[i].first;

                    if (boxes[i].second.max.x > boxes[j].second.min.x)
                        phy.position.x -= phy.direction.x;
                    if (boxes[i].second.min.x > boxes[j].second.max.x)
                        phy.position.x += phy.direction.x;
                    if (boxes[i].second.max.z > boxes[j].second.min.z)
                        phy.position.z -= phy.direction.z;
                    if (boxes[i].second.min.z > boxes[j].second.max.z)
                        phy.position.z += phy.direction.z;

                }
            }
        }
    }
};

#endif // !INPUT_MANAGER