#include "ai_system.hpp"
#include <cmath>
#include <random>

// Cada cuanto se percibe al jugador
void AISystem::perception(BlackBoard_t& bb, AIComponent& ai) {

    if (ai.elapsed_perception >= ai.countdown_perception) {
        ai.elapsed_perception = 0;
        ai.tx = bb.tx;
        ai.tz = bb.tz;
        ai.tactive = true;
        ai.teid = bb.teid;
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
        if (e.hasTag<EnemyDeathTag>())
            return;
        //Ptr a ai y lc
        AIComponent* aiptr = &ai;
        LifeComponent* lcptr = &lc;
        //percibir el entorno
        if (e.hasTag<SnowmanTag>() || e.hasTag<GolemTag>() || e.hasTag<SlimeTag>()) {
            perception(bb, ai);
            
            //guardar potencialees targets para calcular el flocking
            //Si el vector esta vacío, el elemento se inserta
            bool id_found = false;
            // Iterar sobre el vector positions
            if (bb.potencial_targets.size() != 0) {
                for (auto it = bb.potencial_targets.begin(); it != bb.potencial_targets.end(); ++it) {
                    // Si ya existe ele elemento , se comprueba si esta detectando al player
                    if (it->first == e.getID()) {
                        // si lo detecta, actualizamos posición y sino, lo borramos del vector
                        if (ai.playerdetected) {
                            // Actualizar la posición
                            it->second = phy.position;
                            // Marcar que se ha encontrado el ID
                            id_found = true;
                        }
                        else {
                            bb.potencial_targets.erase(it);
                        }
                        // Salir del bucle
                        break;
                    }
                }
            }
            // Si no se ha encontrado el ID en el vector positions, añadir un nuevo par
            if (!id_found && ai.playerdetected) {
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
        if (e.hasTag<SnowmanTag>()) {
            bb.conesnow.first = phy.position;
            bb.conesnow.second = phy.orientation;
        }
        if (e.hasTag<GolemTag>()) {
            bb.conegolem.first = phy.position;
            bb.conegolem.second = phy.orientation;
        }
        if (e.hasTag<SpiderTag>()) {
            bb.conespider.first = phy.position;
            bb.conespider.second = phy.orientation;
        }

        if (!isDetected && ai.playerdetected)
        {
            li.playerDetected = true;
            isDetected = true;
        }
        //Si está detectando al player, guardamos su poisición
        if (ai.playerdetected)
            enemyPositions.push_back(phy.position);
        
        //Ejecutar behaviour tree
        if (ai.behaviourTree) {
            ai.behaviourTree->run({ em,e,aiptr,nullptr,phy,ren,lcptr, timeStep });
            return;
        }
    });

    // Boss Final
    //Manejo del borrado de súbditos
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

    // guardar posiciones de los enemigos detectados
    li.enemyPositions = enemyPositions;
}
