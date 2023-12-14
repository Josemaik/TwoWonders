#pragma once

#include <cstdint>

struct InformationComponent {
    uint16_t coins{}, bombs{}, max_bombs{ 8 };

    void addCoin() { coins += 1; }
    void add30Coins() { coins += 30; }
    void addBomb() {
        if (bombs < max_bombs)
            bombs += 3;

        if (bombs > max_bombs)
            bombs = max_bombs;
    }

    void decreaseBomb() { if (bombs > 0) bombs -= 1; }
};