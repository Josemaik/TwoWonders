#pragma once
#include <cstdint>

struct ShieldComponent{
    bool activeShield{ true };
    std::size_t shield; // ID de la entidad escudo
};