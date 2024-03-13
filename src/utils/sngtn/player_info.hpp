#pragma once
#include <cstdint>
#include <array>
#include <limits>
#include <memory>
#include "../Item.hpp"

struct PlayerInfo
{
    static constexpr std::size_t max = std::numeric_limits<std::size_t>::max();

    double increaseLife{ 0.0 };
    uint16_t coins{}, bombs{}, max_bombs{ 8 };
    float elapsed_limit_coins{ 5.0f }, elapsed_coins{ elapsed_limit_coins };
    double max_mana{ 100.0 }, mana{ max_mana };
    int mana_width{}, armor{};
    bool hasKey{ false };
    bool hasStaff{ false };
    std::vector<std::unique_ptr<Item>> inventory{};
    std::vector<Spell> spells{};
    Spell currentSpell{ "None", "No spell", Spells::None, 0.0, 0 };
    std::size_t selectedItem{ max };
    bool isDead{ false };
    vec3d spawnPoint{};

    void addSpell(Spell spell) { spells.push_back(spell); currentSpell = spell; }
    void changeCurrentSpell()
    {
        auto it = std::find(spells.begin(), spells.end(), currentSpell);
        if (it != spells.end())
        {
            if (it + 1 != spells.end())
                currentSpell = *(it + 1);
            else
                currentSpell = spells.front();
        }
    }

    void addItem(std::unique_ptr<Item> item) { inventory.push_back(std::move(item)); }
    void removeItem(const std::size_t& id) {
        auto it = std::find_if(inventory.begin(), inventory.end(), [id](const std::unique_ptr<Item>& item) {
            return item->getID() == id;
        });
        if (it != inventory.end())
            inventory.erase(it);
    }

    Item* getItem(const std::size_t& id) {
        auto it = std::find_if(inventory.begin(), inventory.end(), [id](const std::unique_ptr<Item>& item) {
            return item->getID() == id;
        });
        return it->get();
    }

    void addCoin(uint16_t add)
    {
        coins += add;
        elapsed_coins = 0.0f;
    }
    void addKey() { hasKey = true; }
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

    void usePotion(Potion potion)
    {
        switch (potion.type)
        {
        case PotionType::Mana:
            mana += potion.value;
            if (mana > max_mana)
                mana = max_mana;
            break;
        case PotionType::Health:
            increaseLife = potion.value;
            break;
        }

        removeItem(potion.getID());
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