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
    NoEvent = 0x00,
    SpawnKey = 0x01,
    SpawnDungeonKey = 0x02,
    OpenChest = 0x04,
};

struct EventManager
{
public:
    // Programa un evento para ser despachado tan pronto como sea posible
    void scheduleEvent(const Event& event) {
        events.push_back(event);
    }

    // Dispara todos los eventos pendientes
    void dispatchEvents(EntityManager& em, ObjectSystem& os) {
        // Recorre todos los eventos pendientes
        while (!events.empty()) {
            // Obtiene el siguiente evento y lo elimina de la cola
            Event& event = events.back();
            events.pop_back();

            using CMPs = MP::TypeList<ListenerComponent>;
            using noTag = MP::TypeList<>;

            // Notifica a todos los listeners que estén interesados en el evento
            em.forEach<CMPs, noTag>([&](Entity& e, ListenerComponent& lc)
            {
                if (lc.hasCode(event.code))
                {
                    switch (event.code)
                    {
                    case EventCodes::SpawnKey:
                        break;

                    case EventCodes::SpawnDungeonKey:
                    {
                        auto& li = em.getSingleton<LevelInfo>();
                        vec3d pos{ 83.0, 0.0, -71.0 };
                        os.addObject(ObjectType::Key, pos);
                        li.dungeonKeyCreated = true;
                        break;
                    }
                    case EventCodes::OpenChest:
                    {
                        auto& li = em.getSingleton<LevelInfo>();
                        auto& playerPos = em.getComponent<PhysicsComponent>(e).position;
                        auto& chest = *em.getEntityByID(li.chestToOpen);
                        auto& chestComp = em.getComponent<ChestComponent>(chest);

                        os.addObject(chestComp.content, playerPos);
                        li.chestToOpen = li.max;
                        break;
                    }
                    }
                }
            });
        }
    }

    void reset() { events.clear(); }

private:
    // Cola de eventos pendientes
    std::vector<Event> events;
};

#endif
