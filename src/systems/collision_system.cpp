#include "collision_system.hpp"

void CollisionSystem::update(EntityManager& em)
{
    // Vector que contendrá las entidades y sus bounding boxes pa calcular sus colisiones luego
    std::vector<std::pair<Entity&, BBox>> boxes;
    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, PhysicsComponent& phy, RenderComponent& ren, ColliderComponent& col)
    {
        // Actualizar bounding box
        auto& pos = phy.position;
        auto& scl = ren.scale;
        col.updateBox(pos, scl);

        // Guardar para comprobar colisiones con otras entidades
        auto& b = col.boundingBox;
        boxes.push_back(std::make_pair(std::ref(e), b));

        // Comprobar colisiones con los bordes
        if (b.min.x() < -BORDER)
            pos.setX(-BORDER + scl.x() / 2);
        if (b.max.x() > BORDER)
            pos.setX(BORDER - scl.x() / 2);
        if (b.min.z() < -BORDER)
            pos.setZ(-BORDER + scl.z() / 2);
        if (b.max.z() > BORDER)
            pos.setZ(BORDER - scl.z() / 2);
    });

    checkCollision(em, boxes);
}

void CollisionSystem::checkCollision(EntityManager& em, std::vector<std::pair<Entity&, BBox>>& boxes)
{
    // Calculo de colisiones de BoundingBoxes
    for (size_t i = 0; i < boxes.size(); ++i)
    {
        for (size_t j = i + 1; j < boxes.size(); ++j)
        {
            if (CheckCollisionBoxes(boxes[i].second, boxes[j].second))
            {
                Entity& e = boxes[i].first;
                auto& pos = em.getComponent<PhysicsComponent>(e).position;

                Entity& e2 = boxes[j].first;
                auto& pos2 = em.getComponent<PhysicsComponent>(e2).position;

                auto& box1 = boxes[i].second;
                auto& box2 = boxes[j].second;

                vec3f overlap = box1.max - box2.min;
                vec3f overlap2 = box2.max - box1.min;

                vec3f minOverlap = vec3f::min(overlap, overlap2);

                if (minOverlap.x() < minOverlap.z())
                {
                    if (pos.x() < pos2.x())
                        pos.setX(pos.x() - minOverlap.x());
                    else
                        pos.setX(pos.x() + minOverlap.x());
                }
                else
                {
                    if (pos.z() < pos2.z())
                        pos.setZ(pos.z() - minOverlap.z());
                    else
                        pos.setZ(pos.z() + minOverlap.z());
                }
                // Se reduce la vida del player si colisiona con un enemigo
                if(e.hasTag<PlayerTag>() && e2.hasTag<EnemyTag>()){
                    auto& l{ em.getComponent<LifeComponent>(e) };
                    l.decreaseLife();
                }
            }
        }
    }
}

bool CollisionSystem::CheckCollisionBoxes(BBox box1, BBox box2)
{
    return !(box1.max < box2.min || box1.min > box2.max);
}
