#include "life_system.hpp"

void LifeSystem::update(EntityManager& em, ObjectSystem& os, float deltaTime) {
    auto& li = em.getSingleton<LevelInfo>();

    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& ent, LifeComponent& lif)
    {
        lif.decreaseCountdown(deltaTime);

        if (lif.life == 0)
        {
            // Si es enemigo creamos un objeto
            if (ent.hasTag<EnemyTag>() && !lif.decreaseNextFrame)
                createObject(em, os, em.getComponent<PhysicsComponent>(ent).position);
            //Si es un slime
            if (ent.hasTag<SlimeTag>())
            {
                if (!lif.decreaseNextFrame)
                    lif.decreaseNextFrame = true;
                else
                    lif.decreaseNextFrame = false;
                if (em.getComponent<AIComponent>(ent).healbeforedie) {
                    em.getComponent<AttackComponent>(ent).attack(AttackType::HealSpell);
                }
                else {
                    em.getComponent<AttackComponent>(ent).attack(AttackType::HealSpell);
                }
            }
            //si es un golem
            if (ent.hasTag<GolemTag>()) {
                if (!lif.decreaseNextFrame)
                    lif.decreaseNextFrame = true;
                else
                    lif.decreaseNextFrame = false;

                em.getComponent<AttackComponent>(ent).attack(AttackType::AreaAttack);
            }

            //Si es una bala
            if (ent.hasTag<HitPlayerTag>()) {
                if (!lif.decreaseNextFrame)
                    lif.decreaseNextFrame = true;
                else
                    lif.decreaseNextFrame = false;

                if (ent.hasComponent<ColliderComponent>() && ent.hasComponent<AttackComponent>()) {
                    if (em.getComponent<ColliderComponent>(ent).attackType == AttackType::Spiderweb) {
                        em.getComponent<AttackComponent>(ent).attack(AttackType::Spiderweb);
                    }
                }
            }

            if (ent.hasTag<SubditoTag>()) {
                if (!lif.decreaseNextFrame)
                    lif.decreaseNextFrame = true;
                else
                    lif.decreaseNextFrame = false;
                auto& bb = em.getSingleton<BlackBoard_t>();
                bb.subditosData.erase(ent.getID());
            }

            lif.markedForDeletion = true;
        }

        if (lif.markedForDeletion && !lif.decreaseNextFrame)
            li.dead_entities.insert(ent.getID());

    });
}

// Se podra crear objetos: vida, bomba, moneda o nada
void LifeSystem::createObject(EntityManager&, ObjectSystem& os, vec3d pos) {
    int random_value = std::rand();
    if (random_value % 5 > 0) {
        ObjectType tipo_nuevo_objeto{};
        Color color_nuevo_objeto{};
        if (random_value % 5 == 1)
        {
            tipo_nuevo_objeto = ObjectType::Bomb;
            color_nuevo_objeto = GRAY;
        }
        else if (random_value % 5 == 2)
        {
            tipo_nuevo_objeto = ObjectType::Coin;
            color_nuevo_objeto = YELLOW;
        }
        else if (random_value % 5 == 3)
        {
            tipo_nuevo_objeto = ObjectType::Life;
            color_nuevo_objeto = RED;
        }
        else if (random_value % 5 == 4)
        {
            tipo_nuevo_objeto = ObjectType::Mana_Potion;
            color_nuevo_objeto = SKYBLUE;
        }
        else
        {
            tipo_nuevo_objeto = ObjectType::None;
        }

        // Se crea el nuevo objeto en el object system
        os.addObject(tipo_nuevo_objeto, pos);
    }
}
