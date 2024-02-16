#pragma once
#include <cstdint>
#include <array>
#include "../Item.hpp"

struct PlayerInfo {
    uint16_t coins{}, bombs{}, max_bombs{ 8 };
    double max_mana{ 100.0 }, mana{ max_mana };
    int mana_width{};
    bool hasKey{ false };
    std::vector<Item> inventory{};
    std::vector<Spell> spells{};
    Spells currentSpell{};
    bool isDead{ false };
    vec3d spawnPoint{};
    uint8_t type{ 1 };
    std::array<uint8_t, 4> types{ 1, 1, 1, 1 };

    void addSpell(Spell spell) { spells.push_back(spell); currentSpell = spell.spell; }
    void changeCurrentSpell()
    {
        auto it = std::find(spells.begin(), spells.end(), currentSpell);
        if (it != spells.end())
        {
            if (it + 1 != spells.end())
                currentSpell = (it + 1)->spell;
            else
                currentSpell = spells.front().spell;
        }
    }
    void addItem(Item item) { inventory.push_back(item); }
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

    void reset()
    {
        coins = 0;
        bombs = 0;
        max_mana = 100.0;
        mana = max_mana;
        mana_width = 0;
        hasKey = false;
    }
};