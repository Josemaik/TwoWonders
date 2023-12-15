#pragma once
#include <cstdint>

enum struct Direction_Shield { Up, Down, Left, Right };

struct ShieldComponent{
    bool activeShield{ true };
    Direction_Shield direction{ Direction_Shield::Up };

    std::size_t shield; // Referencia a la entidad escudo
};