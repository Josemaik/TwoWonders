#include "ai_system.hpp"
#include <cmath>
#include <random>

// Cada cuanto se percibe al jugador
void AISystem::perception(BlackBoard_t& bb, AIComponent& ai) {
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
        ai.plusDeltatime(timeStep, ai.elapsed_perception);
    }
}
// Actualizar las IA
void AISystem::update(EntityManager& em)
{
    bool isDetected{ false };
    std::vector<vec3d> enemyPositions{};
    auto& li = em.getSingleton<LevelInfo>();
    auto& bb = em.getSingleton<BlackBoard_t>();
    bb.idsubditos.clear();

    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, PhysicsComponent& phy, RenderComponent& ren, AIComponent& ai, LifeComponent& lc)
    {
        //Ptr a ai y lc
        AIComponent* aiptr = &ai;
        LifeComponent* lcptr = &lc;
        //percibir el entorno
        perception(bb, ai);

        //Actualizar posiciones de la IAs o potenciales targets para calcular Flocking
        // Comprobamos si el elemento debe procesarse
        // Más alante hacer que se cumpla si esta a una cierta distancia del player
        if(e.hasTag<SnowmanTag>()){
            //Si el vector esta vacío, el elemento se inserta
            bool id_found = false;
            // Iterar sobre el vector positions
            if(bb.potencial_targets.size() != 0){
                for(auto it = bb.potencial_targets.begin(); it != bb.potencial_targets.end(); ++it) {
                    // Si ya existe ele elemento , se comprueba si esta detectando al player
                    if(it->first == e.getID()) {
                        // si lo detecta, actualizamos posición y sino, lo borramos del vector
                        if(ai.playerdetected){
                            // Actualizar la posición
                            it->second = phy.position;
                            // Marcar que se ha encontrado el ID
                            id_found = true;
                        }else{
                            bb.potencial_targets.erase(it);
                        }
                        // Salir del bucle
                        break;
                    }
                }
            }
            // Si no se ha encontrado el ID en el vector positions, añadir un nuevo par
            if(!id_found && ai.playerdetected) {
                bb.potencial_targets.push_back(std::make_pair(e.getID(), phy.position));
            }
        }

        // Actualizar datos de los slimes , subditos y boss en blackboard
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
        if (e.hasTag<SpiderTag>() && e.hasTag<SnowmanTag>()) {
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
            ai.behaviourTree->run({ em,e,aiptr,nullptr,phy,ren,lcptr, timeStep });
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
