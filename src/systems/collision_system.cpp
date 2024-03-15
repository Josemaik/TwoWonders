#include "collision_system.hpp"
#include <raylib.h>
#include <chrono>

void CollisionSystem::update(EntityManager& em)
{
    auto& li = em.getSingleton<LevelInfo>();

    // Liberar el octree
    octree.clear();
    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, PhysicsComponent& phy, ColliderComponent& col)
    {
        // Si la entidad está por debajo del suelo, se destruye
        auto& pos = phy.position;
        if (pos.y() < -20.)
        {
            li.insertDeath(e.getID());
            return;
        }

        // Actualizar bounding box
        auto& scl = phy.scale;
        col.updateBox(pos, scl, phy.gravity, phy.orientation, phy.rotationVec);

        // Insertar en el Octree
        octree.insert(e, col);
    });

    // Vector para saber qué colisiones se han calculado ya
    checkedPairs.clear();

    // Comprobar colisiones
    checkCollision(em, octree, checkedPairs);

    // Comprobar colisiones con rampas
    handleRampCollision(em);

    // checkBorderCollision(em, ECPair);
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

void CollisionSystem::handleRampCollision(EntityManager& em)
{
    for (auto& phy : previousEntsOnRamp)
    {
        if (phy->onRamp)
            phy->onRamp = false;
    }

    previousEntsOnRamp.clear();

    for (auto [rID, eID] : checkedPairsRamp)
    {
        auto& r = *em.getEntityByID(rID);
        auto& e = *em.getEntityByID(eID);

        auto& phy = em.getComponent<PhysicsComponent>(e);
        previousEntsOnRamp.push_back(&phy);
        auto& pos = phy.position;

        auto& ramp = em.getComponent<RampComponent>(r);
        auto& offSet = ramp.offset;


        phy.onRamp = true;
        // baseheight provisionalmente a 0
        double baseHeight = 0.0;
        double newHeight{};

        switch (ramp.type)
        {
        case RampType::Normal:
        {
            // Calculamos la nueva altura dependiendo del slope
            newHeight = baseHeight + ramp.slope;

            // Utilizamos el offset para saber la dirección de la rampa,
            // si el offset en x es 0, la nueva altura se multiplica por la posición en z
            // 
            // Queremos que el offset siempre sea el contrario del punto donde empieza la rampa.
            // Por ejemplo, si la rampa te mueve hacia arriba por desde un 9x a un 14x,
            // el offset será -9x y el slope será positivo.
            // Si queremos invertir la rampa, el offset será -14x y el slope será negativo.
            if (offSet.x() == 0.0)
                newHeight *= (pos.z() + offSet.z());
            else
                newHeight *= (pos.x() + offSet.x());

            break;
        }
        case RampType::Triangular:
        {
            // El punto objetivo se encuentra en offset.x, offset.z
            vec2d targetPoint = { offSet.x(), offSet.z() };

            // Calcula la distancia al punto objetivo
            double distanceToTarget = sqrt(pow(pos.x() - targetPoint.x, 2) + pow(pos.z() - targetPoint.y, 2));

            // Episilon para evitar divisiones por debajo de cierto valor
            double ep = 3;
            if (distanceToTarget < ep)
                distanceToTarget = ep;

            // Calcula la nueva altura basándose en la distancia al punto objetivo
            newHeight = baseHeight + (ramp.slope / distanceToTarget);

            break;
        }
        default:
            break;
        }

        // Nos aseguramos de que la nueva altura no sea menor que baseHeight
        newHeight = std::max(newHeight, baseHeight);
        newHeight += offSet.y() + phy.scale.y() / 2;

        // Ajustamos la posición en y con la nueva altura
        phy.position.setY(newHeight);
    }

    checkedPairsRamp.clear();
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

    // Rampas
    if (behaviorType1 & BehaviorType::RAMP || behaviorType2 & BehaviorType::RAMP)
    {
        auto* staticEntPtr = &staticEnt;
        auto* otherEntPtr = &otherEnt;

        auto* staticPhyPtr = &em.getComponent<PhysicsComponent>(*staticEntPtr);
        auto* otherPhyPtr = &em.getComponent<PhysicsComponent>(*otherEntPtr);

        if (behaviorType2 & BehaviorType::RAMP)
        {
            std::swap(staticEntPtr, otherEntPtr);
            std::swap(staticPhyPtr, otherPhyPtr);
        }

        auto& offSet = em.getComponent<RampComponent>(*staticEntPtr).offset;
        auto& pos = otherPhyPtr->position;

        // Revisamos que no esté tratando de subir por los lados de la rampa
        if (pos.y() <= (offSet.y() + otherPhyPtr->scale.y() / 2))
        {
            if (offSet.x() == 0.0 && minOverlap.x() < minOverlap.z())
            {
                resolveCollision<&vec3d::x, &vec3d::setX>(*otherPhyPtr, *staticPhyPtr, minOverlap.x());
                return;
            }
            else if (offSet.z() == 0.0 && minOverlap.z() < minOverlap.x())
            {
                resolveCollision<&vec3d::z, &vec3d::setZ>(*otherPhyPtr, *staticPhyPtr, minOverlap.z());
                return;
            }
        }

        checkedPairsRamp.insert({ staticEntPtr->getID(), otherEntPtr->getID() });
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
            auto& balaCol = em.getComponent<ColliderComponent>(*staticEntPtr);
            if (balaCol.attackType & AttackType::Spiderweb)
                em.getComponent<AttackComponent>(*staticEntPtr).attack(AttackType::Spiderweb);

            auto& li = em.getSingleton<LevelInfo>();
            li.insertDeath(staticEntPtr->getID());
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

    if (behaviorType2 & BehaviorType::METEORITE || behaviorType1 & BehaviorType::METEORITE)
    {
        auto* meteoriteEntPtr = &staticEnt;
        auto* otherEntPtr = &otherEnt;

        if (behaviorType2 & BehaviorType::METEORITE)
            std::swap(meteoriteEntPtr, otherEntPtr);

        auto& li = em.getSingleton<LevelInfo>();
        li.insertDeath(meteoriteEntPtr->getID());

        return;
    }

    if (behaviorType2 & BehaviorType::WARNINGZONE || behaviorType1 & BehaviorType::WARNINGZONE
        || behaviorType2 & BehaviorType::AREADAMAGECRUSHER || behaviorType1 & BehaviorType::AREADAMAGECRUSHER)
        return;

    // Esto ya es cualquier colisión que no sea de player, paredes, zonas o ataques
    nonStaticCollision(staticPhy, otherPhy, minOverlap);
}

void CollisionSystem::handleStaticCollision(EntityManager& em, Entity& staticEnt, Entity& otherEnt, PhysicsComponent& statPhy, PhysicsComponent& othrPhy, vec3d& minOverlap, BehaviorType behaviorType1, BehaviorType behaviorType2)
{
    auto& li = em.getSingleton<LevelInfo>();

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
        return;

    if (!staticEntPtr->hasComponent<ObjectComponent>() && behaviorType2 & BehaviorType::RAMP)
        return;

    if (staticEntPtr->hasTag<WallTag>() || otherEntPtr->hasTag<WallTag>())
    {
        if (otherEntPtr->hasTag<WallTag>())
        {
            std::swap(staticEntPtr, otherEntPtr);
            std::swap(staticPhy, otherPhy);
            std::swap(behaviorType1, behaviorType2);
        }

        if (otherEntPtr->hasTag<WallTag>() || otherEntPtr->hasTag<DestructibleTag>() || otherEntPtr->hasTag<CrusherTag>() || staticEntPtr->hasTag<GroundTag>())
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
            // floorCollision(*staticPhy, *otherPhy, minOverlap);
            return;
        }

        if (otherEntPtr->hasTag<WallTag>())
            return;
    }

    // Colisiones con el suelo
    if (staticEntPtr->hasTag<GroundTag>() && !otherEntPtr->hasTag<HitPlayerTag>())
    {
        if (minOverlap.y() <= 1.0 || otherPhy->onRamp)
        {
            groundCollision(*otherPhy, otherPhy->scale, minOverlap);
            return;
        }
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
        if (staticEntPtr->hasTag<DestructibleTag>() && staticEntPtr->hasComponent<LifeComponent>())
        {
            if (otherEntPtr->hasComponent<TypeComponent>())
            {
                auto& bulletType = em.getComponent<TypeComponent>(*otherEntPtr);
                if (em.getComponent<DestructibleComponent>(*staticEntPtr).checkIfDamaged(bulletType.type))
                    em.getComponent<LifeComponent>(*staticEntPtr).decreaseLife();
            }
        }

        if (!otherEntPtr->hasComponent<ObjectComponent>())
            li.insertDeath(otherEntPtr->getID());

        return;
    }

    // Si impacta enemigo con pared
    if (behaviorType2 & BehaviorType::ENEMY)
    {
        if (staticEntPtr->hasTag<WaterTag>())
        {
            staticCollision(*otherPhy, *staticPhy, minOverlap);
            return;
        }

        enemiesWallCollision(em, *otherEntPtr, *staticPhy, *otherPhy, minOverlap);
        return;
    }

    if (behaviorType2 & BehaviorType::SPIDERWEB || behaviorType2 & BehaviorType::WARNINGZONE)
        return;

    if (behaviorType2 & BehaviorType::METEORITE)
    {
        li.insertDeath(otherEntPtr->getID());
        return;
    }

    // Colisiones con paredes
    staticCollision(*otherPhy, *staticPhy, minOverlap);
}

void CollisionSystem::enemiesWallCollision(EntityManager& em, Entity& ent, PhysicsComponent& staticPhy, PhysicsComponent& otherPhy, vec3d& minOverlap)
{
    // Determine which axis had the minimum overlap
    if (minOverlap.z() < minOverlap.x())
        resolveEnemyDirection(em, ent, staticPhy, otherPhy, minOverlap.z(), true);
    else if (minOverlap.x() < minOverlap.z())
        resolveEnemyDirection(em, ent, staticPhy, otherPhy, minOverlap.x(), false);

    if (ent.hasComponent<AngryBushComponent>())
    {
        auto& ab = em.getComponent<AngryBushComponent>(ent);
        ab.chargeAttack = true;
        ab.move = false;
    }
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

        if (otherEntPtr->hasTag<NoDamageTag>() && !otherEntPtr->hasTag<CrusherTag>())
        {
            resolvePlayerDirection(*staticPhy, *otherPhy);
            return;
        }
        else if (otherEntPtr->hasTag<CrusherTag>())
            return;

        enemyCollision(em, *staticEntPtr);
        return;
    }

    if (behaviorType2 & BehaviorType::SPAWN)
    {
        auto& plfi = em.getSingleton<PlayerInfo>();;
        if (evm != nullptr && plfi.spawnPoint != otherPhy->position)
        {
            evm->scheduleEvent(Event{ EventCodes::SetSpawn });

            plfi.spawnPoint = otherPhy->position;
            auto& life = em.getComponent<LifeComponent>(*staticEntPtr);
            life.life = life.maxLife;
            plfi.mana = plfi.max_mana - 3.0;
        }

        return;
    }

    // Daño en área
    if (behaviorType2 & BehaviorType::AREADAMAGE)
    {
        em.getComponent<LifeComponent>(*staticEntPtr).decreaseLife();
        return;
    }

    if (behaviorType2 & BehaviorType::AREADAMAGECRUSHER)
    {
        auto& bb = em.getSingleton<BlackBoard_t>();
        if (bb.playerdamagebycrusher == false) {
            em.getComponent<LifeComponent>(*staticEntPtr).decreaseLife(2);
            bb.playerdamagebycrusher = true;

            // El jugador se mueve hacia atrás de la posición del crusher
            resolvePlayerDirection(*staticPhy, *otherPhy);
        }
        return;
    }

    //Telaraña
    if (behaviorType2 & BehaviorType::SPIDERWEB)
    {
        em.getSingleton<BlackBoard_t>().playerhunted = true;
        staticPhy->dragActivated = true;
        return;
    }

    //Meteorit
    if (behaviorType2 & BehaviorType::METEORITE)
    {
        if (em.getEntityByID(staticEntPtr->getID())->hasTag<PlayerTag>()) {
            em.getComponent<LifeComponent>(*staticEntPtr).decreaseLife(2);
        }
        auto& li = em.getSingleton<LevelInfo>();
        li.insertDeath(otherEntPtr->getID());
        return;
    }
}

void CollisionSystem::resolvePlayerDirection(PhysicsComponent& playerPhy, PhysicsComponent& enemyPhy)
{
    auto& pos = playerPhy.position;
    auto& otherPos = enemyPhy.position;
    vec3d dir = { pos.x() - otherPos.x(), 0, pos.z() - otherPos.z() };
    dir.normalize();
    playerPhy.velocity = dir * 7;
    playerPhy.stopped = true;
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
            {
                if (!ent1Ptr->hasComponent<ObjectComponent>())
                {
                    auto& li = em.getSingleton<LevelInfo>();
                    li.insertDeath(ent1Ptr->getID());
                }
            }

            //Si la bala es lanzada por una araña no quita vida
            if (!balalaunchedbyspider)
            {
                auto& li = em.getComponent<LifeComponent>(*ent2Ptr);

                if (li.invulnerable)
                    return;
                // Comprobar el tipo de la bala y el enemigo/player
                if ((typeBala == ElementalType::Fire && typeEnemyPlayer == ElementalType::Ice) ||
                    (typeBala == ElementalType::Ice && typeEnemyPlayer == ElementalType::Water) ||
                    (typeBala == ElementalType::Water && typeEnemyPlayer == ElementalType::Fire))
                {
                    li.decreaseLife(3);
                }
                else if (typeBala == ElementalType::Neutral)
                    li.decreaseLife(2);
                else
                {
                    li.decreaseLife(1);
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
    // else if (minOverlap.y() > 2.0)
    // {
        // minOverlap -= vec3d{ playerEsc.x() / 2, 0.f, playerEsc.z() / 2 };
    // std::cout << "alooo\n";
    // else if (minOverlap.x() < minOverlap.z())
    // {
    //     pos1.setX(pos1.x() + minOverlap.x());
    // }
    // else
    // {
    //     pos1.setZ(pos1.z() + minOverlap.z());
    // }
    // }
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

bool CollisionSystem::checkWallCollision(EntityManager& em, vec3d& pos)
{
    // Buscamos todas las paredes cercanas a la posición
    using noCMPs = MP::TypeList<PhysicsComponent, ColliderComponent>;
    using wallTag = MP::TypeList<WallTag>;
    bool collision = false;

    // Creamos una caja de colisión de 1x1x1 centrada en la posición
    BBox box{ pos, {2, 4, 2} };

    em.forEach<noCMPs, wallTag>([&](Entity&, PhysicsComponent& phy, ColliderComponent& col)
    {
        if (collision)
            return;

        auto& bbox = col.boundingBox;
        if (bbox.intersects(box))
        {
            collision = true;

            // Calculamos el mínimo solape entre las dos entidades
            vec3d minOverlap = BBox::minOverlap(bbox, box);

            // Sacamos cuál sería la posición de la entidad después de la colisión
            //std::cout << pos << std::endl;
            auto newPos = pos;
            //std::cout << newPos << std::endl;
            if (minOverlap.x() < minOverlap.y() && minOverlap.x() < minOverlap.z())
            {
                if (phy.position.x() < pos.x())
                    newPos.setX(pos.x() + minOverlap.x());
                else
                    newPos.setX(pos.x() - minOverlap.x());
            }
            else if (minOverlap.z() < minOverlap.y())
            {
                if (phy.position.z() < pos.z())
                    newPos.setZ(pos.z() + minOverlap.z());
                else
                    newPos.setZ(pos.z() - minOverlap.z());
            }

            pos = newPos;
        }

    });

    return collision;
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
