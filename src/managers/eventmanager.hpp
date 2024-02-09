#pragma once
#ifndef EVENT_MANAGER
#define EVENT_MANAGER

#include "../systems/object_system.hpp"
#include "../utils/types.hpp"
#include <cstdint>
#include <vector>

struct Event {
    uint16_t code; // Código identificador del evento
};

//Eventos
enum EventCodes
{
    SpawnKey = 0x01,
    OpenChest = 0x02,
};

// Clase ListenerRegistration para almacenar datos sobre un listener registrado
struct ListenerRegistration {

    uint16_t interestCode;
    const Entity* listener;

    ListenerRegistration(uint16_t code, const Entity& entity)
        : interestCode(code), listener(&entity) {}

    // Constructor de copia
    ListenerRegistration(const ListenerRegistration& other)
        : interestCode(other.interestCode), listener(other.listener) {}

    // Operador de asignación de movimiento
    ListenerRegistration& operator=(ListenerRegistration&& other) noexcept {
        if (this != &other) {
            interestCode = other.interestCode;
            listener = other.listener;
        }
        return *this;
    }
};

struct Eventmanager
{
public:
    // Programa un evento para ser despachado tan pronto como sea posible
    void scheduleEvent(const Event& event) {
        events.push_back(event);
    }

    // Registra un listener en el gestor de eventos
    void registerListener(const Entity& listener, uint16_t code)
    {
        ListenerRegistration lr(code, listener);
        listeners.push_back(lr);
    }

    // Quita un listener del gestor de eventos
    void unregisterListener(const Entity& listener, uint16_t code)
    {
        std::vector<decltype(listeners)::iterator> toRemove{};
        for (auto it = listeners.begin(); it != listeners.end(); ++it) {
            if (it->listener == &listener && it->interestCode == code) {
                toRemove.push_back(it);
                break;
            }
        }

        for (auto& lr : toRemove) {
            listeners.erase(lr);
        }
    }

    // Dispara todos los eventos pendientes
    void dispatchEvents(EntityManager& em, ObjectSystem& os) {
        // Recorre todos los eventos pendientes
        while (!events.empty()) {
            // Obtiene el siguiente evento y lo elimina de la cola
            Event& event = events.back();
            events.pop_back();

            // Notifica a cada listener interesado en el evento
            for (const auto& lr : listeners) {
                if (lr.interestCode & event.code) {
                    switch (event.code)
                    {
                    case EventCodes::SpawnKey:
                        // Llama a la función de callback del listener
                        // em.getComponent<EventComponent>(lr.listener).onSpawnKey();
                        break;
                    case EventCodes::OpenChest:
                    {
                        auto& li = em.getSingleton<LevelInfo>();
                        auto& playerPos = em.getComponent<PhysicsComponent>(*em.getEntityByID(li.playerID)).position;
                        auto& chest = *em.getEntityByID(li.chestToOpen);
                        auto& chestComp = em.getComponent<ChestComponent>(chest);

                        os.addObject(chestComp.content, playerPos);
                        li.chestToOpen = li.max;
                        break;
                    }
                    }

                    continue;
                }
            }
        }
    }

    void reset()
    {
        listeners.clear();
        events.clear();
    }

private:
    // Lista de registros de listeners
    std::vector<ListenerRegistration> listeners;
    // Cola de eventos pendientes
    std::vector<Event> events;
};

#endif
