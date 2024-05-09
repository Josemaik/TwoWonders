#pragma once

#include "../utils/types.hpp"
#include "../components/attack_component.hpp"
#include "../systems/collision_system.hpp"

struct AttackManager
{
    std::map<AttackType, std::function<void(EntityManager&, Entity&, Attack&)>> attackMap = {
        { AttackType::MeleePlayer, [&](EntityManager& em, Entity& e, Attack& att)
        {
            auto& plfi = em.getSingleton<PlayerInfo>();
            auto baseDamage = plfi.attackUpgrade ? att.damage + 1 : att.damage;
            for (auto& targetID : att.targets)
            {
                auto& target = *em.getEntityByID(targetID);
                if (target.hasComponent<LifeComponent>() && target.hasComponent<TypeComponent>())
                {
                    auto& tpc = em.getComponent<TypeComponent>(target);
                    auto& life = em.getComponent<LifeComponent>(target);

                    baseDamage *= att.resolveType(tpc.type);

                    life.decreaseLife(static_cast<int>(baseDamage));
                }
            }
        }},
        { AttackType::SpiderShot, [&](EntityManager& em, Entity& atkEnt, Attack& att)
        {
            auto& li = em.getSingleton<LevelInfo>();
            auto& phy = em.getComponent<PhysicsComponent>(atkEnt);

            createSpiderWeb(em, phy.position);
            li.insertDeath(atkEnt.getID());
        }},
        { AttackType::Spiderweb, [&](EntityManager& em, Entity& atkEnt, Attack& att)
        {
            for (auto& targetID : att.targets)
            {
                auto& target = *em.getEntityByID(targetID);
                if (target.hasTag<PlayerTag>() && target.hasComponent<PhysicsComponent>())
                {
                    auto& playerPhy = em.getComponent<PhysicsComponent>(target);
                    em.getSingleton<BlackBoard_t>().playerhunted = true;
                    playerPhy.dragActivated = true;
                    return;
                }
            }
        }},
        { AttackType::AreaCrusher, [&](EntityManager& em, Entity& atkEnt, Attack& att)
        {
            if (atkEnt.hasComponent<PhysicsComponent>())
            {
                for (auto& targetID : att.targets)
                {
                    auto& target = *em.getEntityByID(targetID);
                    if (target.hasTag<PlayerTag>() && target.hasComponent<PhysicsComponent>())
                    {
                        auto& playerPhy = em.getComponent<PhysicsComponent>(target);
                        auto& atkPhy = em.getComponent<PhysicsComponent>(atkEnt);
                        auto& bb = em.getSingleton<BlackBoard_t>();
                        if (bb.playerdamagebycrusher == false) {
                            em.getComponent<LifeComponent>(target).decreaseLife(att.damage);
                            bb.playerdamagebycrusher = true;

                            // El jugador se mueve hacia atrás de la posición del crusher
                            resolvePlayerDirection(playerPhy, atkPhy, false);
                            em.getSingleton<SoundSystem>().sonido_rebote();
                        }
                        return;
                    }
                }
            }
        }},
        { AttackType::WaterBombShot, [&](EntityManager& em, Entity& atkEnt, Attack& att)
        {
            auto& li = em.getSingleton<LevelInfo>();
            auto& plfi = em.getSingleton<PlayerInfo>();
            auto baseDamage = plfi.attackUpgrade ? att.damage + 1 : att.damage;
            for (auto& targetID : att.targets)
            {
                auto& target = *em.getEntityByID(targetID);
                if (target.hasComponent<LifeComponent>() && target.hasComponent<TypeComponent>())
                {
                    auto& tpc = em.getComponent<TypeComponent>(target);
                    auto& life = em.getComponent<LifeComponent>(target);

                    baseDamage *= att.resolveType(tpc.type);

                    life.decreaseLife(static_cast<int>(baseDamage));

                    // Función de crear ataques -> waterbomb
                }
            }
            li.insertDeath(atkEnt.getID());
        }},
        { AttackType::FireBallShot, [&](EntityManager& em, Entity& atkEnt, Attack& att)
        {
            auto& li = em.getSingleton<LevelInfo>();
            auto& plfi = em.getSingleton<PlayerInfo>();
            auto baseDamage = plfi.attackUpgrade ? att.damage + 1 : att.damage;
            for (auto& targetID : att.targets)
            {
                auto& target = *em.getEntityByID(targetID);
                if (target.hasComponent<LifeComponent>() && target.hasComponent<TypeComponent>())
                {
                    auto& tpc = em.getComponent<TypeComponent>(target);
                    auto& life = em.getComponent<LifeComponent>(target);

                    baseDamage *= att.resolveType(tpc.type);

                    life.decreaseLife(static_cast<int>(baseDamage));

                    // Función de crear ataques -> fireball
                }
            }
            li.insertDeath(atkEnt.getID());
        }},
        { AttackType::WaterBomb, [&](EntityManager& em, Entity& atkEnt, Attack& att)
        {
            auto& plfi = em.getSingleton<PlayerInfo>();
            auto baseDamage = plfi.attackUpgrade ? att.damage + 1 : att.damage;
            for (auto& targetID : att.targets)
            {
                auto& target = *em.getEntityByID(targetID);
                if (target.hasComponent<LifeComponent>() && target.hasComponent<TypeComponent>())
                {
                    auto& tpc = em.getComponent<TypeComponent>(target);
                    auto& life = em.getComponent<LifeComponent>(target);

                    baseDamage *= att.resolveType(tpc.type);

                    life.decreaseLife(static_cast<int>(baseDamage));
                }
            }
        }},
        { AttackType::FireBall, [&](EntityManager& em, Entity& atkEnt, Attack& att)
        {
            auto& plfi = em.getSingleton<PlayerInfo>();
            auto baseDamage = plfi.attackUpgrade ? att.damage + 1 : att.damage;
            for (auto& targetID : att.targets)
            {
                auto& target = *em.getEntityByID(targetID);
                if (target.hasComponent<LifeComponent>() && target.hasComponent<TypeComponent>())
                {
                    auto& tpc = em.getComponent<TypeComponent>(target);
                    auto& life = em.getComponent<LifeComponent>(target);

                    baseDamage *= att.resolveType(tpc.type);

                    life.decreaseLife(static_cast<int>(baseDamage));
                }
            }
        }},
        { AttackType::WaterDashArea, [&](EntityManager& em, Entity& atkEnt, Attack& att)
        {
            auto& plfi = em.getSingleton<PlayerInfo>();
            auto baseDamage = plfi.attackUpgrade ? att.damage + 1 : att.damage;
            for (auto& targetID : att.targets)
            {
                auto& target = *em.getEntityByID(targetID);
                if (target.hasComponent<LifeComponent>() && target.hasComponent<TypeComponent>())
                {
                    auto& tpc = em.getComponent<TypeComponent>(target);
                    auto& life = em.getComponent<LifeComponent>(target);

                    baseDamage *= att.resolveType(tpc.type);

                    life.decreaseLife(static_cast<int>(baseDamage));
                }
            }
        }},
        { AttackType::MeteoritePlayer, [&](EntityManager& em, Entity& atkEnt, Attack& att)
        {
            auto& li = em.getSingleton<LevelInfo>();
            auto& plfi = em.getSingleton<PlayerInfo>();
            auto baseDamage = plfi.attackUpgrade ? att.damage + 1 : att.damage;
            for (auto& targetID : att.targets)
            {
                auto& target = *em.getEntityByID(targetID);
                if (target.hasComponent<LifeComponent>() && target.hasComponent<TypeComponent>())
                {
                    auto& tpc = em.getComponent<TypeComponent>(target);
                    auto& life = em.getComponent<LifeComponent>(target);

                    baseDamage *= att.resolveType(tpc.type);

                    life.decreaseLife(static_cast<int>(baseDamage));
                }
            }
            li.insertDeath(atkEnt.getID());
        }},
        { AttackType::IceShard, [&](EntityManager& em, Entity& atkEnt, Attack& att)
        {
            auto& li = em.getSingleton<LevelInfo>();
            auto& plfi = em.getSingleton<PlayerInfo>();
            auto baseDamage = plfi.attackUpgrade ? att.damage + 1 : att.damage;
            for (auto& targetID : att.targets)
            {
                auto& target = *em.getEntityByID(targetID);
                if (target.hasComponent<LifeComponent>() && target.hasComponent<TypeComponent>())
                {
                    auto& tpc = em.getComponent<TypeComponent>(target);
                    auto& life = em.getComponent<LifeComponent>(target);

                    baseDamage *= att.resolveType(tpc.type);

                    life.decreaseLife(static_cast<int>(baseDamage));
                }
            }
            li.insertDeath(atkEnt.getID());
        } },
        { AttackType::GollemAttack, [&](EntityManager& em, Entity& atkEnt, Attack& att)
        {
            if (atkEnt.hasComponent<PhysicsComponent>())
            {
                for (auto& targetID : att.targets)
                {
                    auto& target = *em.getEntityByID(targetID);
                    if (target.hasTag<PlayerTag>() && target.hasComponent<PhysicsComponent>())
                    {
                        auto& playerPhy = em.getComponent<PhysicsComponent>(target);
                        auto& atkPhy = em.getComponent<PhysicsComponent>(atkEnt);

                        em.getComponent<LifeComponent>(target).decreaseLife(att.damage);

                        // El jugador se mueve hacia atrás de la posición del crusher
                        resolvePlayerDirection(playerPhy, atkPhy, false);
                        playerPhy.dragActivatedTime = true;
                        em.getSingleton<SoundSystem>().sonido_rebote();

                        auto& li = em.getSingleton<LevelInfo>();
                        li.insertDeath(atkEnt.getID());
                        return;
                    }
                }
            }
        }},
        { AttackType::SnowmanBall, [&](EntityManager& em, Entity& atkEnt, Attack& att)
        {
            if (atkEnt.hasComponent<PhysicsComponent>())
            {
                for (auto& targetID : att.targets)
                {
                    auto& target = *em.getEntityByID(targetID);
                    if (target.hasTag<PlayerTag>() && target.hasComponent<PhysicsComponent>())
                    {
                        auto& playerPhy = em.getComponent<PhysicsComponent>(target);
                        auto& atkPhy = em.getComponent<PhysicsComponent>(atkEnt);

                        em.getComponent<LifeComponent>(target).decreaseLife(att.damage);

                        // El jugador se mueve hacia atrás de la posición del crusher
                        resolvePlayerDirection(playerPhy, atkPhy, false);
                        em.getSingleton<SoundSystem>().sonido_rebote();

                        auto& li = em.getSingleton<LevelInfo>();
                        li.insertDeath(atkEnt.getID());
                        return;
                    }
                }
            }
        }},
        { AttackType::AirAttack, [&](EntityManager& em, Entity& atkEnt, Attack& att)
        {
            if (atkEnt.hasComponent<PhysicsComponent>())
            {
                for (auto& targetID : att.targets)
                {
                    auto& target = *em.getEntityByID(targetID);
                    if (target.hasTag<PlayerTag>() && target.hasComponent<PhysicsComponent>())
                    {
                        auto& playerPhy = em.getComponent<PhysicsComponent>(target);
                        auto& atkPhy = em.getComponent<PhysicsComponent>(atkEnt);

                        em.getComponent<LifeComponent>(target).decreaseLife(att.damage);

                        // El jugador se mueve hacia atrás de la posición del crusher
                        resolvePlayerDirection(playerPhy, atkPhy, false);
                        em.getSingleton<SoundSystem>().sonido_rebote();

                        return;
                    }
                }
            }
        }},
        { AttackType::HealSpell, [&](EntityManager& em, Entity& atkEnt, Attack& att)
        {
            for (auto& targetID : att.targets)
            {
                auto& target = *em.getEntityByID(targetID);
                if (target.hasTag<PlayerTag>() && target.hasComponent<PhysicsComponent>())
                {
                    auto& playerPhy = em.getComponent<PhysicsComponent>(target);
                    auto& atkPhy = em.getComponent<PhysicsComponent>(atkEnt);

                    em.getComponent<LifeComponent>(target).decreaseLife(att.damage);

                    // El jugador se mueve hacia atrás de la posición del crusher
                    resolvePlayerDirection(playerPhy, atkPhy, false);
                    em.getSingleton<SoundSystem>().sonido_rebote();
                }
                else if (target.hasTag<SlimeTag>() && target.hasComponent<LifeComponent>())
                {
                    auto& life = em.getComponent<LifeComponent>(target);
                    life.increaseLife(att.damage);
                }
            }
        }}
    };

    void createAttackType(EntityManager& em, Entity& e, AttackComponent& attack);

private:
    void resolvePlayerDirection(PhysicsComponent& playerPhy, PhysicsComponent& enemyPhy, bool isEnemy);
    void createSpiderWeb(EntityManager& em, vec3d& pos);
    void createAttackMultipleShot(EntityManager& em, Entity& ent, AttackComponent& att, int numShots);
    void createAttackRangedOrMelee(EntityManager& em, Entity& ent, AttackComponent& att, bool isRanged, double const scale_to_respawn_attack, double const ranged);
    void createSpellAttack(EntityManager& em, Entity& ent, AttackComponent& att);
    void setCollisionSystem(CollisionSystem* col);
    void setPlayerAtkVel(EntityManager& em, Entity& e, AttackComponent& att);
    vec3d getPosMeteorito(uint16_t fase, vec3d posplayer);

    CollisionSystem* col_sys{ nullptr };
};