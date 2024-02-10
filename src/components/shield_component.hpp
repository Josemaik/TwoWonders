#pragma once
#include <cstdint>

enum class EntitieswithShield : uint8_t { Player, Subdito};

struct ShieldComponent{
    bool activeShield{ true };
    EntitieswithShield createdby{};
    std::size_t id_subdito{};
};