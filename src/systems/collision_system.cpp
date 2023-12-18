#include "collision_system.hpp"
#include <raylib.h>

void CollisionSystem::update(EntityManager& em)
{
    // Liberar el octree
    octree.clear();

    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, PhysicsComponent& phy, RenderComponent& ren, ColliderComponent& col)
    {
        // Actualizar bounding box
        auto& pos = phy.position;
        auto& scl = ren.scale;

        // if (phy.velocity != vec3f::zero())
        col.updateBox(pos, scl, phy.gravity);

        // Insertar en el Octree
        octree.insert(e, col);
    });

    // Vector para saber qué colisiones se han calculado ya
    checkedPairs.clear();

    // Comprobar colisiones
    checkCollision(em, octree, checkedPairs);

    // checkBorderCollision(em, ECPair);

    if (!dead_entities.empty())
    {
        em.destroyEntities(dead_entities);
        dead_entities.clear();
    }
}

// Función recursiva qué revisa las colisiones de las entidades del octree actual con otras entidades
void CollisionSystem::checkCollision(EntityManager& em, Octree& octree, pairsType& checkedPairs)
{
    // Si el octree está dividido, revisar sus hijos
    if (octree.isDivided())
    {
        for (auto& octant : octree.getOctants())
        {
            // Si el octante tiene entidades o está dividido, revisar sus colisiones
            if (octant.get()->getNumEntities() > 0 || octant.get()->isDivided())
                checkCollision(em, *octant, checkedPairs);
        }
        return;
    }

    for (auto it1 = octree.getOctEntities().begin(); it1 != octree.getOctEntities().end(); ++it1)
    {
        for (auto it2 = std::next(it1); it2 != octree.getOctEntities().end(); ++it2)
        {
            auto [e, c] = *it1;
            auto [nEnt, nCol] = *it2;

            if (checkedPairs.find({ e->getID(), nEnt->getID() }) == checkedPairs.end() && e != nEnt)
            {
                BBox& bbox1 = c->boundingBox;
                BBox& bbox2 = nCol->boundingBox;

                if (bbox1.intersects(bbox2))
                {
                    // Calculamos el mínimo solape entre las dos entidades
                    vec3f minOverlap = BBox::minOverlap(bbox1, bbox2);

                    // Manejamos la colisión
                    handleCollision(em, *e, *nEnt, minOverlap, c->behaviorType, nCol->behaviorType);

                    // Marcamos la colisión entre ambas entidades como comprobada
                    checkedPairs.insert({ e->getID(), nEnt->getID() });
                }
                else if (bbox1.min.y() < -10.f)
                {
                    dead_entities.insert(e->getID());
                }
            }
        }
    }
}

void CollisionSystem::handleCollision(EntityManager& em, Entity& staticEnt, Entity& otherEnt, vec3f& minOverlap, BehaviorType behaviorType1, BehaviorType behaviorType2)
{
    // Sacamos las físicas para pasarlas por parámetro swaps
    auto& staticPhy = em.getComponent<PhysicsComponent>(staticEnt);
    auto& otherPhy = em.getComponent<PhysicsComponent>(otherEnt);

    // Si el objeto es el suelo o una pared - importante que sea el primero porque es el tipo de colisión más común
    if (behaviorType1 & BehaviorType::STATIC || behaviorType2 & BehaviorType::STATIC)
    {
        handleStaticCollision(em, staticEnt, otherEnt, staticPhy, otherPhy, minOverlap, behaviorType1, behaviorType2);
        return;
    }

    // Segundo tipo de colisión más común, colisiones con zonas
    if ((behaviorType1 & BehaviorType::ZONE || behaviorType2 & BehaviorType::ZONE))
    {
        handleZoneCollision(em, staticEnt, otherEnt, staticPhy, otherPhy, behaviorType1, behaviorType2);
        return;
    }

    // Colisiones de enemigos con enemigos - creo que son más o menos comunes
    if (behaviorType1 & BehaviorType::ENEMY && behaviorType2 & BehaviorType::ENEMY)
    {
        return;
    }

    // Colisiones de balas con enemigos o jugadores
    bool isAtkPlayer1 = behaviorType1 & BehaviorType::ATK_PLAYER;
    bool isAtkPlayer2 = behaviorType2 & BehaviorType::ATK_PLAYER;
    bool isAtkEnemy1 = behaviorType1 & BehaviorType::ATK_ENEMY;
    bool isAtkEnemy2 = behaviorType2 & BehaviorType::ATK_ENEMY;


    if (isAtkPlayer1 || isAtkPlayer2 || isAtkEnemy1 || isAtkEnemy2)
    {
        if ((behaviorType2 & BehaviorType::SHIELD || behaviorType1 & BehaviorType::SHIELD)
            && (isAtkEnemy1 || isAtkEnemy2))
        {
            if (isAtkEnemy2)
            {
                std::swap(staticEnt, otherEnt);
                std::swap(staticPhy, otherPhy);
                std::swap(behaviorType1, behaviorType2);
            }

            em.getComponent<LifeComponent>(staticEnt).decreaseLife();
            return;
        }

        handleAtkCollision(em, isAtkPlayer1, isAtkPlayer2, isAtkEnemy1, isAtkEnemy2, staticEnt, otherEnt);
        return;
    }

    if (behaviorType1 & BehaviorType::PLAYER || behaviorType2 & BehaviorType::PLAYER)
    {
        // Colision con la meta
        if (behaviorType1 & BehaviorType::ENDING || behaviorType2 & BehaviorType::ENDING){
            auto& li = em.getSingleton<LevelInfo>();
            li.currentScreen = GameScreen::ENDING;
            //dead_entities.insert(li.playerID);
            return;
        }

        // Colisiones de enemigos con el jugador
        if (!(behaviorType1 & BehaviorType::SHIELD || behaviorType2 & BehaviorType::SHIELD))
            handlePlayerCollision(em, staticEnt, otherEnt, staticPhy, otherPhy, minOverlap, behaviorType1, behaviorType2);

        return;
    }

    // Esto ya es cualquier colisión que no sea de player, paredes, zonas o ataques
    nonStaticCollision(staticPhy, otherPhy, minOverlap);
}

void CollisionSystem::handleStaticCollision(EntityManager& em, Entity& staticEnt, Entity& otherEnt, PhysicsComponent& statPhy, PhysicsComponent& othrPhy, vec3f& minOverlap, BehaviorType behaviorType1, BehaviorType behaviorType2)
{
    // Sacamos punteros de las físicas y de las entidades para poder hacer swaps
    auto* staticPhy = &statPhy;
    auto* otherPhy = &othrPhy;

    auto* staticEntPtr = &staticEnt;
    auto* otherEntPtr = &otherEnt;

    // Comprobar si es un objeto estático - el objeto estático quedará en staticEnt y el otro en otherEnt
    if (behaviorType2 & BehaviorType::STATIC)
    {
        std::swap(staticPhy, otherPhy);
        std::swap(staticEntPtr, otherEntPtr);
        std::swap(behaviorType1, behaviorType2);
    }

    if (behaviorType2 & BehaviorType::ZONE)
    {
        return;
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

    // Si el suelo choca entre sí o las paredes chocan entre ellas
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
        if (!staticEntPtr->hasTag<WaterTag>())
            em.getComponent<LifeComponent>(*otherEntPtr).decreaseLife();
        return;
    }

    //Si impacta enemigo con pared
    if (behaviorType2 & BehaviorType::ENEMY)
    {
        if (staticEntPtr->hasTag<WaterTag>())
        {
            //groundCollision(*otherPhy, em.getComponent<RenderComponent>(*otherEntPtr).scale, minOverlap);
            staticCollision(*otherPhy, *staticPhy, minOverlap);
            return;
        }

        enemiesWallCollision(em, *otherEntPtr, *staticPhy, *otherPhy, minOverlap);
        return;
    }

    // Colisiones con paredes
    staticCollision(*otherPhy, *staticPhy, minOverlap);
}

void CollisionSystem::enemiesWallCollision(EntityManager& em, Entity& entity2, PhysicsComponent& staticPhy, PhysicsComponent& otherPhy, vec3f& minOverlap)
{
    if (entity2.hasComponent<RandomShootComponent>())
    {
        // Determine which axis had the minimum overlap
        if (minOverlap.z() < minOverlap.x())
            resolveEnemyDirection(em, entity2, staticPhy, otherPhy, minOverlap.z(), true);
        else if (minOverlap.x() < minOverlap.z())
            resolveEnemyDirection(em, entity2, staticPhy, otherPhy, minOverlap.x(), false);
    }
}

void CollisionSystem::resolveEnemyDirection(EntityManager& em, Entity& entity2, PhysicsComponent& staticPhy, PhysicsComponent& otherPhy, float overlap, bool isZAxis)
{
    auto& rands = em.getComponent<RandomShootComponent>(entity2);
    bool dir = isZAxis ? resolveCollision<&vec3f::z, &vec3f::setZ>(otherPhy, staticPhy, overlap)
        : resolveCollision<&vec3f::x, &vec3f::setX>(otherPhy, staticPhy, overlap);

    vec3f randir{};
    if (dir)    // Si es true, es derecha o arriba
        randir = rands.getRandomDirection(rands.right, rands.up, rands.left);
    else        // Si es false, es izquierda o abajo
        randir = rands.getRandomDirection(rands.right, rands.left, rands.down);

    otherPhy.velocity = randir;
}

void CollisionSystem::handleZoneCollision(EntityManager& em, Entity& staticEnt, Entity& otherEnt, PhysicsComponent& statPhy, PhysicsComponent& othrPhy, BehaviorType behaviorType1, BehaviorType behaviorType2)
{
    if (behaviorType1 & BehaviorType::PLAYER || behaviorType2 & BehaviorType::PLAYER)
    {
        // Sacamos punteros de las físicas y de las entidades para poder hacer swaps
        auto* staticPhy = &statPhy;
        auto* otherPhy = &othrPhy;

        auto* staticEntPtr = &staticEnt;
        auto* otherEntPtr = &otherEnt;

        if (behaviorType2 & BehaviorType::ZONE)
        {
            std::swap(staticEntPtr, otherEntPtr);
            std::swap(staticPhy, otherPhy);
        }

        em.getComponent<ZoneComponent>(*staticEntPtr).changeZone = true;
    }
}

void CollisionSystem::handlePlayerCollision(EntityManager& em, Entity& staticEnt, Entity& otherEnt, PhysicsComponent& statPhy, PhysicsComponent& othrPhy, vec3f& minOverlap, BehaviorType, BehaviorType behaviorType2)
{
    // Sacamos punteros de las físicas y de las entidades para poder hacer swaps
    auto* staticPhy = &statPhy;
    auto* otherPhy = &othrPhy;

    auto* staticEntPtr = &staticEnt;
    auto* otherEntPtr = &otherEnt;

    if (behaviorType2 & BehaviorType::PLAYER)
    {
        std::swap(staticEntPtr, otherEntPtr);
        std::swap(staticPhy, otherPhy);
    }
    classicCollision(*staticPhy, *otherPhy, minOverlap);
    enemyCollision(em, *staticEntPtr);
}

void CollisionSystem::handleAtkCollision(EntityManager& em, bool& atkPl1, bool& atkPl2, bool& atkEn1, bool& atkEn2, Entity& entity1, Entity& entity2)
{
    // Nos aseguramos que haya una bala que provenga de alguna de las dos entidades
    if (atkPl1 || atkPl2 || atkEn1 || atkEn2)
    {
        // Punteros a las entidades
        auto* ent1Ptr = &entity1;
        auto* ent2Ptr = &entity2;

        // Nos aseguramos que la bala esté en sea la de la entidad 1
        if (atkPl2 || atkEn2)
        {
            std::swap(ent1Ptr, ent2Ptr);
        }

        // Ahora sabemos seguro que la entidad con la que ha colisionado la bala está en la entidad 2, pero es enemigo o jugador?
        // Lo comprobamos con el tipo de comportamiento
        bool isPlayer2 = em.getComponent<ColliderComponent>(*ent2Ptr).behaviorType & BehaviorType::PLAYER;
        bool isEnemy2 = em.getComponent<ColliderComponent>(*ent2Ptr).behaviorType & BehaviorType::ENEMY;

        // Si la bala es del jugador y ha colisionado con un enemigo, o si la bala es de un enemigo y ha colisionado con el jugador, se baja la vida
        if ((isPlayer2 && (atkEn1 || atkEn2)) || (isEnemy2 && (atkPl1 || atkPl2)))
        {
            em.getComponent<LifeComponent>(*ent1Ptr).decreaseLife();
            em.getComponent<LifeComponent>(*ent2Ptr).decreaseLife();
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
    if (minOverlap.x() < minOverlap.y() && minOverlap.x() < minOverlap.z())
    {
        resolveCollision<&vec3f::x, &vec3f::setX>(phy1, phy2, minOverlap.x());
    }
    else if (minOverlap.z() < minOverlap.y())
    {
        resolveCollision<&vec3f::z, &vec3f::setZ>(phy1, phy2, minOverlap.z());
    }
}

// Efecto de cuando se choca con un enemigo
void CollisionSystem::enemyCollision(EntityManager& em, Entity& damagedEntity)
{
    em.getComponent<LifeComponent>(damagedEntity).decreaseLife();
}

// Efecto de cuando se choca contra una pared - podría expandirse para más usos en el futuro
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
    // Si el solape en X es menor que el solape en Y y Z, desplazar en X, la misma lógica para el resto
    if (minOverlap.x() < minOverlap.y() && minOverlap.x() < minOverlap.z())
    {
        resolveCollision<&vec3f::x, &vec3f::setX>(phy1, phy2, minOverlap.x());
    }
    else if (minOverlap.z() < minOverlap.y())
    {
        resolveCollision<&vec3f::z, &vec3f::setZ>(phy1, phy2, minOverlap.z());
    }
    else
    {
        // minOverlap += vec3f{ 0.f, phy2.gravity, 0.f };
        resolveCollision<&vec3f::y, &vec3f::setY>(phy1, phy2, minOverlap.y());
    }
}

template <auto getPos, auto setPos>
bool CollisionSystem::resolveCollision(PhysicsComponent& phy1, PhysicsComponent& phy2, float overlap)
{
    auto& pos1 = phy1.position;
    auto& pos2 = phy2.position;

    if ((pos1.*getPos)() < (pos2.*getPos)()) // derecha/abajo
    {
        (pos1.*setPos)((pos1.*getPos)() - overlap);
        return true;
    }
    else // izquierda/arriba
    {
        (pos1.*setPos)((pos1.*getPos)() + overlap);
        return false;
    }
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