#pragma once

#include "../utils/types.hpp"
#include "../components/attack_component.hpp"
#include "../systems/collision_system.hpp"

struct AttackManager
{
    void createAttackType(EntityManager& em, Entity& e, AttackComponent& attack);
    void createAttackType(EntityManager& em, Entity& e, AttackType type);

private:
    void resolvePlayerDirection(PhysicsComponent& playerPhy, PhysicsComponent& enemyPhy, bool isEnemy);
    void createSpiderWeb(EntityManager& em, vec3d& pos);
    void createAttackMultipleShot(EntityManager& em, Entity& ent, AttackComponent& att, int numShots);
    void createAttackRangedOrMelee(EntityManager& em, Entity& ent, AttackComponent& att, bool isRanged, double const scale_to_respawn_attack, double const ranged);
    void createAttackRangedOrMelee(EntityManager& em, vec3d pos, double ori, vec3d vel, AttackType atkType, uint16_t damage, ElementalType type, BehaviorType behavior);
    void createSpellAttack(EntityManager& em, Entity& ent, AttackComponent& att);
    void setCollisionSystem(CollisionSystem* col);
    void setPlayerAtkVel(EntityManager& em, Entity& e, AttackComponent& att);
    void setPlayerAtkVel(EntityManager& em, vec3d& pos, double ori, vec3d& vel);
    vec3d getPosMeteorito(uint16_t fase, vec3d posplayer);

    CollisionSystem* col_sys{ nullptr };
};