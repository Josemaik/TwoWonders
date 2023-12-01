#include "collision_system.hpp"
#include "../managers/entity_manager.hpp"
#include <utility>

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

            // for (auto it1 = boxes.begin(); it1 != boxes.end(); ++it1)
            // {
            //     for (auto it2 = boxes.begin(); it2 != boxes.end(); ++it2)
            //     {
            //         if (it1 == it2) continue; // Skip checking collision with itself

            BBox* playerBBox = &it1->second->boundingBox;
            BBox* otherBBox = &it2->second->boundingBox;

            if (CheckCollisionBoxes(*playerBBox, *otherBBox))
            {
                Entity* player = it1->first;
                Entity* other = it2->first;

                ColliderComponent* playerCollider = it1->second;
                ColliderComponent* otherCollider = it2->second;

                vec3f overlap = playerBBox->max - otherBBox->min;
                vec3f overlap2 = otherBBox->max - playerBBox->min;

                vec3f minOverlap = vec3f::min(overlap, overlap2);

                if (otherCollider->behaviorType & BehaviorType::PLAYER)
                {
                    std::swap(player, other);
                    std::swap(playerBBox, otherBBox);
                    std::swap(playerCollider, otherCollider);

                    switch (otherCollider->behaviorType)
                    {
                    case BehaviorType::ENEMY:
                    {
                        auto& p = em.getComponent<PhysicsComponent>(*player);
                        enemyCollision(p, em.getComponent<PhysicsComponent>(*other), minOverlap);
                        auto& l = em.getComponent<LifeComponent>(*player);

                        if (l.life > 0 && p.invincibilityTimer <= 0) l.decreaseLife();
                        break;
                    }
                    case BehaviorType::STATIC:
                        staticCollision(em.getComponent<PhysicsComponent>(*player), em.getComponent<PhysicsComponent>(*other), minOverlap);
                        break;
                    case BehaviorType::PLAYER:
                        break;
                    case BehaviorType::NOTHING:
                        break;
                    }

                }
                else if (!(playerCollider->behaviorType & BehaviorType::PLAYER))
                {
                    nonPlayerCollision(em.getComponent<PhysicsComponent>(*player), em.getComponent<PhysicsComponent>(*other), minOverlap);
                }
            }
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

void CollisionSystem::nonPlayerCollision(PhysicsComponent& phy1, PhysicsComponent& phy2, vec3f& minOverlap)
{
    auto& pos1 = phy1.position;
    auto& pos2 = phy2.position;

    // Check the velocity of the entities
    if (phy1.velocity.length() <= phy2.velocity.length())
    {
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

    if (phy2.velocity.length() <= phy1.velocity.length())
    {
        if (minOverlap.x() < minOverlap.z())
        {
            if (pos2.x() < pos1.x())
                pos2.setX(pos2.x() - minOverlap.x());
            else
                pos2.setX(pos2.x() + minOverlap.x());
        }
        else
        {
            if (pos2.z() < pos1.z())
                pos2.setZ(pos2.z() - minOverlap.z());
            else
                pos2.setZ(pos2.z() + minOverlap.z());
        }
    }
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