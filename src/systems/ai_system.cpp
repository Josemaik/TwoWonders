#include "ai_system.hpp"
#include <cmath>
#include <random>

// Cada cuanto se percibe al jugador
void perception(BlackBoard_t& bb, AIComponent& ai, float dt) {
    // Accumulate delta time still perception time
    // ai.accumulated_dt += dt;
    // if (ai.accumulated_dt <= ai.perceptionTime) return;
    //Perception time reached
    // ai.accumulated_dt -= ai.perceptionTime;
    // Al pulsar la G , la ia con seek va a la posición del player
    if (ai.elapsed_perception >= ai.countdown_perception) {
        ai.elapsed_perception = 0;
        if (bb.tactive) {
            ai.tx = bb.tx;
            ai.tz = bb.tz;
            ai.tactive = true;
            ai.teid = bb.teid;
            // ai.behaviour = bb.behaviour;
            bb.tactive = false;
            // ai.pathIt = bb.path.begin();
            //  id {static_cast<int>(e.getID()) };
        }
    }
    else {
        ai.plusdeltatime(dt, ai.elapsed_perception);
    }
}
// Actualizar las IA
void AISystem::update(EntityManager& em, float dt)
{
    bool isDetected{ false };
    std::vector<vec3d> enemyPositions{};
    auto& li = em.getSingleton<LevelInfo>();
    auto& bb = em.getSingleton<BlackBoard_t>();
    bb.idsubditos.clear();

    em.forEach<SYSCMPs, SYSTAGs>([&, dt](Entity& e, PhysicsComponent& phy, RenderComponent& ren, AIComponent& ai, LifeComponent& lc)
    {
        //percibir el entorno
        perception(bb, ai, dt);
        // Actualizar datos de los slimes y subditos en blackboard
        if (e.hasTag<SlimeTag>()) {
            bb.updateInfoSlime(e.getID(), phy.position, lc.life);
        }
        if (e.hasTag<SubjectTag>() && e.hasComponent<SubjectComponent>()) {
            auto& sub = em.getComponent<SubjectComponent>(e);
            bb.updateInfoSub(e.getID(), phy.position, lc.life, sub.activeShield);
        }
        if (e.hasTag<BossFinalTag>()) {
            bb.updateInfoBoss(phy.position);
        }
        //visual debug cone
        if(e.hasTag<SpiderTag>()){
            bb.pos_enemy = phy.position;
            bb.orientation_enemy = phy.orientation;
            bb.horizontalFOV = 200.0;
            bb.VerticalFOV = 80.0;
        }

        if (!isDetected && ai.playerdetected)
        {
            li.playerDetected = true;
            isDetected = true;
        }

        if (ai.playerdetected)
            enemyPositions.push_back(phy.position);

        if (ai.behaviourTree) {
            ai.behaviourTree->run({ em,e,ai,phy,ren,lc,dt });
            return;
        }
    });

    if (!bb.idsubditos.empty())
    {
        std::vector<std::size_t> keysToRemove;

        for (const auto& s : bb.subditosData) {
            bool remove = true;
            for (const auto& id : bb.idsubditos) {
                if (s.first == id) {
                    remove = false;
                    break;
                }
            }
            if (remove) {
                keysToRemove.push_back(s.first);
            }
        }

        for (const auto& key : keysToRemove) {
            bb.subditosData.erase(key);
        }
    }
    else if (!bb.subditosData.empty())
        bb.subditosData.clear();

    if (!isDetected && li.playerDetected)
        li.playerDetected = false;


    li.enemyPositions = enemyPositions;
}
