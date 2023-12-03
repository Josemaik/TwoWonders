#include "collision_system.hpp"

void CollisionSystem::update(EntityManager& em)
{
    // Vector que contendrá las entidades y sus bounding boxes pa calcular sus colisiones luego
    EntColPair ECPair;
    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, PhysicsComponent& phy, RenderComponent& ren, ColliderComponent& col)
    {
        // Actualizar bounding box
        auto& pos = phy.position;
        auto& scl = ren.scale;
        col.updateBox(pos, scl);

        // Guardar para comprobar colisiones con otras entidades
        ECPair.insert({ &e, &col });
    });

    checkCollision(em, ECPair);
    // checkBorderCollision(em, ECPair);
}

void CollisionSystem::checkCollision(EntityManager& em, EntColPair& ECPair)
{
    // Calculo de colisiones de BoundingBoxes
    for (auto it1 = ECPair.begin(); it1 != ECPair.end(); ++it1)
    {
        auto it2 = it1;
        ++it2;
        for (; it2 != ECPair.end(); ++it2)
        {
            BBox* bbox1 = &it1->second->boundingBox;
            BBox* bbox2 = &it2->second->boundingBox;

            if (CheckCollisionBoxes(*bbox1, *bbox2))
            {
                Entity* entity1 = it1->first;
                Entity* entity2 = it2->first;

                ColliderComponent* collider1 = it1->second;
                ColliderComponent* collider2 = it2->second;

                vec3f overlap = bbox1->max - bbox2->min;
                vec3f overlap2 = bbox2->max - bbox1->min;

                vec3f minOverlap = vec3f::min(overlap, overlap2);

                handleCollision(em, entity1, entity2, minOverlap, collider1->behaviorType, collider2->behaviorType);
            }
        }
    }
}

void CollisionSystem::handleCollision(EntityManager& em, Entity* staticEnt, Entity* otherEnt, vec3f& minOverlap, BehaviorType behaviorType1, BehaviorType behaviorType2)
{
    auto* staticPhy = &em.getComponent<PhysicsComponent>(*staticEnt);
    auto* otherPhy = &em.getComponent<PhysicsComponent>(*otherEnt);

    if (behaviorType1 & BehaviorType::STATIC || behaviorType2 & BehaviorType::STATIC)
    {
        if (behaviorType2 & BehaviorType::STATIC)
        {
            std::swap(staticPhy, otherPhy);
            std::swap(staticEnt, otherEnt);
            std::swap(behaviorType1, behaviorType2);
        }

        if (otherEnt->hasTag<GroundTag>())
            std::swap(staticPhy, otherPhy);

        staticCollision(*otherPhy, *staticPhy, minOverlap);
        return;
    }

    if (behaviorType2 & BehaviorType::PLAYER || behaviorType1 & BehaviorType::PLAYER)
    {
        if (behaviorType2 & BehaviorType::PLAYER)
        {
            std::swap(staticEnt, otherEnt);
            std::swap(staticPhy, otherPhy);
        }
        enemyCollision(*staticPhy, *otherPhy, minOverlap);
        em.getComponent<LifeComponent>(*staticEnt).decreaseLife();
        return;
    }

    nonStaticCollision(*staticPhy, *otherPhy, minOverlap);

}

void CollisionSystem::enemyCollision(PhysicsComponent& playerPhysics, PhysicsComponent& enemyPhysics, vec3f& minOverlap)
{
    classicCollision(playerPhysics, enemyPhysics, minOverlap);
}

void CollisionSystem::staticCollision(PhysicsComponent& playerPhysics, PhysicsComponent& staticPhysics, vec3f& minOverlap)
{
    classicCollision(playerPhysics, staticPhysics, minOverlap);
}

void CollisionSystem::nonStaticCollision(PhysicsComponent& phy2, PhysicsComponent& phy1, vec3f& minOverlap)
{
    classicCollision(phy1, phy2, minOverlap);
}

void CollisionSystem::classicCollision(PhysicsComponent& phy1, PhysicsComponent& phy2, vec3f& minOverlap)
{
    auto& pos1 = phy1.position;
    auto& pos2 = phy2.position;

    if (minOverlap.x() < minOverlap.y() && minOverlap.x() < minOverlap.z())
    {
        if (pos1.x() < pos2.x())
            pos1.setX(pos1.x() - minOverlap.x());
        else
            pos1.setX(pos1.x() + minOverlap.x());
    }
    else if (minOverlap.y() < minOverlap.z())
    {
        if (pos1.y() < pos2.y())
            pos1.setY(pos1.y() - minOverlap.y());
        else
            pos1.setY(pos1.y() + minOverlap.y());
    }
    else
    {
        if (pos1.z() < pos2.z())
            pos1.setZ(pos1.z() - minOverlap.z());
        else
            pos1.setZ(pos1.z() + minOverlap.z());
    }
}


bool CollisionSystem::CheckCollisionBoxes(BBox box1, BBox box2)
{
    return !(box1.max < box2.min || box1.min > box2.max);
}

void CollisionSystem::checkBorderCollision(EntityManager& em, EntColPair& ECPair)
{
    // Calculo de colisiones con los bordes
    for (auto [e, c] : ECPair)
    {
        auto& b = c->boundingBox;
        auto& scl = em.getComponent<RenderComponent>(*e).scale;
        auto& pos = em.getComponent<PhysicsComponent>(*e).position;

        // Comprobar colisiones con los bordes
        if (b.min.x() < -BORDER)
            pos.setX(-BORDER + scl.x() / 2);
        if (b.max.x() > BORDER)
            pos.setX(BORDER - scl.x() / 2);
        if (b.min.z() < -BORDER)
            pos.setZ(-BORDER + scl.z() / 2);
        if (b.max.z() > BORDER)
            pos.setZ(BORDER - scl.z() / 2);
    }
}