#pragma once
#ifndef IA_MAN
#define IA_MAN

#include "../utils/types.hpp"
#include <cstdint>
#include <cmath>

//behaviour tree , node sequece and node selector
#include "../utils/bt/behaviourtree.hpp"
#include "../utils/bt/nodesequence.hpp"
#include "../utils/bt/nodeselector.hpp"
//actions
#include "../utils/bt/action_patrol.hpp"
#include "../utils/bt/action_randommovement.hpp"
#include "../utils/bt/action_changeposition.hpp"
#include "../utils/bt/action_diagonalmovement.hpp"
#include "../utils/bt/action_shoot.hpp"
#include "../utils/bt/action_arrive.hpp"
#include "../utils/bt/action_seek.hpp"
#include "../utils/bt/action_flee.hpp"
#include "../utils/bt/action_pursue.hpp"
#include "../utils/bt/action_avoid.hpp"
#include "../utils/bt/action_jumptoplayer.hpp"
#include "../utils/bt/action_healing.hpp"
#include "../utils/bt/action_heal_mate.hpp"
#include "../utils/bt/action_generatesubditos.hpp"
//decision
#include "../utils/bt/decision_playerdetected.hpp"
#include "../utils/bt/decision_readyforattack.hpp"
#include "../utils/bt/decision_on_attack_radius.hpp"
#include "../utils/bt/decision_checklifes.hpp"
#include "../utils/bt/decision_flee_or_curepartner.hpp"
#include "../utils/bt/decision_readyforheal.hpp"
#include "../utils/bt/decision_playerhunted.hpp"
#include "../utils/bt/decision_subditosalreadygenerated.hpp"
#include "../utils/bt/decision_activateorquit_shield.hpp"
#include "../utils/bt/decision_readyforairattack.hpp"
struct Ia_man
{
    using jsonType = const rapidjson::Value&;

    // void createMap(EntityManager& em);
    void createEnemiesZone(EntityManager& em, uint16_t zone);
    bool checkEnemiesCreaeted(uint16_t zone);
    void createEnemies(EntityManager& em);
    void createEnemy(EntityManager& em, jsonType json);
    void resetVec();
    void createSubdito(EntityManager& em,double generate_radius);
    void createBossFinalFase2(EntityManager& em, const mapType& map);
    vec3d getRandomPosAroundBoss(double radio,const vec3d& spawnerPos);
private:
    bool createdzone2{ false }, createdzone3{ false }, createdzone4{ false },
        createdzone5{ false }, createdzone6{ false }, createdzone12{ false },
        createdzone11{ false };

    void createEnemiesZone2(EntityManager& em);
    void createEnemiesZone3(EntityManager& em);
    void createEnemiesZone4(EntityManager& em);
    void createEnemiesZone5(EntityManager& em);
    void createEnemiesZone6(EntityManager& em);
    void createEnemiesZone12(EntityManager& em);
    void createEnemiesZone11(EntityManager& em);

    void setCreatedtofalse(uint16_t z);

    std::vector<std::unique_ptr<BehaviourTree_t>> vec_t{};

};

#endif // !MAP
