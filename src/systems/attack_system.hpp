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
    void createAttackRangedOrMelee(EntityManager& em, Entity& e, AttackComponent& att, bool isRanged, double const scale_to_respawn_attack,double const ranged);
    void createAttackMultipleShot(EntityManager& em, Entity& ent, AttackComponent& att, int numShots);
};

#endif // !ATTACK_SYSTEM
