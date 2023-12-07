#pragma once

#include <cstdint>

struct InformationComponent{
    uint16_t coins{}, bombs{};

    void addCoin(){ coins += 1; }
    void addBomb(){ bombs += 1; }
};