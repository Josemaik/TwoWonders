#pragma once
#include <iostream>
#include <managers/eventmanager.hpp>

//class EventComponent
struct EventComponent
{
    void notify(const Event& event) {
        // Lógica para reaccionar al evento
        // Puedes hacer algo específico aquí, por ejemplo, imprimir un mensaje
        std::cout << "Entity notified about event with code: " << event.code << std::endl;
    }
};