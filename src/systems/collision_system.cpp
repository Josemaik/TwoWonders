#include "collision_system.hpp"
#include <raylib.h>

void CollisionSystem::update(EntityManager& em)
{
    // Octree que contendrá las entidades y sus colliders boxes pa calcular sus colisiones luego
    Octree octree(0, BBox(vec3f{ 0, 0, 0 }, vec3f{ 120, 30, 120 }));

    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, PhysicsComponent& phy, RenderComponent& ren, ColliderComponent& col)
    {
        // Actualizar bounding box
        auto& pos = phy.position;
        auto& scl = ren.scale;
        col.updateBox(pos, scl);

        // Insertar en el Octree
        octree.insert(e, col);
    });

    // Vector para saber qué colisiones se han calculado ya
    pairsType checkedPairs;

    checkCollision(em, octree, checkedPairs);
    // checkBorderCollision(em, ECPair);
}

// Función recursiva qué revisa las colisiones de las entidades del octree actual con otras entidades
void CollisionSystem::checkCollision(EntityManager& em, Octree& octree, pairsType& checkedPairs)
{
    // Si el octree está dividido, revisar sus hijos
    if (octree.divided)
    {
        for (auto& octant : octree.octants)
        {
            // Si el octante tiene entidades o está dividido, revisar sus colisiones
            if (octant.get()->entities.size() > 0 || octant.get()->divided)
                checkCollision(em, *octant, checkedPairs);
        }
        return;
    }

    // Auxiliar octree para guardar las entidades del mismo octante que el que se está comprobando
    Octree auxOct(octree.depth, octree.bounds, octree.parent, static_cast<uint8_t>(octree.entities.size()));

    // Check collisions between the entities in the sorted order
    for (auto it1 = octree.entities.begin(); it1 != octree.entities.end(); ++it1)
    {
        BBox& bbox1 = it1->second->boundingBox;

        // Guardamos las entidades del mismo octante en el auxiliar
        for (auto it2 = std::next(it1); it2 != octree.entities.end(); ++it2)
            auxOct.insert(*it2->first, *it2->second);


        // Revisamos las entidades de los octantes vecinos
        auto neighbors = octree.getNeighbors(*it1->second);
        neighbors.push_back(&auxOct);
        for (auto& neighbor : neighbors)
        {
            for (auto& neighborEntity : neighbor->entities)
            {
                // Si la colisión entre estas dos entidades no se ha comprobado ya, se hace ahora
                if (checkedPairs.find({ it1->first->getID(), neighborEntity.first->getID() }) == checkedPairs.end() &&
                    checkedPairs.find({ neighborEntity.first->getID(), it1->first->getID() }) == checkedPairs.end())
                {
                    BBox& bbox2 = neighborEntity.second->boundingBox;

                    if (bbox1.intersects(bbox2))
                    {
                        Entity& entity1 = *it1->first;
                        Entity& entity2 = *neighborEntity.first;

                        ColliderComponent& collider1 = *it1->second;
                        ColliderComponent& collider2 = *neighborEntity.second;

                        // Solo compruebo colisiones cuando no sean ENEMY ambas.
                        if (!(collider1.behaviorType & BehaviorType::ENEMY) || !(collider2.behaviorType & BehaviorType::ENEMY))
                        {
                            // Calculamos el mínimo solape entre las dos entidades
                            vec3f minOverlap = BBox::minOverlap(bbox1, bbox2);

                            handleCollision(em, entity1, entity2, minOverlap, collider1.behaviorType, collider2.behaviorType);
                        }

                        // Marcamos la colisión entre ambas entidades como comprobada
                        checkedPairs.insert({ it1->first->getID(), neighborEntity.first->getID() });
                        checkedPairs.insert({ neighborEntity.first->getID(), it1->first->getID() });
                    }
                }
            }
        }
    }
}

void CollisionSystem::handleCollision(EntityManager& em, Entity& staticEnt, Entity& otherEnt, vec3f& minOverlap, BehaviorType behaviorType1, BehaviorType behaviorType2)
{
    // Sacamos punteros de las físicas y de las entidades para poder hacer swaps
    auto* staticPhy = &em.getComponent<PhysicsComponent>(staticEnt);
    auto* otherPhy = &em.getComponent<PhysicsComponent>(otherEnt);

    auto* staticEntPtr = &staticEnt;
    auto* otherEntPtr = &otherEnt;

    if(staticEntPtr->hasTag<ZoneTag>() || otherEntPtr->hasTag<ZoneTag>()){
        if(otherEntPtr->hasTag<ZoneTag>()){
            std::swap(staticEntPtr, otherEntPtr);
            std::swap(staticPhy, otherPhy);
        }
        if(otherEntPtr->hasTag<PlayerTag>())
            em.getComponent<ZoneComponent>(*staticEntPtr).changeZone = true;
        
        return;
    }

    if (behaviorType1 & BehaviorType::STATIC || behaviorType2 & BehaviorType::STATIC)
    {
        // Comprobar si es un objeto estático - el objeto estático quedará en staticEnt y el otro en otherEnt
        if (behaviorType2 & BehaviorType::STATIC)
        {
            std::swap(staticPhy, otherPhy);
            std::swap(staticEntPtr, otherEntPtr);
            std::swap(behaviorType1, behaviorType2);
        }

        // Nos aseguramos que el suelo siempre esté en staticEntPtr
        if (otherEntPtr->hasTag<GroundTag>() || otherEntPtr->hasTag<WaterTag>())
        {
            std::swap(staticPhy, otherPhy);
            std::swap(staticEntPtr, otherEntPtr);
            std::swap(behaviorType1, behaviorType2);

            if (otherEntPtr->hasTag<GroundTag>() || otherEntPtr->hasTag<WaterTag>())
            {
                floorCollision(*staticPhy, *otherPhy, minOverlap);
                return;
            }
        }

        // Colisiones con el suelo
        if (staticEntPtr->hasTag<GroundTag>())
        {
            groundCollision(*otherPhy, em.getComponent<RenderComponent>(*otherEntPtr).scale, minOverlap);
            return;
        }

        // Si las paredes chocan entre ellas
        if (behaviorType1 & BehaviorType::STATIC && behaviorType2 & BehaviorType::STATIC)
        {
            floorCollision(*staticPhy, *otherPhy, minOverlap);
            return;
        }

        // Si el objeto estático es un objeto
        if (otherEntPtr->hasTag<PlayerTag>() && staticEntPtr->hasTag<ObjectTag>())
        {
            em.getComponent<ObjectComponent>(*staticEntPtr).effect();
            return;
        }

        // Si cualquiera de los impactos es con una bala, se baja la vida del otro
        if (behaviorType2 & BehaviorType::ATK_PLAYER || behaviorType2 & BehaviorType::ATK_ENEMY)
        {
            em.getComponent<LifeComponent>(*otherEntPtr).decreaseLife();
            return;
        }

        // Colisiones con paredes
        staticCollision(*otherPhy, *staticPhy, minOverlap);
        return;
    }

    bool isBulletPlayer1 = behaviorType1 & BehaviorType::ATK_PLAYER;
    bool isBulletPlayer2 = behaviorType2 & BehaviorType::ATK_PLAYER;
    bool isBulletEnemy1 = behaviorType1 & BehaviorType::ATK_ENEMY;
    bool isBulletEnemy2 = behaviorType2 & BehaviorType::ATK_ENEMY;

    if (isBulletPlayer1 || isBulletPlayer2 || isBulletEnemy1 || isBulletEnemy2)
    {
        bulletCollision(isBulletPlayer1, isBulletPlayer2, isBulletEnemy1, isBulletEnemy2, em, *staticEntPtr, *otherEntPtr);
        return;
    }

    if (behaviorType2 & BehaviorType::PLAYER || behaviorType1 & BehaviorType::PLAYER)
    {
        if (behaviorType2 & BehaviorType::PLAYER)
        {
            std::swap(staticEntPtr, otherEntPtr);
            std::swap(staticPhy, otherPhy);
        }
        classicCollision(*staticPhy, *otherPhy, minOverlap);
        enemyCollision(em, *staticEntPtr);
        return;
    }

    nonStaticCollision(*staticPhy, *otherPhy, minOverlap);
}

void CollisionSystem::bulletCollision(bool& bulletPl1, bool& bulletPl2, bool& bulletEn1, bool& bulletEn2, EntityManager& em, Entity& entity1, Entity& entity2)
{
    // Nos aseguramos que haya una bala que provenga de alguna de las dos entidades
    if (bulletPl1 || bulletPl2 || bulletEn1 || bulletEn2)
    {
        // Punteros a las entidades
        auto* ent1Ptr = &entity1;
        auto* ent2Ptr = &entity2;

        // Nos aseguramos que la bala esté en sea la de la entidad 1
        if (bulletPl2 || bulletEn2)
        {
            std::swap(ent1Ptr, ent2Ptr);
        }

        // Ahora sabemos seguro que la entidad con la que ha colisionado la bala está en la entidad 2, pero es enemigo o jugador?
        // Lo comprobamos con el tipo de comportamiento
        bool isPlayer2 = em.getComponent<ColliderComponent>(*ent2Ptr).behaviorType & BehaviorType::PLAYER;
        bool isEnemy2 = em.getComponent<ColliderComponent>(*ent2Ptr).behaviorType & BehaviorType::ENEMY;

        // Si la bala es del jugador y ha colisionado con un enemigo, o si la bala es de un enemigo y ha colisionado con el jugador, se baja la vida
        if ((isPlayer2 && (bulletEn1 || bulletEn2)) || (isEnemy2 && (bulletPl1 || bulletPl2)))
        {
            em.getComponent<LifeComponent>(*ent1Ptr).decreaseLife();
            em.getComponent<LifeComponent>(*ent2Ptr).decreaseLifeNextFrame = true;
        }
    }
}

// Si chocamos contra el suelo, solo se desplaza en Y
void CollisionSystem::groundCollision(PhysicsComponent& playerPhysics, vec3f& playerEsc, vec3f& minOverlap)
{
    auto& pos1 = playerPhysics.position;
    minOverlap += vec3f{ playerEsc.x() / 2, 0.f, playerEsc.z() / 2 };

    if (!playerPhysics.alreadyGrounded && minOverlap.y() < minOverlap.x() && minOverlap.y() < minOverlap.z())
    {
        pos1.setY(pos1.y() + minOverlap.y());
        playerPhysics.alreadyGrounded = true;
    }
}

// Si el suelo choca contra el suelo, solo se desplaza en X o Z (también se usa para paredes con paredes)
void CollisionSystem::floorCollision(PhysicsComponent& phy1, PhysicsComponent& phy2, vec3f& minOverlap)
{
    // auto& pos1 = phy1.position;
    // auto& pos2 = phy2.position;

    if (minOverlap.x() < minOverlap.y() && minOverlap.x() < minOverlap.z())
    {
        resolveCollisionX(phy1, phy2, minOverlap.x());
    }
    else if (minOverlap.z() < minOverlap.y())
    {
        resolveCollisionZ(phy1, phy2, minOverlap.z());
    }
}

// Efecto de cuando se choca con un enemigo
void CollisionSystem::enemyCollision(EntityManager& em, Entity& damagedEntity)
{
    em.getComponent<LifeComponent>(damagedEntity).decreaseLife();
}

// Efecto de cuando se choca contra una pared
void CollisionSystem::staticCollision(PhysicsComponent& playerPhysics, PhysicsComponent& staticPhysics, vec3f& minOverlap)
{
    classicCollision(playerPhysics, staticPhysics, minOverlap);
}

// Colisión entre entidades no estáticas, no hay diferencias definidas aún con las colisiones estáticas
void CollisionSystem::nonStaticCollision(PhysicsComponent& phy2, PhysicsComponent& phy1, vec3f& minOverlap)
{
    classicCollision(phy1, phy2, minOverlap);
}

// Colisión que desplaza una entidad en el eje que tenga el mínimo solape con quien haya chocado
void CollisionSystem::classicCollision(PhysicsComponent& phy1, PhysicsComponent& phy2, vec3f& minOverlap)
{
    // auto& pos1 = phy1.position;
    // auto& pos2 = phy2.position;

    // Si el solape en X es menor que el solape en Y y Z, desplazar en X, la misma lógica para el resto
    if (minOverlap.x() < minOverlap.y() && minOverlap.x() < minOverlap.z())
    {
        resolveCollisionX(phy1, phy2, minOverlap.x());
    }
    else if (minOverlap.z() < minOverlap.y())
    {
        resolveCollisionZ(phy1, phy2, minOverlap.z());
    }
    else
    {
        resolveCollisionY(phy1, phy2, minOverlap.y());
    }
}

// Desplazar la entidad en el eje X
void CollisionSystem::resolveCollisionX(PhysicsComponent& phy1, PhysicsComponent& phy2, float overlap)
{
    auto& pos1 = phy1.position;
    auto& pos2 = phy2.position;

    if (pos1.x() < pos2.x())
        pos1.setX(pos1.x() - overlap);
    else
        pos1.setX(pos1.x() + overlap);
}

// Desplazar la entidad en el eje Y
void CollisionSystem::resolveCollisionY(PhysicsComponent& phy1, PhysicsComponent& phy2, float overlap)
{
    auto& pos1 = phy1.position;
    auto& pos2 = phy2.position;

    if (pos1.y() < pos2.y())
        pos1.setY(pos1.y() - overlap);
    else
        pos1.setY(pos1.y() + overlap);
}

// Desplazar la entidad en el eje Z
void CollisionSystem::resolveCollisionZ(PhysicsComponent& phy1, PhysicsComponent& phy2, float overlap)
{
    auto& pos1 = phy1.position;
    auto& pos2 = phy2.position;

    if (pos1.z() < pos2.z())
        pos1.setZ(pos1.z() - overlap);
    else
        pos1.setZ(pos1.z() + overlap);
}

// Función para que no se salgan de los bordes, no se usa
// void CollisionSystem::checkBorderCollision(EntityManager&, Octree&)
// {
//     // Calculo de colisiones con los bordes
//     for (auto [e, c] : ECPair)
//     {
//         auto& b = c->boundingBox;
//         auto& scl = em.getComponent<RenderComponent>(*e).scale;
//         auto& pos = em.getComponent<PhysicsComponent>(*e).position;

//         // Comprobar colisiones con los bordes
//         if (b.min.x() < -BORDER)
//             pos.setX(-BORDER + scl.x() / 2);
//         if (b.max.x() > BORDER)
//             pos.setX(BORDER - scl.x() / 2);
//         if (b.min.z() < -BORDER)
//             pos.setZ(-BORDER + scl.z() / 2);
//         if (b.max.z() > BORDER)
//             pos.setZ(BORDER - scl.z() / 2);
//     }
// }