#include "collision_system.hpp"
#include <raylib.h>
#include <chrono>

void CollisionSystem::update(EntityManager& em)
{
    // Liberar el octree
    octree.clear();
    std::vector<Entity*> EntsForRamps{};
    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, PhysicsComponent& phy, RenderComponent& ren, ColliderComponent& col)
    {
        // Si la entidad está por debajo del suelo, se destruye
        auto& pos = phy.position;
        if (pos.y() < -20.)
        {
            dead_entities.insert(e.getID());
            return;
        }

        // Actualizar bounding box
        auto& scl = ren.scale;
        col.updateBox(pos, scl, phy.gravity, phy.orientation);

        // Insertar en el Octree
        octree.insert(e, col);

        // Revisamos que no sean entidades estáticas
        if (!(col.behaviorType & BehaviorType::STATIC) && !(col.behaviorType & BehaviorType::ZONE))
            EntsForRamps.push_back(&e);
        else if (e.hasTag<ObjectTag>())
            EntsForRamps.push_back(&e);
    });

    // Vector para saber qué colisiones se han calculado ya
    checkedPairs.clear();

    // Comprobar colisiones
    checkCollision(em, octree, checkedPairs);

    // Comprobar colisiones con rampas
    checkRampCollision(em, EntsForRamps);

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
                    vec3d minOverlap = BBox::minOverlap(bbox1, bbox2);

                    // Manejamos la colisión
                    handleCollision(em, *e, *nEnt, minOverlap, c->behaviorType, nCol->behaviorType);

                    // Marcamos la colisión entre ambas entidades como comprobada
                    checkedPairs.insert({ e->getID(), nEnt->getID() });
                }
            }
        }
    }
}

void CollisionSystem::checkRampCollision(EntityManager& em, std::vector<Entity*>& Ents)
{
    for (auto e : Ents)
    {
        auto& col = em.getComponent<ColliderComponent>(*e);

        auto& bbox = col.boundingBox;
        auto pos = bbox.center();


        if (ramps.size() == 0)
        {
            using SYSCMPs = MP::TypeList<RampComponent>;
            using SYSTAGs = MP::TypeList<>;
            em.forEach<SYSCMPs, SYSTAGs>([&](Entity&, RampComponent& ramp)
            {
                ramps.push_back(&ramp);
            });
        }

        for (const auto rampP : ramps)
        {
            auto& ramp = *rampP;

            auto& min = ramp.min;
            auto& max = ramp.max;
            auto& offset = ramp.offset;

            if (pos.x() >= ramp.min.x && pos.x() <= max.x && pos.z() >= min.y && pos.z() <= max.y)
            {
                auto& ren = em.getComponent<RenderComponent>(*e);
                auto& phy = em.getComponent<PhysicsComponent>(*e);

                double baseHeight = ren.scale.y() / 2 - 0.5;
                double newHeight = 0.0;

                // Deltas para calcular la altura
                if (offset.x == 0.0)
                    newHeight = baseHeight + ramp.slope * (pos.z() + offset.y);
                else
                    newHeight = baseHeight + ramp.slope * (pos.x() + offset.x);

                phy.position.setY(newHeight);
                break;
            }
        }
    }
}

void CollisionSystem::handleCollision(EntityManager& em, Entity& staticEnt, Entity& otherEnt, vec3d& minOverlap, BehaviorType behaviorType1, BehaviorType behaviorType2)
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

    //Segundo tipo de colisión más común, colisiones con zonas
    if ((behaviorType1 & BehaviorType::ZONE || behaviorType2 & BehaviorType::ZONE))
    {
        handleZoneCollision(em, staticEnt, otherEnt, staticPhy, otherPhy, behaviorType1, behaviorType2);
        return;
    }

    // Colisiones de enemigos con enemigos - creo que son más o menos comunes
    if (behaviorType1 & BehaviorType::ENEMY && behaviorType2 & BehaviorType::ENEMY)
    {
        nonStaticCollision(staticPhy, otherPhy, minOverlap);
        return;
    }

    //BOMBA DE CURACION
    if (behaviorType2 & BehaviorType::HEAL || behaviorType1 & BehaviorType::HEAL)
    {
        if (staticEnt.hasComponent<LifeComponent>() && staticEnt.hasTag<SlimeTag>()) {
            em.getComponent<LifeComponent>(staticEnt).increaseLife();
            return;
        }
        if (otherEnt.hasComponent<LifeComponent>() && otherEnt.hasTag<SlimeTag>()) {
            em.getComponent<LifeComponent>(otherEnt).increaseLife();
        }
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
            && (isAtkEnemy1 || isAtkEnemy2) && behaviorType1 != BehaviorType::AREADAMAGE && behaviorType2 != BehaviorType::AREADAMAGE)
        {
            auto* staticEntPtr = &staticEnt;
            auto* otherEntPtr = &otherEnt;

            if (isAtkEnemy2)
                std::swap(staticEntPtr, otherEntPtr);

            // Si cualquiera de las dos entidades es una bala y es generada por una araña
            // creo una telaraña antes de borrar la bala
            if (staticEntPtr->hasComponent<ColliderComponent>())
            {
                auto& balaCol = em.getComponent<ColliderComponent>(*staticEntPtr);
                if (balaCol.attackType & AttackType::Spiderweb)
                {
                    em.getComponent<AttackComponent>(*staticEntPtr).attack(AttackType::Spiderweb);
                    return;
                }
            }

            if (otherEntPtr->hasComponent<ColliderComponent>())
            {
                auto& balaCol = em.getComponent<ColliderComponent>(*otherEntPtr);
                if (balaCol.attackType & AttackType::Spiderweb)
                {
                    em.getComponent<AttackComponent>(*otherEntPtr).attack(AttackType::Spiderweb);
                    return;
                }
            }

            dead_entities.insert(staticEntPtr->getID());
            return;
        }

        handleAtkCollision(em, isAtkPlayer1, isAtkPlayer2, isAtkEnemy1, isAtkEnemy2, staticEnt, otherEnt);
        return;
    }


    if (behaviorType1 & BehaviorType::PLAYER || behaviorType2 & BehaviorType::PLAYER)
    {
        // Colision con la meta
        if (behaviorType1 & BehaviorType::ENDING || behaviorType2 & BehaviorType::ENDING)
        {
            auto& li = em.getSingleton<LevelInfo>();
            li.currentScreen = GameScreen::ENDING;
            return;
        }

        // Colisiones de enemigos con el jugador
        if (!(behaviorType1 & BehaviorType::SHIELD || behaviorType2 & BehaviorType::SHIELD))
        {
            handlePlayerCollision(em, staticEnt, otherEnt, staticPhy, otherPhy, minOverlap, behaviorType1, behaviorType2);
        }

        return;
    }

    if (behaviorType2 & BehaviorType::SPIDERWEB || behaviorType1 & BehaviorType::SPIDERWEB)
    {
        if (behaviorType2 & BehaviorType::SPIDERWEB)
            std::swap(behaviorType1, behaviorType2);

        if (behaviorType2 & BehaviorType::ENEMY || behaviorType2 & BehaviorType::SHIELD)
            return;
    }

    // Esto ya es cualquier colisión que no sea de player, paredes, zonas o ataques
    //Si una de las entidades es una telaraña no se comprueba las colisiones
    if(!(behaviorType1 & BehaviorType::SPIDERWEB) && !(behaviorType2 & BehaviorType::SPIDERWEB)){
        nonStaticCollision(staticPhy, otherPhy, minOverlap);
    }
}

void CollisionSystem::handleStaticCollision(EntityManager& em, Entity& staticEnt, Entity& otherEnt, PhysicsComponent& statPhy, PhysicsComponent& othrPhy, vec3d& minOverlap, BehaviorType behaviorType1, BehaviorType behaviorType2)
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

    // Si el objeto estático es un objeto
    if (otherEntPtr->hasTag<PlayerTag>() && staticEntPtr->hasTag<ObjectTag>())
    {
        em.getComponent<ObjectComponent>(*staticEntPtr).effect();
        return;
    }
    // Si el objeto estático es una escalera
    // if(staticEntPtr->hasTag<StairTag>() && otherEntPtr->hasTag<PlayerTag>()){
    //    std::cout << "COLISION CON ESCALERA \n";
    //     em.getComponent<PhysicsComponent>(*otherEntPtr).blockXZ = true;
    //     em.getComponent<PhysicsComponent>(*otherEntPtr).gravity = 0.0;
    //     return;
    // }


    // Si cualquiera de los impactos es con una bala, se baja la vida del otro
    if (behaviorType2 & BehaviorType::ATK_PLAYER || behaviorType2 & BehaviorType::ATK_ENEMY)
    {
        if (!staticEntPtr->hasTag<WaterTag>())
            dead_entities.insert(otherEntPtr->getID());

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

    if (staticEntPtr->hasTag<DoorTag>() && behaviorType2 & BehaviorType::PLAYER)
    {
        auto& ic = em.getComponent<InformationComponent>(*otherEntPtr);
        if (ic.hasKey)
        {
            dead_entities.insert(staticEntPtr->getID());

            ic.hasKey = false;
            return;
        }
    }
    // Colisiones con paredes
    staticCollision(*otherPhy, *staticPhy, minOverlap);
}

void CollisionSystem::enemiesWallCollision(EntityManager& em, Entity& entity2, PhysicsComponent& staticPhy, PhysicsComponent& otherPhy, vec3d& minOverlap)
{
    // Determine which axis had the minimum overlap
    if (minOverlap.z() < minOverlap.x())
        resolveEnemyDirection(em, entity2, staticPhy, otherPhy, minOverlap.z(), true);
    else if (minOverlap.x() < minOverlap.z())
        resolveEnemyDirection(em, entity2, staticPhy, otherPhy, minOverlap.x(), false);
}

void CollisionSystem::resolveEnemyDirection(EntityManager&, Entity&, PhysicsComponent& staticPhy, PhysicsComponent& otherPhy, double overlap, bool isZAxis)
{
    [[maybe_unused]] bool dir = isZAxis ? resolveCollision<&vec3d::z, &vec3d::setZ>(otherPhy, staticPhy, overlap)
        : resolveCollision<&vec3d::x, &vec3d::setX>(otherPhy, staticPhy, overlap);

    // vec3d randir{};
    // if (dir)    // Si es true, es derecha o arriba
    //     randir = rands.getRandomDirection(rands.right, rands.up, rands.left);
    // else        // Si es false, es izquierda o abajo
    //     randir = rands.getRandomDirection(rands.right, rands.left, rands.down);

    // otherPhy.velocity = randir;
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

void CollisionSystem::handlePlayerCollision(EntityManager& em, Entity& staticEnt, Entity& otherEnt, PhysicsComponent& statPhy, PhysicsComponent& othrPhy, vec3d& minOverlap, BehaviorType behaviorType1, BehaviorType behaviorType2)
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
        std::swap(behaviorType1, behaviorType2);
    }

    if (behaviorType2 & BehaviorType::ENEMY)
    {
        classicCollision(*staticPhy, *otherPhy, minOverlap);
        enemyCollision(em, *staticEntPtr);
        return;
    }

    // Daño en área
    if (behaviorType2 & BehaviorType::AREADAMAGE)
    {
        em.getComponent<LifeComponent>(*staticEntPtr).decreaseLife();
        return;
    }

    //Telaraña
    if (behaviorType2 & BehaviorType::SPIDERWEB)
    {
        em.getSingleton<BlackBoard_t>().playerhunted = true;
        staticPhy->dragActivated = true;
        return;
    }
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
        // Lo comprobamos con el tipo de comportamientoto
        auto& c = em.getComponent<ColliderComponent>(*ent2Ptr);
        auto& balaCol = em.getComponent<ColliderComponent>(*ent1Ptr);
        bool isPlayer2 = c.behaviorType & BehaviorType::PLAYER;
        bool isEnemy2 = c.behaviorType & BehaviorType::ENEMY;

        // Si la bala es del jugador y ha colisionado con un enemigo, o si la bala es de un enemigo y ha colisionado con el jugador, se baja la vida
        if ((isPlayer2 && (atkEn1 || atkEn2)) || (isEnemy2 && (atkPl1 || atkPl2)))
        {
            // Comprobar si tiene tipo la bala
            ElementalType typeBala{ ElementalType::Neutral }, typeEnemyPlayer{ ElementalType::Neutral };
            if (ent1Ptr->hasComponent<TypeComponent>())
                typeBala = em.getComponent<TypeComponent>(*ent1Ptr).type;
            if (ent2Ptr->hasComponent<TypeComponent>())
                typeEnemyPlayer = em.getComponent<TypeComponent>(*ent2Ptr).type;

            // Destruir bala
            // si es dispara por una araña creo un ataque de telaraña antes de matarla
            bool balalaunchedbyspider{ false };
            if (balaCol.attackType & AttackType::Spiderweb)
            {
                em.getComponent<AttackComponent>(*ent1Ptr).attack(AttackType::Spiderweb);
                balalaunchedbyspider = true;
            }
            else
                dead_entities.insert(ent1Ptr->getID());

            //Si la bala es lanzada por una araña no quita vida
            if (!balalaunchedbyspider)
            {
                // Comprobar el tipo de la bala y el enemigo/player
                if ((typeBala == ElementalType::Fuego && typeEnemyPlayer == ElementalType::Hielo) ||
                    (typeBala == ElementalType::Hielo && typeEnemyPlayer == ElementalType::Agua) ||
                    (typeBala == ElementalType::Agua && typeEnemyPlayer == ElementalType::Fuego))
                {
                    em.getComponent<LifeComponent>(*ent2Ptr).decreaseLife(3);
                }
                else if (typeBala == ElementalType::Neutral)
                    em.getComponent<LifeComponent>(*ent2Ptr).decreaseLife(2);
                else
                {
                    em.getComponent<LifeComponent>(*ent2Ptr).decreaseLife(1);
                    if (balaCol.attackType & AttackType::GollemAttack) {
                        em.getComponent<PhysicsComponent>(*ent2Ptr).dragActivatedTime = true;
                    }
                }
            }
        }
    }
}

// Si chocamos contra el suelo, solo se desplaza en Y
void CollisionSystem::groundCollision(PhysicsComponent& playerPhysics, vec3d& playerEsc, vec3d& minOverlap)
{
    auto& pos1 = playerPhysics.position;
    minOverlap += vec3d{ playerEsc.x() / 2, 0.f, playerEsc.z() / 2 };

    if (!playerPhysics.alreadyGrounded && minOverlap.y() < minOverlap.x() && minOverlap.y() < minOverlap.z())
    {
        pos1.setY(pos1.y() + minOverlap.y());
        playerPhysics.alreadyGrounded = true;
    }
}

// Si el suelo choca contra el suelo, solo se desplaza en X o Z (también se usa para paredes con paredes)
void CollisionSystem::floorCollision(PhysicsComponent& phy1, PhysicsComponent& phy2, vec3d& minOverlap)
{
    if (minOverlap.x() < minOverlap.y() && minOverlap.x() < minOverlap.z())
    {
        resolveCollision<&vec3d::x, &vec3d::setX>(phy1, phy2, minOverlap.x());
    }
    else if (minOverlap.z() < minOverlap.y())
    {
        resolveCollision<&vec3d::z, &vec3d::setZ>(phy1, phy2, minOverlap.z());
    }
}

// Efecto de cuando se choca con un enemigo
void CollisionSystem::enemyCollision(EntityManager& em, Entity& damagedEntity)
{
    em.getComponent<LifeComponent>(damagedEntity).decreaseLife(1);
}

// Efecto de cuando se choca contra una pared - podría expandirse para más usos en el futuro
void CollisionSystem::staticCollision(PhysicsComponent& playerPhysics, PhysicsComponent& staticPhysics, vec3d& minOverlap)
{
    classicCollision(playerPhysics, staticPhysics, minOverlap);
}

// Colisión entre entidades no estáticas, no hay diferencias definidas aún con las colisiones estáticas
void CollisionSystem::nonStaticCollision(PhysicsComponent& phy2, PhysicsComponent& phy1, vec3d& minOverlap)
{
    classicCollision(phy1, phy2, minOverlap);
}

// Colisión que desplaza una entidad en el eje que tenga el mínimo solape con quien haya chocado
void CollisionSystem::classicCollision(PhysicsComponent& phy1, PhysicsComponent& phy2, vec3d& minOverlap)
{
    // Si el solape en X es menor que el solape en Y y Z, desplazar en X, la misma lógica para el resto
    if (minOverlap.x() < minOverlap.y() && minOverlap.x() < minOverlap.z())
    {
        resolveCollision<&vec3d::x, &vec3d::setX>(phy1, phy2, minOverlap.x());
    }
    else if (minOverlap.z() < minOverlap.y())
    {
        resolveCollision<&vec3d::z, &vec3d::setZ>(phy1, phy2, minOverlap.z());
    }
    else
    {
        // minOverlap += vec3f{ 0.f, phy2.gravity, 0.f };
        resolveCollision<&vec3d::y, &vec3d::setY>(phy1, phy2, minOverlap.y());
    }
}

template <auto getPos, auto setPos>
bool CollisionSystem::resolveCollision(PhysicsComponent& phy1, PhysicsComponent& phy2, double overlap)
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
