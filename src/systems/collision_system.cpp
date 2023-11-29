#include "collision_system.hpp"

void CollisionSystem::update(EntityManager& em)
{
    // Vector que contendrá las entidades y sus bounding boxes pa calcular sus colisiones luego
    std::vector<std::pair<Entity&, BoundingBox>> boxes;
    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, PhysicsComponent& phy, RenderComponent& ren)
    {
        BoundingBox b = phy.getBoundingBox(ren.scale);
        boxes.push_back(std::make_pair(std::ref(e), b));

        if (b.min.x < -BORDER)
            phy.position.x = -BORDER + ren.scale.x / 2;
        if (b.max.x > BORDER)
            phy.position.x = BORDER - ren.scale.x / 2;
        if (b.min.z < -BORDER)
            phy.position.z = -BORDER + ren.scale.z / 2;
        if (b.max.z > BORDER)
            phy.position.z = BORDER - ren.scale.z / 2;
    });

    // Calculo de colisiones de BoundingBoxes
    for (size_t i = 0; i < boxes.size(); ++i)
    {
        for (size_t j = i + 1; j < boxes.size(); ++j)
        {
            if (CheckCollisionBoxes(boxes[i].second, boxes[j].second))
            {
                Entity& e = boxes[i].first;
                auto& phy = em.getComponent<PhysicsComponent>(e);

                Entity& e2 = boxes[j].first;
                auto& phy2 = em.getComponent<PhysicsComponent>(e2);

                auto& box1 = boxes[i].second;
                auto& box2 = boxes[j].second;

                Vector3 overlap = { box1.max.x - box2.min.x, box1.max.y - box2.min.y, box1.max.z - box2.min.z };
                Vector3 overlap2 = { box2.max.x - box1.min.x, box2.max.y - box1.min.y, box2.max.z - box1.min.z };

                Vector3 minOverlap = { std::min(overlap.x, overlap2.x), std::min(overlap.y, overlap2.y), std::min(overlap.z, overlap2.z) };

                if (minOverlap.x < minOverlap.z)
                {
                    if (phy.position.x < phy2.position.x)
                        phy.position.x -= minOverlap.x;
                    else
                        phy.position.x += minOverlap.x;
                }
                else
                {
                    if (phy.position.z < phy2.position.z)
                        phy.position.z -= minOverlap.z;
                    else
                        phy.position.z += minOverlap.z;
                }
            }
        }
    }
}