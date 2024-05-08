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
#include "../utils/bt/action_pendulum.hpp"
#include "../utils/bt/actiion_evade.hpp"
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
#include "../utils/bt/decision_readyforcrusher.hpp"
struct Ia_man
{
    using jsonType = const rapidjson::Value&;

    void createEnemy(EntityManager& em, jsonType json);
    void resetVec();
    void createSubdito(EntityManager& em,double generate_radius);
    void createBossFinalFase2(EntityManager& em, const mapType& map);
    vec3d getRandomPosAroundBoss(double radio,const vec3d& spawnerPos);
private:
    std::vector<std::unique_ptr<BehaviourTree_t>> vec_t{};

};

#endif // !MAP
