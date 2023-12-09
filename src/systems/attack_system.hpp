#pragma once
#ifndef ATTACK_SYSTEM
#define ATTACK_SYSTEM
#include "../utils/types.hpp"

struct AttackSystem
{
    // Se van a buscar las entidad que tengan estos componentes y tags
    using SYSCMPs = MP::TypeList<AttackComponent>;
    using SYSTAGs = MP::TypeList<>;

    void update(EntityManager& em, float deltaTime);

    private:
        void createAttack(EntityManager& em, Entity& e, AttackComponent& att);
        void createAttackRanged(EntityManager& em, Entity& e, AttackComponent& att);
        void createAttackMelee(EntityManager& em, Entity& e, AttackComponent& att);
};

#endif // !ATTACK_SYSTEM