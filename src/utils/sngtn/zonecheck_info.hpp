#pragma once
#include <unordered_set>
#include <cstdint>
#include "../../components/interactive_component.hpp"

struct ZoneCheckInfo
{
    void insertZone(uint8_t zone, InteractableType type)
    {
        switch (type)
        {
        case InteractableType::Chest:
            chestZones.insert(zone);
            break;
        case InteractableType::Door:
            doorZones.insert(zone);
            break;
        case InteractableType::Lever:
            leverZones.insert(zone);
            break;
        case InteractableType::Ladder:
            ladderZones.insert(zone);
            break;
        case InteractableType::NPC:
            npcZones.insert(zone);
            break;
        default:
            break;
        }
    }
    void clearSets() { chestZones.clear(); leverZones.clear(); doorZones.clear(); npcZones.clear(); ladderZones.clear(); }

    const std::unordered_set<uint8_t>& getChestZones() const { return chestZones; }
    const std::unordered_set<uint8_t>& getLeverZones() const { return leverZones; }
    const std::unordered_set<uint8_t>& getDoorZones() const { return doorZones; }
    const std::unordered_set<uint8_t>& getNpcZones() const { return npcZones; }
    const std::unordered_set<uint8_t>& getLadderZones() const { return ladderZones; }

private:
    std::unordered_set<uint8_t> chestZones{};
    std::unordered_set<uint8_t> leverZones{};
    std::unordered_set<uint8_t> doorZones{};
    std::unordered_set<uint8_t> npcZones{};
    std::unordered_set<uint8_t> ladderZones{};
};