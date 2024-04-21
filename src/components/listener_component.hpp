#pragma once
#include <cstdint>
#include <unordered_set>

struct ListenerComponent
{
    // Función para añadir un código de evento
    void addCode(uint16_t code) { codeSet.insert(code); }

    // Función para quitar un código de evento
    void removeCode(uint16_t code) { codeSet.erase(code); }

    // Función para comprobar si un código de evento está registrado
    bool hasCode(uint16_t code) const { return codeSet.find(code) != codeSet.end(); }

private:
    // Set para añadir códigos
    std::unordered_set<uint16_t> codeSet;
};