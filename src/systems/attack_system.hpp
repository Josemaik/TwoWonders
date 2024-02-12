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
    void createAttack(EntityManager& em, Entity& e, AttackComponent& att,float dt);
    vec3d getPosMeteorito(uint16_t fase,vec3d posplayer);
    void createAttackRangedOrMelee(EntityManager& em, Entity& e, AttackComponent& att, bool isRanged, double const scale_to_respawn_attack, double const ranged);
    void createAttackMultipleShot(EntityManager& em, Entity& ent, AttackComponent& att, int numShots);

    static constexpr double MANA_CUT = 15.0f;
};

#endif // !ATTACK_SYSTEM
