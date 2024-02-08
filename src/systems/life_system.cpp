#include "life_system.hpp"

void LifeSystem::update(EntityManager& em, float deltaTime) {
    auto& li = em.getSingleton<LevelInfo>();

    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& ent, LifeComponent& lif)
    {
        lif.decreaseCountdown(deltaTime);

        if (lif.life == 0)
        {
            // Si es enemigo creamos un objeto
            if (ent.hasTag<EnemyTag>() && !lif.decreaseNextFrame)
                createObject(em, em.getComponent<PhysicsComponent>(ent).position);
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

            if(ent.hasTag<SubditoTag>()){
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
void LifeSystem::createObject(EntityManager& em, vec3d pos) {
    int random_value = std::rand();
    if (random_value % 5 > 0) {
        Object_type tipo_nuevo_objeto{};
        Color color_nuevo_objeto{};
        if (random_value % 5 == 1)
        {
            tipo_nuevo_objeto = Object_type::Bomb;
            color_nuevo_objeto = GRAY;
        }
        else if (random_value % 5 == 2)
        {
            tipo_nuevo_objeto = Object_type::Coin;
            color_nuevo_objeto = YELLOW;
        }
        else if (random_value % 5 == 3)
        {
            tipo_nuevo_objeto = Object_type::Life;
            color_nuevo_objeto = RED;
        }
        else if (random_value % 5 == 4)
        {
            tipo_nuevo_objeto = Object_type::Mana_Potion;
            color_nuevo_objeto = SKYBLUE;
        }
        else
        {
            tipo_nuevo_objeto = Object_type::None;
        }

        // Se crea el nuevo objeto
        auto& e{ em.newEntity() };
        em.addTag<ObjectTag>(e);
        auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = pos, .scale = { 0.5, 0.5, 0.5 }, .color = color_nuevo_objeto });
        auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position{ r.position } });
        em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::STATIC });
        em.addComponent<ObjectComponent>(e, ObjectComponent{ .type = tipo_nuevo_objeto });
    }
}
