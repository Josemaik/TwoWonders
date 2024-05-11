#pragma once

#include "../utils/types.hpp"
#include "../components/attack_component.hpp"
#include "../systems/collision_system.hpp"

struct AttackManager
{
    void createAttackType(EntityManager& em, Entity& e, AttackType type, vec3d vel = vec3d::zero());

private:
    void resolvePlayerDirection(PhysicsComponent& playerPhy, PhysicsComponent& enemyPhy, bool isEnemy);
    void createAttackMultipleShot(EntityManager& em, vec3d pos, vec3d vel, double ori, AttackType atkType, uint16_t damage, ElementalType eleType, BehaviorType behavior, int numShots);
    void createAttackRangedOrMelee(EntityManager& em, vec3d pos, vec3d vel, double ori, AttackType atkType, uint16_t damage, ElementalType type, BehaviorType behavior);
    void createSpellAttack(EntityManager& em, Entity& ent, AttackerComponent& att);
    void setAtkVel(vec3d& pos, vec3d& vel, double ori, vec3d otherPos = vec3d::zero(), double velY = 0.7);
    void setPlayerAtkVel(EntityManager& em, vec3d& pos, vec3d& vel, double ori);
    bool checkWallCollision(EntityManager& em, vec3d& pos, vec3d& normalDir);
    vec3d getPosMeteorito(uint16_t fase, vec3d posplayer);

    uint8_t airAttackFase{ 0 };
    float airAttackElapsed{ 0.0f }, airAttackLimit{ 0.2f }, airAttackWarningElapsed{ 0.0f }, airAttackWarningLimit{ 0.75f };
};