#pragma once

#include <cstdint>

struct PlayerInfo {
    uint16_t coins{}, bombs{}, max_bombs{ 8 };
    bool hasKey{ false };

    void addCoin() { coins += 5; }
    void addKey() { hasKey = true; }
    void add30Coins() { coins += 30; }
    void addBomb() {
        if (bombs < max_bombs)
            bombs += 3;

        if (bombs > max_bombs)
            bombs = max_bombs;
    }

    bool decreaseCoins(uint16_t decrease) {
        if (decrease <= coins) {
            coins -= decrease;
            return true;
        }
        return false;
    }

    bool buyBomb() {
        if (decreaseCoins(20)) {
            addBomb();
            return true;
        }
        return false;
    }

    void decreaseBomb() { if (bombs > 0) bombs -= 1; }
};