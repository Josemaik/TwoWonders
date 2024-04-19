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
    void setPart(uint8_t& num) { part = static_cast<BoatParts>(num++); }
    BoatParts part{};
};