#pragma once
#ifndef ATTACK_SYSTEM
#define ATTACK_SYSTEM
#include "../utils/types.hpp"
#include "collision_system.hpp"
#include "../managers/attack_manager.hpp"

struct AttackSystem
{
    // Se van a buscar las entidad que tengan estos componentes y tags
    using SYSCMPs = MP::TypeList<AttackerComponent>;
    using SYSTAGs = MP::TypeList<>;

    void update(EntityManager& em, AttackManager& am);

private:
    void resolvePlayerDirection(PhysicsComponent& playerPhy, PhysicsComponent& enemyPhy, bool isEnemy);

    std::map<AttackType, std::function<void(EntityManager&, Entity&, AttackComponent&, AttackManager&)>> attackMap = {
    { AttackType::MeleePlayer, [&](EntityManager& em, Entity&, AttackComponent& att, AttackManager&)
    {
        auto& plfi = em.getSingleton<PlayerInfo>();
        float baseDamage = plfi.attackUpgrade ? att.damage + 1 : att.damage;
        for (auto& targetID : att.targets)
        {
            auto& target = *em.getEntityByID(targetID);
            if (target.hasComponent<LifeComponent>())
            {
                if (target.hasComponent<TypeComponent>())
                {
                    auto& tpc = em.getComponent<TypeComponent>(target);
                    baseDamage *= att.resolveType(tpc.type);
                }

                auto& life = em.getComponent<LifeComponent>(target);
                life.decreaseLife(static_cast<int>(baseDamage));
            }
        }
        att.targets.clear();
    }},
    { AttackType::SpiderShot, [&](EntityManager& em, Entity& atkEnt, AttackComponent&, AttackManager& am)
    {
        auto& li = em.getSingleton<LevelInfo>();
        am.createAttackType(em, atkEnt, AttackType::Spiderweb);
        li.insertDeath(atkEnt.getID());
    }},
    { AttackType::Spiderweb, [&](EntityManager& em, Entity&, AttackComponent& att, AttackManager&)
    {
        for (auto& targetID : att.targets)
        {
            auto& target = *em.getEntityByID(targetID);
            if (target.hasTag<PlayerTag>() && target.hasComponent<PhysicsComponent>())
            {
                auto& lc = em.getComponent<LifeComponent>(target);
                if (lc.onDeathAnim)
                    return;

                auto& playerPhy = em.getComponent<PhysicsComponent>(target);
                em.getSingleton<BlackBoard_t>().playerhunted = true;
                playerPhy.dragActivated = true;
                return;
            }
        }
        att.targets.clear();
    }},
    { AttackType::CrusherAttack, [&](EntityManager& em, Entity& atkEnt, AttackComponent& att, AttackManager&)
    {
        if (atkEnt.hasComponent<PhysicsComponent>())
        {
            for (auto& targetID : att.targets)
            {
                auto& target = *em.getEntityByID(targetID);
                if (target.hasTag<PlayerTag>() && target.hasComponent<PhysicsComponent>())
                {
                    auto& lc = em.getComponent<LifeComponent>(target);
                    if (lc.onDeathAnim)
                        return;

                    auto& playerPhy = em.getComponent<PhysicsComponent>(target);
                    auto& atkPhy = em.getComponent<PhysicsComponent>(atkEnt);
                    auto& bb = em.getSingleton<BlackBoard_t>();
                    if (bb.playerdamagebycrusher == false) {
                        lc.decreaseLife(att.damage);
                        bb.playerdamagebycrusher = true;

                        // El jugador se mueve hacia atrás de la posición del crusher
                        resolvePlayerDirection(playerPhy, atkPhy, false);
                        em.getSingleton<SoundSystem>().sonido_rebote();
                    }
                    return;
                }
            }
            att.targets.clear();
        }
    }},
    { AttackType::WaterBombShot, [&](EntityManager& em, Entity& atkEnt, AttackComponent& att, AttackManager& am)
    {
        auto& li = em.getSingleton<LevelInfo>();
        auto& plfi = em.getSingleton<PlayerInfo>();
        float baseDamage = plfi.attackUpgrade ? att.damage + 1 : att.damage;

        for (auto& targetID : att.targets)
        {
            auto& target = *em.getEntityByID(targetID);
            if (target.hasComponent<LifeComponent>())
            {
                if (target.hasComponent<TypeComponent>())
                {
                    auto& tpc = em.getComponent<TypeComponent>(target);
                    baseDamage *= att.resolveType(tpc.type);
                }

                auto& life = em.getComponent<LifeComponent>(target);
                life.decreaseLife(static_cast<int>(baseDamage));

                am.createAttackType(em, atkEnt, AttackType::WaterBomb);
            }
        }
        li.insertDeath(atkEnt.getID());
    }},
    { AttackType::FireBallShot, [&](EntityManager& em, Entity& atkEnt, AttackComponent& att, AttackManager& am)
    {
        auto& li = em.getSingleton<LevelInfo>();
        auto& plfi = em.getSingleton<PlayerInfo>();
        float baseDamage = plfi.attackUpgrade ? att.damage + 1 : att.damage;
        for (auto& targetID : att.targets)
        {
            auto& target = *em.getEntityByID(targetID);
            if (target.hasComponent<LifeComponent>() && target.hasComponent<TypeComponent>())
            {
                auto& tpc = em.getComponent<TypeComponent>(target);
                auto& life = em.getComponent<LifeComponent>(target);

                baseDamage *= att.resolveType(tpc.type);

                life.decreaseLife(static_cast<int>(baseDamage));

                am.createAttackType(em, atkEnt, AttackType::FireBall);
            }
        }
        li.insertDeath(atkEnt.getID());
    }},
    { AttackType::WaterBomb, [&](EntityManager& em, Entity&, AttackComponent& att, AttackManager&)
    {
        auto& plfi = em.getSingleton<PlayerInfo>();
        float baseDamage = plfi.attackUpgrade ? att.damage + 1 : att.damage;
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
        att.targets.clear();
    }},
    { AttackType::FireBall, [&](EntityManager& em, Entity&, AttackComponent& att, AttackManager&)
    {
        auto& plfi = em.getSingleton<PlayerInfo>();
        float baseDamage = plfi.attackUpgrade ? att.damage + 1 : att.damage;
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
        att.targets.clear();
    }},
    { AttackType::WaterDashArea, [&](EntityManager& em, Entity&, AttackComponent& att, AttackManager&)
    {
        auto& plfi = em.getSingleton<PlayerInfo>();
        float baseDamage = plfi.attackUpgrade ? att.damage + 1 : att.damage;
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
        att.targets.clear();
    }},
    { AttackType::MeteoritePlayer, [&](EntityManager& em, Entity& atkEnt, AttackComponent& att, AttackManager&)
    {
        auto& li = em.getSingleton<LevelInfo>();
        auto& plfi = em.getSingleton<PlayerInfo>();
        float baseDamage = plfi.attackUpgrade ? att.damage + 1 : att.damage;
        for (auto& targetID : att.targets)
        {
            auto& target = *em.getEntityByID(targetID);
            if (target.hasComponent<LifeComponent>())
            {
                if (target.hasComponent<TypeComponent>())
                {
                    auto& tpc = em.getComponent<TypeComponent>(target);
                    baseDamage *= att.resolveType(tpc.type);
                }

                auto& life = em.getComponent<LifeComponent>(target);
                life.decreaseLife(static_cast<int>(baseDamage));
            }
        }
        li.insertDeath(atkEnt.getID());
    }},
    { AttackType::IceShard, [&](EntityManager& em, Entity& atkEnt, AttackComponent& att, AttackManager&)
    {
        auto& li = em.getSingleton<LevelInfo>();
        auto& plfi = em.getSingleton<PlayerInfo>();
        float baseDamage = plfi.attackUpgrade ? att.damage + 1 : att.damage;

        for (auto& targetID : att.targets)
        {
            auto& target = *em.getEntityByID(targetID);
            if (target.hasComponent<LifeComponent>())
            {
                if (target.hasComponent<TypeComponent>())
                {
                    auto& tpc = em.getComponent<TypeComponent>(target);
                    baseDamage *= att.resolveType(tpc.type);
                }

                auto& life = em.getComponent<LifeComponent>(target);
                life.decreaseLife(static_cast<int>(baseDamage));
            }
        }
        li.insertDeath(atkEnt.getID());
    } },
    { AttackType::GollemAttack, [&](EntityManager& em, Entity& atkEnt, AttackComponent& att, AttackManager&)
    {
        if (atkEnt.hasComponent<PhysicsComponent>())
        {
            for (auto& targetID : att.targets)
            {
                auto& target = *em.getEntityByID(targetID);
                if (target.hasTag<PlayerTag>() && target.hasComponent<PhysicsComponent>())
                {
                    auto& lc = em.getComponent<LifeComponent>(target);
                    if (lc.onDeathAnim)
                        return;

                    auto& playerPhy = em.getComponent<PhysicsComponent>(target);
                    auto& atkPhy = em.getComponent<PhysicsComponent>(atkEnt);

                    lc.decreaseLife(att.damage);

                    // El jugador se mueve hacia atrás de la posición del crusher
                    resolvePlayerDirection(playerPhy, atkPhy, false);
                    playerPhy.dragActivatedTime = true;
                    em.getSingleton<SoundSystem>().sonido_rebote();

                    auto& li = em.getSingleton<LevelInfo>();
                    li.insertDeath(atkEnt.getID());
                    return;
                }
            }
            att.targets.clear();
        }
    }},
    { AttackType::SnowmanBall, [&](EntityManager& em, Entity& atkEnt, AttackComponent& att, AttackManager&)
    {
        auto& li = em.getSingleton<LevelInfo>();
        if (atkEnt.hasComponent<PhysicsComponent>())
        {
            for (auto& targetID : att.targets)
            {
                auto& target = *em.getEntityByID(targetID);
                if (target.hasTag<PlayerTag>() && target.hasComponent<PhysicsComponent>())
                {
                    auto& lc = em.getComponent<LifeComponent>(target);
                    if (lc.onDeathAnim)
                        return;

                    auto& playerPhy = em.getComponent<PhysicsComponent>(target);
                    auto& atkPhy = em.getComponent<PhysicsComponent>(atkEnt);

                    lc.decreaseLife(att.damage);

                    // El jugador se mueve hacia atrás de la posición del crusher
                    resolvePlayerDirection(playerPhy, atkPhy, false);
                    em.getSingleton<SoundSystem>().sonido_rebote();

                    auto& li = em.getSingleton<LevelInfo>();
                    li.insertDeath(atkEnt.getID());
                    return;
                }
            }
            att.targets.clear();
        }
        li.insertDeath(atkEnt.getID());
    }},
    { AttackType::AirAttack, [&](EntityManager& em, Entity& atkEnt, AttackComponent& att, AttackManager&)
    {
        if (atkEnt.hasComponent<PhysicsComponent>())
        {
            for (auto& targetID : att.targets)
            {
                auto& target = *em.getEntityByID(targetID);
                if (target.hasTag<PlayerTag>() && target.hasComponent<PhysicsComponent>())
                {
                    auto& lc = em.getComponent<LifeComponent>(target);
                    if (lc.onDeathAnim)
                        return;

                    auto& playerPhy = em.getComponent<PhysicsComponent>(target);
                    auto& atkPhy = em.getComponent<PhysicsComponent>(atkEnt);

                    lc.decreaseLife(att.damage);

                    // El jugador se mueve hacia atrás de la posición del crusher
                    resolvePlayerDirection(playerPhy, atkPhy, false);
                    em.getSingleton<SoundSystem>().sonido_rebote();

                    return;
                }
            }
            att.targets.clear();
        }
    }},
    { AttackType::HealSpellSetup, [&](EntityManager& em, Entity& atkEnt, AttackComponent&, AttackManager& am)
    {
        am.createAttackType(em, atkEnt, AttackType::HealSpell);
    }},
    { AttackType::HealSpell, [&](EntityManager& em, Entity& atkEnt, AttackComponent& att, AttackManager&)
    {
        for (auto& targetID : att.targets)
        {
            auto& target = *em.getEntityByID(targetID);
            if (target.hasTag<PlayerTag>() && target.hasComponent<PhysicsComponent>())
            {
                auto& lc = em.getComponent<LifeComponent>(target);
                if (lc.onDeathAnim)
                    return;

                auto& playerPhy = em.getComponent<PhysicsComponent>(target);
                auto& atkPhy = em.getComponent<PhysicsComponent>(atkEnt);

                lc.decreaseLife(att.damage);

                // El jugador se mueve hacia atrás de la posición del crusher
                resolvePlayerDirection(playerPhy, atkPhy, false);
                em.getSingleton<SoundSystem>().sonido_rebote();
            }
            else if (target.hasTag<SlimeTag>() && target.hasComponent<LifeComponent>())
            {
                auto& life = em.getComponent<LifeComponent>(target);
                life.increaseLife(att.damage);
                em.getSingleton<SoundSystem>().sonido_slime_curar();
            }
        }
        att.targets.clear();
    }},
    { AttackType::MeleeEnemy, [&](EntityManager& em, Entity& atkEnt, AttackComponent& att, AttackManager&)
    {
        if (atkEnt.hasComponent<PhysicsComponent>())
        {
            for (auto& targetID : att.targets)
            {
                auto& target = *em.getEntityByID(targetID);
                if (target.hasTag<PlayerTag>() && target.hasComponent<PhysicsComponent>())
                {
                    auto& lc = em.getComponent<LifeComponent>(target);
                    if (lc.onDeathAnim)
                        return;

                    auto& playerPhy = em.getComponent<PhysicsComponent>(target);
                    auto& atkPhy = em.getComponent<PhysicsComponent>(atkEnt);

                    lc.decreaseLife(att.damage);

                    // El jugador se mueve hacia atrás de la posición del crusher
                    resolvePlayerDirection(playerPhy, atkPhy, false);
                    return;
                }
            }
            att.targets.clear();
        }
    }},
    { AttackType::RangedEnemy, [&](EntityManager& em, Entity& atkEnt, AttackComponent& att, AttackManager&)
    {
        auto& li = em.getSingleton<LevelInfo>();
        if (atkEnt.hasComponent<PhysicsComponent>())
        {
            for (auto& targetID : att.targets)
            {
                auto& target = *em.getEntityByID(targetID);
                if (target.hasTag<PlayerTag>() && target.hasComponent<PhysicsComponent>())
                {
                    auto& lc = em.getComponent<LifeComponent>(target);
                    if (lc.onDeathAnim)
                        return;

                    auto& playerPhy = em.getComponent<PhysicsComponent>(target);
                    auto& atkPhy = em.getComponent<PhysicsComponent>(atkEnt);

                    lc.decreaseLife(att.damage);

                    // El jugador se mueve hacia atrás de la posición del crusher
                    resolvePlayerDirection(playerPhy, atkPhy, false);
                    return;
                }
            }
            att.targets.clear();
        }
        li.insertDeath(atkEnt.getID());
    }},
    };
};

#endif // !ATTACK_SYSTEM
