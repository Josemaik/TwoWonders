#include "life_system.hpp"

void LifeSystem::update(EntityManager& em, ObjectSystem& os) {
    auto& li = em.getSingleton<LevelInfo>();

    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& ent, LifeComponent& lif)
    {
        if (ent.hasTag<EnemyDeathTag>())
            return;
        if (lif.lifeLost > 0)
        {
            // Si es un subdito y tiene escudo activo, se le resta al escudo
            if (ent.hasTag<SubjectTag>() && ent.hasComponent<SubjectComponent>())
            {
                auto& sub = em.getComponent<SubjectComponent>(ent);
                if (sub.activeShield)
                {
                    lif.life += lif.lifeLost;
                    sub.decreaseShield();

                    if (sub.shieldLife == 0)
                        sub.activeShield = false;
                }
            }
            else if (ent.hasTag<PlayerTag>())
            {
                auto& plfi = em.getSingleton<PlayerInfo>();
                em.getSingleton<SoundSystem>().sonido_recibir_danyo();
                if (plfi.armor > 0)
                {
                    lif.life += lif.lifeLost;
                    plfi.armor -= lif.lifeLost;

                    if (plfi.armor < 0)
                    {
                        lif.life += plfi.armor;
                        plfi.armor = 0;
                    }
                }
            }
            else if (ent.hasTag<DummyTag>() || ent.hasTag<DestructibleTag>())
            {
                em.getSingleton<SoundSystem>().sonido_dummy_golpe();
            }
            else if (ent.hasTag<CrusherTag>())
            {
                em.getSingleton<SoundSystem>().sonido_apisonadora_danyo();
            }
            else if (ent.hasTag<GolemTag>())
            {
                em.getSingleton<SoundSystem>().sonido_golem_danyo();
            }
            else if (ent.hasTag<SnowmanTag>())
            {
                em.getSingleton<SoundSystem>().sonido_munyeco_danyo();
            }

            lif.lifeLost = 0;
        }

        lif.decreaseCountdown(timeStep);

        if (lif.life == 0)
        {
            // Si es enemigo creamos un objeto
            if (ent.hasTag<EnemyTag>() && !lif.decreaseNextFrame)
            {
                auto& phy = em.getComponent<PhysicsComponent>(ent);
                createObject(em, os, phy.position);
                em.getSingleton<SoundSystem>().sonido_muerte_enemigo();

                if (li.playerDetected)
                    li.enemyToChestPos = phy.position;
            }

            //Si es un slime
            if (ent.hasTag<SlimeTag>())
            {
                if (!lif.decreaseNextFrame)
                    lif.decreaseNextFrame = true;
                else
                    lif.decreaseNextFrame = false;
                if (em.getComponent<AIComponent>(ent).healbeforedie) {
                    em.getComponent<AttackerComponent>(ent).attack(AttackType::HealSpellSetup);
                }
                else {
                    em.getComponent<AttackerComponent>(ent).attack(AttackType::HealSpellSetup);
                }
            }
            //si es un golem
            if (ent.hasTag<GolemTag>()) {
                if (!lif.decreaseNextFrame) {
                    lif.decreaseNextFrame = true;
                    em.getSingleton<SoundSystem>().sonido_golem_muere();
                }
                else
                    lif.decreaseNextFrame = false;
                //  if (ent.hasComponent<AttackerComponent>()) {
                //     em.getComponent<AttackerComponent>(ent).attack(AttackType::AreaAttack);
                //  }
            }

            //si es un snowman
            if (ent.hasTag<SnowmanTag>()) {
                if (!lif.decreaseNextFrame)
                    em.getSingleton<SoundSystem>().sonido_munyeco_muere();
            }

            //Si es una bala
            if (ent.hasTag<HitPlayerTag>()) {
                if (!lif.decreaseNextFrame)
                    lif.decreaseNextFrame = true;
                else
                    lif.decreaseNextFrame = false;
            }

            if (ent.hasTag<SubjectTag>()) {
                if (!lif.decreaseNextFrame)
                    lif.decreaseNextFrame = true;
                else
                    lif.decreaseNextFrame = false;
                auto& bb = em.getSingleton<BlackBoard_t>();
                bb.subditosData.erase(ent.getID());
            }

            if (ent.hasTag<BossFinalTag>()) {
                // if (!lif.decreaseNextFrame)
                //     lif.decreaseNextFrame = true;
                // else
                //     lif.decreaseNextFrame = false;
                auto& bb = em.getSingleton<BlackBoard_t>();
                bb.boss_fase++;
            }

            if (ent.hasTag<DestructibleTag>()) {
                if (li.mapID == 1) {
                    li.door_open = true;
                }
            }

            if (li.lockedEnemy == ent.getID())
                li.lockedEnemy = li.max;

            lif.markedForDeletion = true;
        }

        // Para cuando se recoge una poción de vida
        if (ent.hasTag<PlayerTag>())
        {
            auto& plfi = em.getSingleton<PlayerInfo>();
            if (plfi.increaseLife > 0.0)
            {
                lif.life += static_cast<int>(plfi.increaseLife);
                if (lif.life > lif.maxLife)
                    lif.life = lif.maxLife;
                plfi.increaseLife = 0.0;
            }
        }

        if (lif.markedForDeletion && !lif.decreaseNextFrame)
        {
            if (!ent.hasTag<EnemyTag>())
                li.insertDeath(ent.getID());
            else
            {
                em.addTag<EnemyDeathTag>(ent);

                if (ent.hasComponent<RenderComponent>())
                {
                    auto& ren = em.getComponent<RenderComponent>(ent);
                    ren.node->setVisible(false);
                }
            }
        }
    });
}

// Se podra crear objetos: vida, bomba, moneda o nada
void LifeSystem::createObject(EntityManager&, ObjectSystem& os, vec3d pos) {
    int random_value = std::rand();
    if (random_value % 4 > 0) {
        ObjectType tipo_nuevo_objeto{};
        Color color_nuevo_objeto{};
        if (random_value % 4 == 1)
        {
            tipo_nuevo_objeto = ObjectType::Coin;
            color_nuevo_objeto = D_YELLOW;
        }
        else if (random_value % 4 == 2)
        {
            tipo_nuevo_objeto = ObjectType::Life;
            color_nuevo_objeto = D_RED;
        }
        else if (random_value % 4 == 3)
        {
            tipo_nuevo_objeto = ObjectType::Mana_Potion;
            color_nuevo_objeto = D_BLUE_LIGHT;
        }
        else
        {
            tipo_nuevo_objeto = ObjectType::None;
        }

        // Se crea el nuevo objeto en el object system
        os.addObject(tipo_nuevo_objeto, pos);
    }
}
