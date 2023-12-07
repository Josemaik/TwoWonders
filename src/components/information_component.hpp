#pragma once

#include <cstdint>

struct InformationComponent{
    uint16_t coins{}, bombs{}, max_bombs{ 8 };

    void addCoin(){ coins += 1; }
    void addBomb(){ if(bombs < max_bombs) bombs += 1; }
};