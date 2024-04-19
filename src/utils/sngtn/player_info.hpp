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
    uint16_t coins{}, coinsAdded{}, bombs{}, max_bombs{ 8 };
    float elapsed_limit_coins{ 5.0f }, elapsed_coins{ elapsed_limit_coins };
    double max_mana{ 100.0 }, mana{ max_mana };
    int mana_width{}, max_armor{ 4 }, armor{};
    bool hasKey{ false };
    bool hasStaff{ false };
    bool hasBoots{ false };
    bool hasHat{ false };
    bool onSpawn{ false };
    bool minusCoins{ false };
    bool onLadder{ false };
    bool showBook{ false };
    bool attackUpgrade{ false };
    std::vector<std::unique_ptr<Item>> inventory{};
    std::vector<Spell> spells{};
    Spell noSpell{ "None", "No spell", Spells::None, 0.0, 0 };
    Spell currentSpell{ noSpell };
    std::array<Spell, 3> spellSlots{ noSpell, noSpell, noSpell };
    std::size_t selectedItem{ max };
    vec3d spawnPoint{};
    std::vector<BoatParts> boatParts{};

    void addSpell(Spell spell)
    {
        spells.push_back(spell);

        bool noCurrent{ false };
        for (auto& slot : spellSlots)
        {
            if (slot == noSpell)
            {
                slot = spell;
                noCurrent = true;
            }

            if (noCurrent)
                break;
        }

        if (!noCurrent)
            spellSlots[0] = spell;
    }

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
        coinsAdded = add;
        elapsed_coins = 0.0f;
    }

    void coinDeath()
    {
        auto prev = coins;
        coins = 0;

        // Hacemos la división y si tiene resto se lo sumamos
        uint16_t rest = prev % 2;
        addCoin((prev / 2) + rest);
        minusCoins = true;
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

    void onDeath()
    {
        mana = max_mana;
        armor = 0;
        coinDeath();
    }

    void reset()
    {
        currentSpell = { "None", "No spell", Spells::None, 0.0, 0 };
        selectedItem = max;
        inventory.clear();
        spells.clear();
        increaseLife = 0.0;
        armor = 0;
        coins = 0;
        bombs = 0;
        max_mana = 100.0;
        mana = max_mana;
        mana_width = 0;
        onLadder = false;
        hasStaff = false;
        hasBoots = true;
        hasHat = false;
        hasKey = false;
        onSpawn = false;
        showBook = false;
        attackUpgrade = false;
        boatParts.clear();
        spawnPoint = { 33.0, 4.0, -25.9 };
    }
};