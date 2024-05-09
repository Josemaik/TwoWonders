#pragma once
#ifndef ATTACK_SYSTEM
#define ATTACK_SYSTEM
#include "../utils/types.hpp"
#include "collision_system.hpp"
#include "../managers/attack_manager.hpp"

struct AttackSystem
{
    // Se van a buscar las entidad que tengan estos componentes y tags
    using SYSCMPs = MP::TypeList<AttackComponent>;
    using SYSTAGs = MP::TypeList<>;

    void update(EntityManager& em, AttackManager& am);
    void setCollisionSystem(CollisionSystem* col_sys);

private:
    void createAttack(EntityManager& em, Entity& e, AttackComponent& att);
    void createAttackType(EntityManager& em, Entity& e, AttackComponent& att);
    vec3d getPosMeteorito(uint16_t fase, vec3d posplayer);

    void createAttackRangedOrMelee(EntityManager& em, Entity& e, AttackComponent& att, bool isRanged, double const scale_to_respawn_attack, double const ranged);
    void createAttackMultipleShot(EntityManager& em, Entity& ent, AttackComponent& att, int numShots);
    void createSpellAttack(EntityManager& em, Entity& e, AttackComponent& att);
    void setPlayerAtkVel(EntityManager& em, Entity& e, AttackComponent& att);
    static constexpr double MANA_CUT = 15.0f;

    CollisionSystem* col_sys{ nullptr };
};

#endif // !ATTACK_SYSTEM
