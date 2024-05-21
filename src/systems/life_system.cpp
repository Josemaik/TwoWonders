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
                em.getComponent<AnimationComponent>(ent).changeAnimation(static_cast<std::size_t>(PlayerAnimations::GOT_HIT));

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
                if (lif.life > 0)
                {
                    auto& anc = em.getComponent<AnimationComponent>(ent);
                    if (anc.currentAnimation != static_cast<std::size_t>(GolemAnimations::ATTACK))
                        anc.changeAnimation(static_cast<std::size_t>(GolemAnimations::GOT_HIT));
                }
            }
            else if (ent.hasTag<SnowmanTag>())
            {
                em.getSingleton<SoundSystem>().sonido_munyeco_danyo();
                if (lif.life > 0)
                {
                    auto& anc = em.getComponent<AnimationComponent>(ent);
                    anc.changeAnimation(static_cast<std::size_t>(SnowmanAnimations::GOT_HIT));
                }

            }
            else if (ent.hasTag<SlimeTag>())
            {
                em.getSingleton<SoundSystem>().sonido_slime_danyo();
            }
            else if (ent.hasTag<SpiderTag>())
            {

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
                {
                    lif.decreaseNextFrame = true;
                    em.getSingleton<SoundSystem>().sonido_slime_muere();
                    if (ent.hasComponent<SoundComponent>())
                    {
                        auto& sc = em.getComponent<SoundComponent>(ent);
                        em.getSingleton<SoundSystem>().stop_enemigo_mov(sc.sound_mov);
                    }
                }
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
            else if (ent.hasTag<GolemTag>()) {
                if (!lif.decreaseNextFrame && !lif.onDeathAnim) {
                    // auto& ia = em.getComponent<AIComponent>(ent);
                    lif.decreaseNextFrame = true;
                    em.getSingleton<SoundSystem>().sonido_golem_muere();
                    if (ent.hasComponent<SoundComponent>()) {
                        auto& sc = em.getComponent<SoundComponent>(ent);
                        em.getSingleton<SoundSystem>().stop_enemigo_mov(sc.sound_mov);
                    }
                }
                else if (!lif.onDeathAnim)
                {
                    auto& anim = em.getComponent<AnimationComponent>(ent);
                    anim.changeAnimation(static_cast<std::size_t>(GolemAnimations::DEATH));
                    lif.onDeathAnim = true;
                    lif.decreaseNextFrame = true;

                    auto& inpi = em.getSingleton<InputInfo>();
                    li.lockedEnemy = li.max;
                    inpi.lockOn = false;
                }
            }

            //si es un snowman
            else if (ent.hasTag<SnowmanTag>()) {
                if (!lif.decreaseNextFrame && !lif.onDeathAnim)
                {
                    em.getSingleton<SoundSystem>().sonido_munyeco_muere();
                    lif.decreaseNextFrame = true;
                    if (ent.hasComponent<SoundComponent>())
                    {
                        auto& sc = em.getComponent<SoundComponent>(ent);
                        em.getSingleton<SoundSystem>().stop_enemigo_mov(sc.sound_mov);
                    }
                }
                else if (!lif.onDeathAnim)
                {
                    auto& anim = em.getComponent<AnimationComponent>(ent);
                    anim.changeAnimation(static_cast<std::size_t>(SnowmanAnimations::DEATH));
                    lif.onDeathAnim = true;
                    lif.decreaseNextFrame = true;

                    auto& inpi = em.getSingleton<InputInfo>();
                    li.lockedEnemy = li.max;
                    inpi.lockOn = false;
                }
            }

            else if (ent.hasTag<PlayerTag>()) {
                if (!lif.decreaseNextFrame && !lif.onDeathAnim)
                {
                    em.getSingleton<SoundSystem>().sonido_jugador_muere();
                    em.getComponent<AnimationComponent>(ent).changeAnimation(static_cast<std::size_t>(PlayerAnimations::DEATH));
                    lif.decreaseNextFrame = true;
                    lif.onDeathAnim = true;
                }
            }

            //Si es una bala
            else if (ent.hasTag<HitPlayerTag>()) {
                if (!lif.decreaseNextFrame)
                    lif.decreaseNextFrame = true;
                else
                    lif.decreaseNextFrame = false;
            }

            else  if (ent.hasTag<SubjectTag>()) {
                if (!lif.decreaseNextFrame)
                    lif.decreaseNextFrame = true;
                else
                    lif.decreaseNextFrame = false;
                auto& bb = em.getSingleton<BlackBoard_t>();
                bb.subditosData.erase(ent.getID());
            }

            else if (ent.hasTag<BossFinalTag>()) {
                // if (!lif.decreaseNextFrame)
                //     lif.decreaseNextFrame = true;
                // else
                //     lif.decreaseNextFrame = false;
                auto& bb = em.getSingleton<BlackBoard_t>();
                bb.boss_fase++;
            }

            else if (ent.hasTag<DestructibleTag>() && li.mapID == 1) {
                auto& doorAnim = em.getComponent<AnimationComponent>(ent);
                doorAnim.animToPlay = static_cast<std::size_t>(DoorAnimations::OPEN);

                lif.life = 20;
                li.door_open = true;
                em.getSingleton<SoundSystem>().sonido_abrir_puerta_magica();

                // Iniciamos la animación de caminar del nómada
                using AnimCMP = MP::TypeList<AnimationComponent>;
                using NomadTAG = MP::TypeList<NomadTag>;

                em.forEach<AnimCMP, NomadTAG>([](Entity&, AnimationComponent& anc)
                {
                    anc.animToPlay = static_cast<std::size_t>(NomadAnimations::WALK);
                });
            }

            if (li.lockedEnemy == ent.getID())
                li.lockedEnemy = li.max;
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
            {
                li.insertDeath(ent.getID());

                if (ent.hasTag<PlayerTag>())
                    lif.onDeathAnim = false;
            }
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
