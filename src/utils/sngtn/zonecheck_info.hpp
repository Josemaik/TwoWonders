#pragma once
#include <unordered_set>
#include <unordered_map>
#include <cstdint>
#include "../../components/interactive_component.hpp"

struct ZoneCheckInfo
{
    void insertZone(uint8_t zone, InteractableType type)
    {
        zones[type].insert(zone);
    }

    void clearSets()
    {
        for (auto& pair : zones)
        {
            pair.second.clear();
        }
    }

    const std::unordered_set<uint8_t>& getZones(InteractableType type) const
    {
        // If the type is not found, return 
        if (zones.find(type) == zones.end())
        {
            return emptySet;
        }
        return zones.at(type);
    }

private:
    std::unordered_map<InteractableType, std::unordered_set<uint8_t>> zones;
    const std::unordered_set<uint8_t> emptySet;
};