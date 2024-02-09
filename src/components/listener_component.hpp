#pragma once
#include <cstdint>

struct ListenerComponent
{
    // Función para añadir un código de evento
    void addCode(uint16_t code) { codeMask |= static_cast<uint16_t>(1u << code); }

    // Función para quitar un código de evento
    void removeCode(uint16_t code) { codeMask &= static_cast<uint16_t>(~(1u << code)); }

    // Máscara para añadir códigos en binario
    uint16_t codeMask{};
};