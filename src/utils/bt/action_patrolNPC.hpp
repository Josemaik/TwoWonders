#pragma once
#include "node.hpp"
#include <random>
#include <cmath>
#include <numbers>
#include <algorithm>
#include <utils/sb/steeringbehaviour.hpp>

struct BTAction_PatrolNPC : BTNode_t {
    template <size_t N>
    bool calculateSteering(EntityContext_t& ectx, typename Path_t<N>::iterator& pathIt, Path_t<N>& path) noexcept {
        Steer_t steering = STBH::Arrive(ectx.phy, *pathIt, ectx.npc->arrival_radius);
        if (steering.arrived) {
            if (*pathIt == *path.end()) {
                ectx.npc->path_finaliced = true;
                return true;
            }
            else {
                ++pathIt;
            }
        }
        ectx.phy.velocity = vec3d{ steering.v_x, 0.0, steering.v_z };
        return false;
    }


    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        //ectx.ai->bh = "patrolling";
        if (!ectx.npc->path_initialized) {
            ectx.npc->pathIt = ectx.npc->path.begin();
            ectx.npc->path_initialized = true;
        }

        //Do patrol
        auto& li = ectx.em.getSingleton<LevelInfo>();
        if (ectx.ent.hasTag<NomadTag>()) {
            if (!ectx.npc->path_finaliced && (li.door_open || ectx.npc->tp)) {
                if (calculateSteering<4>(ectx, ectx.npc->pathIt, ectx.npc->path)) {
                    ectx.npc->tp = false;
                    ectx.npc->path_finaliced = true;

                    if (li.mapID == 2)
                        addDialogEvent(ectx, 21);
                }
            }
        }
        if (ectx.ent.hasTag<InvestigatorTag>()) {
            if (li.investigatorstartwalk && !ectx.npc->path_investigador_finalized) {
                if (calculateSteering<7>(ectx, ectx.npc->pathIt_inestigador, ectx.npc->path_investigador)) {
                    ectx.npc->path_investigador_finalized = true;

                    if (li.mapID == 2)
                        addDialogEvent(ectx, 19);
                }
            }
        }
        return BTNodeStatus_t::success;
    }

    void addDialogEvent(EntityContext_t& ectx, int event) noexcept
    {
        auto& zchi = ectx.em.getSingleton<ZoneCheckInfo>();
        auto& c = ectx.em.getComponent<ColliderComponent>(ectx.ent);
        for (auto [num, bbox] : zchi.zoneBounds)
        {
            if (bbox.intersects(c.boundingBox))
            {
                zchi.insertZone(num, InteractableType::NPC);
            }
        }

        auto& li = ectx.em.getSingleton<LevelInfo>();
        auto& ouc = ectx.em.getComponent<OneUseComponent>(ectx.ent);
        auto& dc = ectx.em.getComponent<DispatcherComponent>(ectx.ent);
        dc.eventCodes.push_back(event);

        std::pair<uint8_t, uint8_t> pair = { li.mapID, ouc.id };
        if (li.dontLoad.find(pair) != li.dontLoad.end())
            li.dontLoad.erase(pair);
    }
};
