#pragma once

#include <cstdint>

// //class EventComponent
struct EventComponent
{
    void notify(uint16_t evento) {
        // Lógica para reaccionar al evento
        std::cout << "Entity notified about event with code: " << evento << std::endl;
    }
};