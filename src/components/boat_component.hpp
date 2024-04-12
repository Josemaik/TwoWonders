#pragma once
#include <cstdint>

enum struct BoatParts : uint8_t
{
    Propeller,
    Motor,
    SteeringWheel,
    Base,
};

struct BoatComponent
{
    void setPart() { part = static_cast<BoatParts>(nextPart++); }
    BoatParts part{};

private:
    static uint8_t nextPart;
};