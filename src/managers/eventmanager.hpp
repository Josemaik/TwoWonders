#pragma once
#ifndef EVENT_MANAGER
#define EVENT_MANAGER

#include <cstdint>
#include <vector>

struct Event {
    uint16_t code; // Código identificador del evento
};

// Interfaz EventListener
struct EventListener {
public:
    virtual void notify(const Event& event) = 0;
};

// Clase ListenerRegistration para almacenar datos sobre un listener registrado
struct ListenerRegistration {
public:
    int interestCode;
    EventListener* listener;
};

struct Eventmanager
{
public:
    // Verifica nuevos eventos y los agrega a la cola
    void checkForEvents() {
         // Detectar eventos de entrada del usuario
        //Event userInputEvent = detectUserInput();

        // Programar el evento detectado en la cola de eventos
        // if (userInputEvent.code != 0) {
        //     scheduleEvent(userInputEvent);
        // }
    }

    // Programa un evento para ser despachado tan pronto como sea posible
    void scheduleEvent(const Event& event) {
        events.push_back(event);
    }

    // Registra un listener en el gestor de eventos
    void registerListener(EventListener* listener, uint16_t code) {
        ListenerRegistration lr;
        lr.listener = listener;
        lr.interestCode = code;
        listeners.push_back(lr);
    }

    // Dispara todos los eventos pendientes
    void dispatchEvents() {
        // Recorre todos los eventos pendientes
        while (!events.empty()) {
            // Obtiene el siguiente evento y lo elimina de la cola
            Event& event = events.back();
            events.pop_back();

            // Notifica a cada listener interesado en el evento
            for (const auto& lr : listeners) {
                if (lr.interestCode == event.code) {
                    lr.listener->notify(event);
                }
            }
        }
    }

    // Ejecuta el gestor de eventos
    void run() {
        checkForEvents();
        dispatchEvents();
    }
private:
    // Lista de registros de listeners
    std::vector<ListenerRegistration> listeners;
    // Cola de eventos pendientes
    std::vector<Event> events;
};
