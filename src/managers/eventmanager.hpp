#pragma once
#ifndef EVENT_MANAGER
#define EVENT_MANAGER

#include "../utils/types.hpp"
#include <cstdint>
#include <vector>


//Eventos
static const uint16_t EVENT_CODE_CHANGE_ZONE = 1;


struct Event {
    uint16_t code; // Código identificador del evento
};

// Clase ListenerRegistration para almacenar datos sobre un listener registrado
struct ListenerRegistration {

    uint16_t interestCode;
    const Entity& listener;

    ListenerRegistration(uint16_t code, const Entity& entity)
        : interestCode(code), listener(entity) {}
};

struct Eventmanager
{
public:
    // Programa un evento para ser despachado tan pronto como sea posible
    void scheduleEvent(const Event& event) {
        events.push_back(event);
    }

    // Registra un listener en el gestor de eventos
    void registerListener(const Entity& listener, uint16_t code) {
        ListenerRegistration lr(code, listener);
        listeners.push_back(lr);
    }

    // Dispara todos los eventos pendientes
    void dispatchEvents(EntityManager& em) {
        // Recorre todos los eventos pendientes
        while (!events.empty()) {
            // Obtiene el siguiente evento y lo elimina de la cola
            Event& event = events.back();
            events.pop_back();

            // Notifica a cada listener interesado en el evento
            for (const auto& lr : listeners) {
                if (lr.interestCode == event.code) {
                    if (event.code == 1) {
                        em.getSingleton<LevelInfo>().drawzone = true;
                    }
                }
            }
        }
    }

private:
    // Lista de registros de listeners
    std::vector<ListenerRegistration> listeners;
    // Cola de eventos pendientes
    std::vector<Event> events;
};

#endif
