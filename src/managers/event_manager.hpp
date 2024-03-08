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
    NoEvent,
    SpawnKey,
    SpawnDungeonKey,
    OpenChest,
    SetSpawn,
    OpenDoor,
    SpawnWallLevel0,
    ViewPointCave
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
                    case EventCodes::NoEvent:
                        break;
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
                        auto& cc = em.getComponent<ChestComponent>(chest);

                        os.addObject(cc.content, playerPos);
                        auto& msgs = cc.messages;
                        while (!msgs.empty())
                        {
                            txti.addText(msgs.front());
                            msgs.pop();
                        }

                        li.chestToOpen = li.max;

                        if (chest.hasComponent<DispatcherComponent>())
                        {
                            auto& dc = em.getComponent<DispatcherComponent>(chest);
                            auto& lc = em.getComponent<ListenerComponent>(e);
                            for (std::size_t i = 0; i < dc.eventCodes.size(); i++)
                            {
                                scheduleEvent(Event{ static_cast<EventCodes>(dc.eventCodes[i]) });
                                lc.addCode(static_cast<EventCodes>(dc.eventCodes[i]));
                            }
                        }

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
                    case EventCodes::SpawnWallLevel0:
                    {
                        auto& e{ em.newEntity() };
                        em.addTag<BarricadeTag>(e);
                        em.addTag<WallTag>(e);
                        auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = { -37.852, 7.0, 139.238 }, .scale = { 19.127, 10.0, 15.979 }, .color = DARKBROWN, .orientation = 90.0 * DEGTORAD, .rotationVec = { 0.0, 1.0, 0.0 } });
                        auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position = r.position, .scale = r.scale, .gravity = 0, .orientation = r.orientation, .rotationVec = r.rotationVec });
                        em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, p.scale, BehaviorType::STATIC });
                        break;
                    }
                    case EventCodes::ViewPointCave:
                    {
                        auto& li = em.getSingleton<LevelInfo>();
                        li.viewPoint = { -100.554, 4.935, 145.0 };
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
