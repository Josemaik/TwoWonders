#include "collision_system.hpp"
#include <chrono>

void CollisionSystem::update(EntityManager& em)
{
    auto& li = em.getSingleton<LevelInfo>();
    auto& frti = em.getSingleton<FrustumInfo>();

    // Liberar el octree
    octree.clear();
    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, PhysicsComponent& phy, ColliderComponent& col)
    {
        if ((!e.hasTags(FrustOut{}) && frti.bboxIn(col.bbox) == FrustPos::OUTSIDE) || e.hasTag<EnemyDeathTag>())
            return;

        // Si la entidad está por debajo del suelo, se destruye
        auto& pos = phy.position;
        if (pos.y() < -20.)
        {
            li.insertDeath(e.getID());
            return;
        }

        // Actualizar bounding box
        col.updateBox(pos, phy.scale, phy.gravity, phy.orientation, phy.rotationVec);

        // Insertar en el Octree
        octree.insert(e, col);
    });

    // Vector para saber qué colisiones se han calculado ya
    resetCollisionsMatrix();

    // Comprobar colisiones
    checkCollision(em, octree);

    // Comprobar colisiones con rampas
    handleRampCollision(em);

    // checkBorderCollision(em, ECPair);
}

// Función recursiva qué revisa las colisiones de las entidades del octree actual con otras entidades
void CollisionSystem::checkCollision(EntityManager& em, Octree& octree)
{
    // Si el octree está dividido, revisar sus hijos
    if (octree.isDivided())
    {
        for (auto& octant : octree.getOctants())
        {
            // Si el octante tiene entidades o está dividido, revisar sus colisiones
            if (octant.get()->getNumEntities() > 0 || octant.get()->isDivided())
                checkCollision(em, *octant);
        }
        return;
    }

    for (auto it1 = octree.getOctEntities().begin(); it1 != octree.getOctEntities().end(); ++it1)
    {
        for (auto it2 = std::next(it1); it2 != octree.getOctEntities().end(); ++it2)
        {
            auto [e, c] = *it1;
            auto [nEnt, nCol] = *it2;

            auto id1 = e->getID();
            auto id2 = nEnt->getID();

            if (id1 > id2)
                std::swap(id1, id2);

            if (!checkedPairs[id1][id2] && e != nEnt)
            {
                BBox& bbox1 = c->bbox;
                BBox& bbox2 = nCol->bbox;

                if (bbox1.intersects(bbox2))
                {
                    // Calculamos el mínimo solape entre las dos entidades
                    vec3d minOverlap = BBox::minOverlap(bbox1, bbox2);

                    // Manejamos la colisión
                    handleCollision(em, *e, *nEnt, minOverlap, c->behaviorType, nCol->behaviorType);

                    // Marcamos la colisión entre ambas entidades como comprobada
                    checkedPairs[id1][id2] = true;

                    auto& phy1 = em.getComponent<PhysicsComponent>(*e);
                    auto& phy2 = em.getComponent<PhysicsComponent>(*nEnt);

                    // Actualizamos las cajas de las entidades
                    c->updateBox(phy1.position, phy1.scale, phy1.gravity, phy1.orientation, phy1.rotationVec);
                    nCol->updateBox(phy2.position, phy2.scale, phy2.gravity, phy2.orientation, phy2.rotationVec);
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
    rampHeights.clear();

    for (auto [rID, eID] : checkedPairsRamp)
    {
        auto* rPtr = em.getEntityByID(rID);
        auto* ePtr = em.getEntityByID(eID);

        if (ePtr->hasComponent<RampComponent>())
            std::swap(rPtr, ePtr);

        auto& r = *rPtr;
        auto& e = *ePtr;

        auto& phy = em.getComponent<PhysicsComponent>(e);
        previousEntsOnRamp.push_back(&phy);
        auto& pos = phy.position;

        auto& ramp = em.getComponent<RampComponent>(r);
        auto& offSet = ramp.offset;

        phy.onRamp = true;
        // baseheight provisionalmente a 0
        double baseHeight = 0.0;
        double newHeight{};

        // switch (ramp.type)
        // {
        // case RampType::Normal:
        // {
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

        //     break;
        // }
        // case RampType::Triangular:
        // {
        //     // El punto objetivo se encuentra en offset.x, offset.z
        //     vec2d targetPoint = { offSet.x(), offSet.z() };

        //     // Calcula la distancia al punto objetivo
        //     double distanceToTarget = sqrt(pow(pos.x() - targetPoint.x, 2) + pow(pos.z() - targetPoint.y, 2));

        //     // Episilon para evitar divisiones por debajo de cierto valor
        //     double ep = 3;
        //     std::cout << distanceToTarget << std::endl;
        //     if (distanceToTarget < ep)
        //         distanceToTarget = ep;

        //     // Calcula la nueva altura basándose en la distancia al punto objetivo
        //     newHeight = baseHeight + (ramp.slope / distanceToTarget);
        //     std::cout << newHeight << std::endl;
        //     break;
        // }
        // default:
        //     break;
        // }

        // Nos aseguramos de que la nueva altura no sea menor que baseHeight
        newHeight = std::max(newHeight, baseHeight);
        newHeight += offSet.y() + phy.scale.y() / 2;

        // Ajustamos la posición en y con la nueva altura
        if (ramp.type == RampType::Normal)
            rampHeights.push_back({ ePtr->getID(), newHeight });
        else
            rampHeights.push_back({ ePtr->getID(), -newHeight });
    }

    if (rampHeights.size() > 0)
    {
        std::sort(rampHeights.begin(), rampHeights.end(), [](const auto& a, const auto& b) { return a.second < b.second; });

        for (auto& [eID, newHeight] : rampHeights)
        {
            auto* ePtr = em.getEntityByID(eID);
            auto& phy = em.getComponent<PhysicsComponent>(*ePtr);
            if (newHeight < 0)
                newHeight = -newHeight;

            phy.position.setY(newHeight);
        }
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
    if (behaviorType1 & BehaviorType::RAMP && behaviorType2 & BehaviorType::RAMP)
        return;
    if (behaviorType1 & BehaviorType::RAMP || behaviorType2 & BehaviorType::RAMP)
    {
        auto* staticEntPtr = &staticEnt;
        auto* otherEntPtr = &otherEnt;

        auto* staticPhyPtr = &staticPhy;
        auto* otherPhyPtr = &otherPhy;

        if (behaviorType2 & BehaviorType::RAMP)
        {
            std::swap(staticEntPtr, otherEntPtr);
            std::swap(staticPhyPtr, otherPhyPtr);
            std::swap(behaviorType1, behaviorType2);
        }

        // Entidades que no queremos que colisionen con rampas
        if (otherEntPtr->hasTag<WaterBombTag>() ||
            otherEntPtr->hasTag<FireBallTag>() ||
            otherEntPtr->hasTag<HitPlayerTag>() ||
            behaviorType2 & BehaviorType::AREADAMAGECRUSHER ||
            behaviorType2 & BehaviorType::AREADAMAGE)
            return;

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

        auto id1 = staticEntPtr->getID();
        auto id2 = otherEntPtr->getID();

        if (id1 > id2)
            std::swap(id1, id2);

        checkedPairsRamp.insert({ id1, id2 });
        return;
    }

    // LAVA
    if (behaviorType1 & BehaviorType::LAVA || behaviorType2 & BehaviorType::LAVA)
    {
        auto* staticEntPtr = &staticEnt;
        auto* otherEntPtr = &otherEnt;

        auto* staticPhyPtr = &staticPhy;
        auto* otherPhyPtr = &otherPhy;

        if (behaviorType2 & BehaviorType::LAVA)
        {
            std::swap(staticEntPtr, otherEntPtr);
            std::swap(staticPhyPtr, otherPhyPtr);
        }

        if (otherEntPtr->hasComponent<TypeComponent>())
        {
            auto& type = em.getComponent<TypeComponent>(*otherEntPtr);

            if (type.type != ElementalType::Fire && otherEntPtr->hasComponent<LifeComponent>())
            {
                auto& life = em.getComponent<LifeComponent>(*otherEntPtr);
                life.decreaseLife();

                resolvePlayerDirection(*otherPhyPtr, *staticPhyPtr, false);
            }
        }
        else if (otherEntPtr->hasTag<PlayerTag>() && otherEntPtr->hasComponent<LifeComponent>())
        {
            auto& life = em.getComponent<LifeComponent>(*otherEntPtr);
            life.decreaseLife();

            resolvePlayerDirection(*otherPhyPtr, *staticPhyPtr, false);
        }

        return;
    }

    // BOMBA DE CURACION
    if (behaviorType2 & BehaviorType::HEAL || behaviorType1 & BehaviorType::HEAL)
    {
        if (staticEnt.hasComponent<LifeComponent>() && staticEnt.hasTag<SlimeTag>()) {
            em.getComponent<LifeComponent>(staticEnt).increaseLife();
            return;
        }
        if (otherEnt.hasComponent<LifeComponent>() && otherEnt.hasTag<SlimeTag>()) {
            em.getComponent<LifeComponent>(otherEnt).increaseLife();
        }
        em.getSingleton<SoundSystem>().sonido_slime_curar();
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
        // Colisiones de enemigos con el jugador
        if (!(behaviorType1 & BehaviorType::SHIELD || behaviorType2 & BehaviorType::SHIELD))
        {
            handlePlayerCollision(em, staticEnt, otherEnt, staticPhy, otherPhy, minOverlap, behaviorType1, behaviorType2);
        }

        return;
    }

    // SPAWN
    if (behaviorType1 & BehaviorType::SPAWN || behaviorType2 & BehaviorType::SPAWN)
        return;

    if ((behaviorType2 & BehaviorType::SPIDERWEB || behaviorType1 & BehaviorType::SPIDERWEB) ||
        (behaviorType2 & BehaviorType::AREADAMAGECRUSHER || behaviorType1 & BehaviorType::AREADAMAGECRUSHER))
    {
        if (behaviorType2 & BehaviorType::SPIDERWEB || behaviorType2 & BehaviorType::AREADAMAGECRUSHER)
            std::swap(behaviorType1, behaviorType2);

        if (behaviorType2 & BehaviorType::ENEMY)
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
        || behaviorType2 & BehaviorType::AREADAMAGECRUSHER || behaviorType1 & BehaviorType::AREADAMAGECRUSHER
        || behaviorType2 & BehaviorType::OBJECT || behaviorType1 & BehaviorType::OBJECT
        || behaviorType2 & BehaviorType::LADDER || behaviorType1 & BehaviorType::LADDER)
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

    if (!staticEntPtr->hasComponent<ObjectComponent>() && behaviorType2 & BehaviorType::RAMP || behaviorType2 & BehaviorType::LADDER)
        return;

    if (staticEntPtr->hasTag<WallTag>() || otherEntPtr->hasTag<WallTag>())
    {
        if (otherEntPtr->hasTag<WallTag>())
        {
            std::swap(staticEntPtr, otherEntPtr);
            std::swap(staticPhy, otherPhy);
            std::swap(behaviorType1, behaviorType2);
        }

        if (otherEntPtr->hasTag<WallTag>() || otherEntPtr->hasTag<DestructibleTag>() || otherEntPtr->hasTag<CrusherTag>() || otherEntPtr->hasTag<DummyTag>() || staticEntPtr->hasTag<GroundTag>())
            return;
    }

    // Nos aseguramos que el suelo siempre esté en staticEntPtr
    if (otherEntPtr->hasTag<GroundTag>())
    {
        std::swap(staticPhy, otherPhy);
        std::swap(staticEntPtr, otherEntPtr);
        std::swap(behaviorType1, behaviorType2);

        if (otherEntPtr->hasTag<GroundTag>())
            return;

        if (otherEntPtr->hasTag<WallTag>())
            return;
    }

    // Colisiones con el suelo
    if (staticEntPtr->hasTag<GroundTag>() && !otherEntPtr->hasTag<HitPlayerTag>())
    {
        groundCollision(*otherPhy, *staticPhy, minOverlap);
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
                if (em.getComponent<DestructibleComponent>(*staticEntPtr).checkIfDamaged(bulletType.type)) {
                    em.getComponent<LifeComponent>(*staticEntPtr).decreaseLife();
                    if (staticEntPtr->hasTag<EnemyTag>()) {
                        //Detectar al player si  te impacta una bala
                        if (staticEntPtr->hasComponent<AIComponent>()) {
                            em.getComponent<AIComponent>(*staticEntPtr).playerdetected = true;
                        }
                    }
                }
            }
        }

        if (!otherEntPtr->hasComponent<ObjectComponent>())
            li.insertDeath(otherEntPtr->getID());

        return;
    }

    // Si impacta enemigo con pared
    if (behaviorType2 & BehaviorType::ENEMY)
    {
        enemiesWallCollision(em, *otherEntPtr, *staticPhy, *otherPhy, minOverlap);
        return;
    }

    if (behaviorType2 & BehaviorType::SPIDERWEB ||
        behaviorType2 & BehaviorType::WARNINGZONE ||
        behaviorType2 & BehaviorType::AREADAMAGECRUSHER ||
        behaviorType2 & BehaviorType::LADDER ||
        behaviorType2 & BehaviorType::LAVA ||
        behaviorType2 & BehaviorType::SPAWN ||
        behaviorType2 & BehaviorType::RAMP ||
        behaviorType2 & BehaviorType::OBJECT)
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
        if (ab.angrySoundOneTime)
        {
            ab.angrySound = true;
            ab.angrySoundOneTime = false;
        }
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

    if (behaviorType2 & BehaviorType::OBJECT)
    {
        // Colisión con un objeto
        em.getComponent<ObjectComponent>(*otherEntPtr).effect();
        return;
    }

    if (behaviorType2 & BehaviorType::ENEMY)
    {
        classicCollision(*staticPhy, *otherPhy, minOverlap);

        if (!otherEntPtr->hasTag<DummyTag>() && !otherEntPtr->hasTag<CrusherTag>())
        {
            resolvePlayerDirection(*staticPhy, *otherPhy, true);
            em.getSingleton<SoundSystem>().sonido_rebote();

            if (!otherEntPtr->hasTag<AngryBushTag>())
                return;
        }
        else if (otherEntPtr->hasTag<CrusherTag>() || otherEntPtr->hasTag<DummyTag>())
            return;

        if (otherEntPtr->hasTag<NoDamageTag>())
            return;

        enemyCollision(em, *staticEntPtr);
        return;
    }

    if (behaviorType2 & BehaviorType::SPAWN)
    {
        auto& plfi = em.getSingleton<PlayerInfo>();;
        auto& life = em.getComponent<LifeComponent>(*staticEntPtr);

        if (life.life < life.maxLife)
            life.life = life.maxLife;

        if (plfi.mana < plfi.max_mana - 3.0)
            plfi.mana = plfi.max_mana - 3.0;
        return;
    }

    // ESCALERA
    if (behaviorType2 & BehaviorType::LADDER)
    {
        auto& plfi = em.getSingleton<PlayerInfo>();
        if (plfi.onLadder)
        {
            auto& ldc = em.getComponent<LadderComponent>(*otherEntPtr);
            auto& col = em.getComponent<ColliderComponent>(*staticEntPtr);
            auto& box = col.bbox;

            if ((col.bbox.min.y() + 1) >= ldc.yMax || (col.bbox.min.y() - 1) <= ldc.yMin)
            {
                auto& phy = em.getComponent<PhysicsComponent>(*staticEntPtr);
                phy.gravity = 1.0;
                plfi.onLadder = false;

                vec3d newPos = phy.position;
                auto& ori = phy.orientation;

                // Si el jugador está por encima de la escalera, lo ponemos un poco más adelante de su posición
                if ((box.min.y() + 1) >= ldc.yMax)
                {
                    newPos.setX(newPos.x() + std::sin(ori) * 2.2);
                    newPos.setZ(newPos.z() + std::cos(ori) * 2.2);
                }
                else
                {
                    newPos.setX(newPos.x() - std::sin(ori) * 2.2);
                    newPos.setZ(newPos.z() - std::cos(ori) * 2.2);
                }

                phy.position = newPos;
            }
            return;

        }

        staticCollision(*staticPhy, *otherPhy, minOverlap);
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
            resolvePlayerDirection(*staticPhy, *otherPhy, false);
            em.getSingleton<SoundSystem>().sonido_rebote();
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

    // Colision con la meta
    if (behaviorType2 & BehaviorType::ENDING)
    {
        auto& li = em.getSingleton<LevelInfo>();
        li.currentScreen = GameScreen::ENDING;
        em.destroyComponent<AttackComponent>(*staticEntPtr);
        auto& lif = em.getComponent<LifeComponent>(*staticEntPtr);
        lif.maxLife = 7;
        lif.life = 7;
        return;
    }
}

void CollisionSystem::resolvePlayerDirection(PhysicsComponent& playerPhy, PhysicsComponent& enemyPhy, bool isEnemy)
{
    auto& pos = playerPhy.position;
    auto& otherPos = enemyPhy.position;
    auto& enemyVel = enemyPhy.velocity;
    vec3d dir = { pos.x() - otherPos.x(), 0, pos.z() - otherPos.z() };
    int multiplier = 7;

    dir.normalize();

    if (isEnemy)
    {
        // Si la dirección del enemigo y la del jugador tienen menos de 20 grados de diferencia, el jugador se le suman 45º en la dirección contraria
        vec3d dirEnemy = { otherPos.x() - enemyVel.x(), 0, otherPos.z() - enemyVel.z() };
        dirEnemy.normalize();

        // Calcular el ángulo entre las dos direcciones
        double dot = dir.x() * dirEnemy.x() + dir.z() * dirEnemy.z(); // producto punto
        double det = dir.x() * dirEnemy.z() - dir.z() * dirEnemy.x(); // determinante
        double angle = atan2(det, dot); // atan2(y, x) o atan2(sin, cos)

        // Convertir el ángulo a grados
        double angleDeg = angle * 180 / K_PI;

        // Si el ángulo es menor de 20 grados, ajustar la dirección del jugador
        if (std::abs(angleDeg) < 20)
        {
            // Rotar la dirección del jugador 45 grados en la dirección contraria
            double angleRad = -135 * K_PI / 180; // Convertir a radianes
            double cosAngle = cos(angleRad);
            double sinAngle = sin(angleRad);
            vec3d newDir = { dir.x() * cosAngle - dir.z() * sinAngle, 0.0, dir.x() * sinAngle + dir.z() * cosAngle };

            dir = newDir;
        }
        multiplier = 7;
    }
    else
        playerPhy.gravity = playerPhy.gravity / 1.5;

    playerPhy.velocity = dir * multiplier;
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
            std::swap(ent1Ptr, ent2Ptr);

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
                int damage = 2;

                if (balaCol.behaviorType & BehaviorType::ATK_PLAYER)
                {
                    //Si pegamos a un enemmigo nos detecta directamente
                    if (ent2Ptr->hasTag<GolemTag>()) {
                        em.getComponent<AIComponent>(*ent2Ptr).playerdetected = true;
                        // //empujar hacia atras si le impacta una bala
                        if (ent2Ptr->hasComponent<PhysicsComponent>() && (ent2Ptr->hasTag<SnowmanTag>()
                            || ent2Ptr->hasTag<GolemTag>())) {
                            auto& li = em.getSingleton<LevelInfo>();
                            auto& enpos = em.getComponent<PhysicsComponent>(*ent2Ptr).position;
                            auto& plphy = em.getComponent<PhysicsComponent>(*em.getEntityByID(li.playerID));
                            vec3d plorientation = vec3d(std::sin(plphy.orientation), 0.0, std::cos(plphy.orientation));
                            enpos.setX(enpos.x() + plorientation.x() * 4.0);
                            enpos.setZ(enpos.z() + plorientation.z() * 4.0);
                        }
                    }

                    auto& plfi = em.getSingleton<PlayerInfo>();
                    damage = plfi.currentSpell.damage;

                    if (ent1Ptr->hasComponent<ObjectComponent>())
                        damage = static_cast<int>(damage * 1.5);

                    if (damage == 0)
                        damage = 2;

                    if (plfi.attackUpgrade)
                        damage += 1;
                }

                if (li.invulnerable)
                    return;
                // Comprobar el tipo de la bala y el enemigo/player
                if ((typeBala == ElementalType::Fire && typeEnemyPlayer == ElementalType::Ice) ||
                    (typeBala == ElementalType::Ice && typeEnemyPlayer == ElementalType::Water) ||
                    (typeBala == ElementalType::Water && typeEnemyPlayer == ElementalType::Fire))
                {
                    li.decreaseLife(static_cast<int>(damage * 1.5));
                }
                else if (typeBala == ElementalType::Neutral) {
                    li.decreaseLife(damage);
                    // if (balaCol.attackType & AttackType::GollemAttack) {
                    //     em.getComponent<PhysicsComponent>(*ent2Ptr).dragActivatedTime = true;
                    // }
                }
                else
                {
                    li.decreaseLife(damage / 2);
                    // if(ent2Ptr->hasTag<GolemTag>()){
                    //     em.getComponent<PhysicsComponent>(*ent2Ptr).dragActivatedTime = true;
                    // }
                    if (balaCol.attackType & AttackType::GollemAttack) {
                        em.getComponent<PhysicsComponent>(*ent2Ptr).dragActivatedTime = true;
                    }
                }
            }
        }
    }
}

// Si chocamos contra el suelo, solo se desplaza en Y
void CollisionSystem::groundCollision(PhysicsComponent& playerPhysics, PhysicsComponent& groundPhy, vec3d& minOverlap)
{
    auto& pos1 = playerPhysics.position;
    auto& playerEsc = playerPhysics.scale;

    minOverlap += vec3d{ playerEsc.x() / 3.0, 0.f, playerEsc.z() / 3.0 };
    if ((minOverlap.y() < minOverlap.x() && minOverlap.y() < minOverlap.z()) || playerPhysics.onRamp)
    {
        pos1.setY(pos1.y() + minOverlap.y());
    }
    else if (minOverlap.x() < minOverlap.z())
    {
        resolveCollision<&vec3d::x, &vec3d::setX>(playerPhysics, groundPhy, minOverlap.x());
    }
    else
    {
        resolveCollision<&vec3d::z, &vec3d::setZ>(playerPhysics, groundPhy, minOverlap.z());
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

bool CollisionSystem::checkWallCollision(EntityManager& em, vec3d& pos, vec3d& normalDir)
{
    // Queremos saber si hay una pared en la dirección de la normal
    // Creamos un rayo que salga de la posición en la dirección de la normal
    RayCast ray{ pos, normalDir };

    // Buscamos todas las paredes cercanas a la posición
    using noCMPs = MP::TypeList<ColliderComponent>;
    using wallTag = MP::TypeList<>;
    bool collision{ false };
    vec3d colPoint{};

    auto& frti = em.getSingleton<FrustumInfo>();
    em.forEach<noCMPs, wallTag>([&](Entity& e, ColliderComponent& col)
    {
        if (frti.bboxIn(col.bbox) == FrustPos::OUTSIDE)
            return;
        if (col.behaviorType & BehaviorType::STATIC || col.behaviorType & BehaviorType::LAVA)
        {
            auto& bbox = col.bbox;
            if (e.hasTag<LavaTag>())
            {
                if (bbox.intersectsRay2(ray.origin, ray.direction, colPoint) && colPoint.distance(pos) < 23.0)
                {
                    auto& phy = em.getComponent<PhysicsComponent>(e);

                    // Pillamos el centro de la caja para asegurarnos que no se quede en la lava o atraviese paredes
                    auto centreDir = (vec3d{ phy.position.x(), pos.y(), phy.position.z() } - pos).normalize();
                    ray.direction = centreDir;

                    bbox.intersectsRay2(ray.origin, ray.direction, colPoint);
                    auto newPos = colPoint + centreDir * 2.0;

                    pos = newPos;

                    collision = true;
                    return;
                }
            }
            else if (bbox.intersectsRay(ray.origin, ray.direction, colPoint) && colPoint.distance(pos) < 10.0)
            {
                pos = colPoint - normalDir * 1.5;

                collision = true;
                return;
            }
        }
    });

    return collision;
}

void CollisionSystem::resetCollisionsMatrix()
{
    for (auto& row : checkedPairs)
        std::fill(row.begin(), row.end(), false);
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
