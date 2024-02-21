#pragma once
#ifndef EVENT_MANAGER
#define EVENT_MANAGER

#include "../systems/object_system.hpp"
#include "map_manager.hpp"
#include "../utils/types.hpp"
#include <cstdint>
#include <vector>

//Eventos
enum EventCodes : uint16_t
{
    SpawnKey,
    SpawnDungeonKey,
    OpenChest,
    SetSpawn,
    OpenDoor,
};

struct Event {
    EventCodes code; // Código identificador del evento
};

struct EventManager
{
public:
    // Programa un evento para ser despachado tan pronto como sea posible
    void scheduleEvent(const Event& event) {
        events.push_back(event);
    }

    // Dispara todos los eventos pendientes
    void dispatchEvents(EntityManager& em, MapManager& mm, Ia_man& iam, ObjectSystem& os) {
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
                        auto& txti = em.getSingleton<TextInfo>();
                        auto& playerPos = em.getComponent<PhysicsComponent>(e).position;
                        auto& chest = *em.getEntityByID(li.chestToOpen);
                        auto& chestComp = em.getComponent<ChestComponent>(chest);

                        os.addObject(chestComp.content, playerPos);
                        txti.addText(chestComp.message);
                        li.chestToOpen = li.max;
                        break;
                    }
                    case EventCodes::SetSpawn:
                    {
                        auto& plfi = em.getSingleton<PlayerInfo>();
                        auto& playerPos = em.getComponent<PhysicsComponent>(e).position;
                        plfi.spawnPoint = playerPos;
                        auto& life = em.getComponent<LifeComponent>(e);
                        life.life = life.maxLife;
                        plfi.mana = plfi.max_mana - 3.0;

                        mm.spawnReset(em, iam);

                        break;
                    }
                    case EventCodes::OpenDoor:
                    {
                        auto& li = em.getSingleton<LevelInfo>();
                        auto& plfi = em.getSingleton<PlayerInfo>();

                        plfi.hasKey = false;
                        li.dead_entities.insert(li.doorToOpen);
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
