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
    checkBorderCollision(em, ECPair);
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

                if (collider2->behaviorType & BehaviorType::PLAYER)
                {
                    std::swap(entity1, entity2);
                    std::swap(bbox1, bbox2);
                    std::swap(collider1, collider2);
                }

                handleCollision(em, entity1, entity2, minOverlap, collider1->behaviorType, collider2->behaviorType);
            }
        }
    }
}

void CollisionSystem::handleCollision(EntityManager& em, Entity* entity1, Entity* entity2, vec3f& minOverlap, BehaviorType behaviorType1, BehaviorType behaviorType2)
{
    auto& physics1 = em.getComponent<PhysicsComponent>(*entity1);
    auto& physics2 = em.getComponent<PhysicsComponent>(*entity2);

    if (behaviorType1 & BehaviorType::PLAYER)
    {
        switch (behaviorType2)
        {
        case BehaviorType::ENEMY:
        {
            enemyCollision(physics1, physics2, minOverlap);
            auto& life = em.getComponent<LifeComponent>(*entity1);
            if (life.life > 0 && physics1.invincibilityTimer <= 0) life.decreaseLife();
            break;
        }
        case BehaviorType::STATIC:
            staticCollision(physics1, physics2, minOverlap);
            break;
        default:
            break;
        }
    }
    else if (behaviorType1 & BehaviorType::ENEMY)
    {
        if (behaviorType2 & BehaviorType::STATIC)
        {
            // Handle the case when one entity is an enemy and the other is static
            staticCollision(physics1, physics2, minOverlap);
        }
        else if (behaviorType2 & BehaviorType::ENEMY)
        {
            // Handle the case when both entities are enemies
            nonPlayerCollision(physics1, physics2, minOverlap);
        }
    }
    else if (behaviorType1 & BehaviorType::STATIC)
    {
        if (behaviorType2 & BehaviorType::STATIC)
        {
            staticCollision(physics1, physics2, minOverlap);
        }
        else if (behaviorType2 & BehaviorType::ENEMY)
        {
            staticCollision(physics1, physics2, minOverlap);
        }
    }
}

void CollisionSystem::enemyCollision(PhysicsComponent& playerPhysics, PhysicsComponent& enemyPhysics, vec3f& minOverlap)
{
    classicCollision(playerPhysics, enemyPhysics, minOverlap);

    // Inmovilizar al jugador si toca a un enemigo
    if (playerPhysics.invincibilityTimer <= 0.f && playerPhysics.immobilizeTimer <= 0.f)
        playerPhysics.immobilizeTimer = 30.0f;
}

void CollisionSystem::staticCollision(PhysicsComponent& playerPhysics, PhysicsComponent& staticPhysics, vec3f& minOverlap)
{
    classicCollision(playerPhysics, staticPhysics, minOverlap);
}

void CollisionSystem::nonPlayerCollision(PhysicsComponent& phy2, PhysicsComponent& phy1, vec3f& minOverlap)
{
    auto& pos1 = phy1.position;
    auto& pos2 = phy2.position;

    classicCollision(phy1, phy2, minOverlap);

    // // Check the velocity of the entities
    // if (phy1.velocity.length() <= phy2.velocity.length())
    // {
    //     if (minOverlap.x() < minOverlap.z())
    //     {
    //         if (pos1.x() < pos2.x())
    //             pos1.setX(pos1.x() - minOverlap.x());
    //         else
    //             pos1.setX(pos1.x() + minOverlap.x());
    //     }
    //     else
    //     {
    //         if (pos1.z() < pos2.z())
    //             pos1.setZ(pos1.z() - minOverlap.z());
    //         else
    //             pos1.setZ(pos1.z() + minOverlap.z());
    //     }
    // }

    // if (phy2.velocity.length() <= phy1.velocity.length())
    // {
    //     if (minOverlap.x() < minOverlap.z())
    //     {
    //         if (pos2.x() < pos1.x())
    //             pos2.setX(pos2.x() - minOverlap.x());
    //         else
    //             pos2.setX(pos2.x() + minOverlap.x());
    //     }
    //     else
    //     {
    //         if (pos2.z() < pos1.z())
    //             pos2.setZ(pos2.z() - minOverlap.z());
    //         else
    //             pos2.setZ(pos2.z() + minOverlap.z());
    //     }
    // }
}

void CollisionSystem::classicCollision(PhysicsComponent& phy1, PhysicsComponent& phy2, vec3f& minOverlap)
{
    auto& pos1 = phy1.position;
    auto& pos2 = phy2.position;

    if (minOverlap.x() < minOverlap.z())
    {
        if (pos1.x() < pos2.x())
            pos1.setX(pos1.x() - minOverlap.x());
        else
            pos1.setX(pos1.x() + minOverlap.x());
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