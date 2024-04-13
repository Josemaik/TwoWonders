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
    ViewPointCave,
    NPCDialog,
    DialogPrisonNomad1,
    DialogPrisonNomad2,
    DialogFirstSpawn,
    ViewPointDoor,
    BoatPartFound,
    BoatDialog,
    DialogNomadVolcano1,
    DialogNomadVolcano2,
    DialogCatVolcano1,
    InitBoatParts,
    MAX
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
    void dispatchEvents(EntityManager& em, MapManager& mm, Ia_man& iam, ObjectSystem& os, SoundSystem& ss) {
        // Recorre todos los eventos pendientes
        bool out = false;
        while (!events.empty() && !out) {
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

                        ss.sonido_abrir_cofre();

                        os.addObject(cc.content, playerPos);

                        auto& msc = em.getComponent<MessageComponent>(chest);
                        auto& msgs = msc.messages;
                        while (!msgs.empty())
                        {
                            txti.addText(msgs.front());
                            msgs.pop();
                        }

                        li.chestToOpen = li.max;
                        li.openChest = true;

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
                        mm.spawnReset(em, iam);
                        break;
                    }
                    case EventCodes::OpenDoor:
                    {
                        auto& li = em.getSingleton<LevelInfo>();
                        auto& plfi = em.getSingleton<PlayerInfo>();

                        ss.sonido_abrir_puerta();

                        plfi.hasKey = false;
                        li.insertDeath(li.doorToOpen);
                        break;
                    }
                    case EventCodes::SpawnWallLevel0:
                    {
                        auto& e{ em.newEntity() };
                        em.addTag<BarricadeTag>(e);
                        em.addTag<WallTag>(e);
                        em.addTag<SeparateModelTag>(e);
                        auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = vec3d::zero(), .scale = vec3d::zero(), .color = DARKBROWN, .orientation = 90.0 * DEGTORAD, .rotationVec = { 0.0, -1.0, 0.0 } });
                        auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position = { -37.852, 7.0, 139.238 }, .scale = { 19.127, 10.0, 15.979 }, .gravity = 0, .orientation = r.orientation, .rotationVec = r.rotationVec });
                        em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, p.scale, BehaviorType::STATIC });
                        break;
                    }
                    case EventCodes::ViewPointCave:
                    {
                        auto& li = em.getSingleton<LevelInfo>();
                        li.viewPoint = { -100.554, 4.935, 145.0 };
                        li.viewPointSound = true;
                        break;
                    }
                    case EventCodes::DialogFirstSpawn:
                    {
                        auto& txti = em.getSingleton<TextInfo>();

                        std::array<std::string, 2> msgs =
                        {
                            "Este parece un buen lugar para descansar...",
                            "Te sientes revitalizado."
                        };

                        // Metemos el texto en el stack de texto
                        for (std::size_t i = 0; i < msgs.size(); i++)
                            txti.addText(msgs[i]);
                        break;
                    }
                    case EventCodes::ViewPointDoor:
                    {
                        auto& li = em.getSingleton<LevelInfo>();
                        li.viewPoint = { 15.153, 18.593, 85.767 };
                        li.viewPointSound = true;
                        break;
                    }
                    case EventCodes::BoatPartFound:
                    {
                        auto& li = em.getSingleton<LevelInfo>();
                        auto& plfi = em.getSingleton<PlayerInfo>();
                        auto& txti = em.getSingleton<TextInfo>();

                        auto& boatPart = *em.getEntityByID(li.boatPartFound);
                        auto& bc = em.getComponent<BoatComponent>(boatPart);

                        plfi.boatParts.push_back(bc.part);
                        std::string part;
                        switch (bc.part)
                        {
                        case BoatParts::Base:
                        {
                            part = "el casco";
                            break;
                        }
                        case BoatParts::Motor:
                        {
                            part = "el motor";
                            break;
                        }
                        case BoatParts::Propeller:
                        {
                            part = "la hélice";
                            break;
                        }
                        case BoatParts::SteeringWheel:
                        {
                            part = "el timón";
                            break;
                        }
                        }
                        std::string msg = "¡Has encontrado " + part + " de la barca!";
                        txti.addText(msg);

                        li.insertDeath(li.boatPartFound);

                        if (plfi.boatParts.size() == 4)
                        {
                            vec3d pos{ -126.872, 7.0, 24.918 };
                            li.viewPoint = pos;
                            li.events.insert(EventCodes::BoatDialog);

                            auto& newBoat{ em.newEntity() };
                            em.addTag<BoatTag>(newBoat);
                            em.addComponent<RenderComponent>(newBoat, RenderComponent{ .position = pos, .scale = { 1.0, 1.0, 1.0 }, .color = WHITE, .orientation = 0.0, .rotationVec = { 0.0, 1.0, 0.0 } });
                        }

                        break;
                    }
                    case EventCodes::BoatDialog:
                    {
                        auto& txti = em.getSingleton<TextInfo>();

                        std::array<std::string, 2> msgs =
                        {
                            "¡Has encontrado todas las partes de la barca!",
                            "¡Ahora podréis salir del volcán!"
                        };

                        for (const auto& msg : msgs)
                            txti.addText(msg);

                        break;
                    }
                    case EventCodes::NPCDialog:
                    {
                        auto& li = em.getSingleton<LevelInfo>();
                        if (li.npcToTalk == li.max)
                            break;

                        auto& npc = *em.getEntityByID(li.npcToTalk);
                        auto& dc = em.getComponent<DispatcherComponent>(npc);

                        if (dc.eventCodes.size() > 1)
                        {
                            auto& lc = em.getComponent<ListenerComponent>(e);
                            for (std::size_t i = 1; i < dc.eventCodes.size(); i++)
                            {
                                scheduleEvent(Event{ static_cast<EventCodes>(dc.eventCodes[i]) });
                                lc.addCode(static_cast<EventCodes>(dc.eventCodes[i]));
                            }
                        }

                        li.npcToTalk = li.max;
                        break;
                    }
                    case EventCodes::DialogPrisonNomad1:
                    {
                        auto& txti = em.getSingleton<TextInfo>();

                        std::array<std::string, 9> msgs =
                        {
                            "Nómada: \nSaludos, no esperaba encontrarme a un \naprendiz de mago por aquí.",
                            "Mago: \nBuenas! Sabes qué son todos estos enemigos?\nNo aparecían en los libros que estudié.",
                            "Nómada: \nCuanto más se debilita la barrera más enemigos \nentran al mundo, cosa del viejo.",
                            "Mago: \n¿Qué viejo?",
                            "Nómada: \nEl pellejo.",
                            "Mago: \nEspera, ¿¿no será el gran mago??",
                            "Nómada: \nEse mismo",
                            "Mago: \nEse es mi maestro! \nMe abandonó y me encomendó que lo encontrara.",
                            "Nómada: \nSi eres el aprendiz del viejo sabrás utilizar esto."
                        };

                        // Metemos el texto en el array de texto
                        for (std::size_t i = 0; i < msgs.size(); i++)
                            txti.addText(msgs[i]);

                        events.push_back(Event{ EventCodes::DialogPrisonNomad2 });
                        out = true;
                        break;
                    }
                    case EventCodes::DialogPrisonNomad2:
                    {
                        auto& li = em.getSingleton<LevelInfo>();
                        auto& txti = em.getSingleton<TextInfo>();
                        auto& plfi = em.getSingleton<PlayerInfo>();

                        std::array<std::string, 2> msgs =
                        {
                            "¡Haz recibido un pergamino con la formulación \npara crear una pompa de agua!",
                            "Nómada: \nCon esto podremos salir de aquí por si disparas a esa puerta. \nLos muñecos de por medio te servirán de práctica."
                        };

                        // Le damos el hechizo
                        Spell spell{ "Pompa de agua", "Disparas una potente concentración de agua que explota al impacto", Spells::WaterBomb, 20.0, 4 };
                        plfi.addSpell(spell);
                        plfi.showBook = true;

                        // Metemos el texto en el stack de texto
                        for (std::size_t i = 0; i < msgs.size(); i++)
                            txti.addText(msgs[i]);

                        li.viewPoint = { -84.847, 8.0, 234.267 };

                        auto& playerPhy = em.getComponent<PhysicsComponent>(*em.getEntityByID(li.playerID));
                        playerPhy.notMove = false;
                        break;
                    }
                    case EventCodes::DialogNomadVolcano1:
                    {
                        auto& txti = em.getSingleton<TextInfo>();
                        std::array<std::string, 3> msgs =
                        {
                            "Nómada: \nTú de nuevo, gracias por sacarme de esa mazmorra.\n ¿Ves los charcos de lava de alrededor?",
                            "Mago: \nSí, pero no puedo apagarlos con mi agua. Y tengo que cruzarlos.",
                            "Nómada: \n¿Qué te enseñó el viejo ese? \n Toma, usa esto te ayudará con la lava."
                        };

                        // Metemos el texto en el stack de texto
                        for (std::size_t i = 0; i < msgs.size(); i++)
                            txti.addText(msgs[i]);

                        events.push_back(Event{ EventCodes::DialogNomadVolcano2 });
                        out = true;
                        break;
                    }
                    case EventCodes::DialogNomadVolcano2:
                    {
                        auto& li = em.getSingleton<LevelInfo>();
                        auto& txti = em.getSingleton<TextInfo>();
                        auto& plfi = em.getSingleton<PlayerInfo>();

                        std::array<std::string, 2> msgs =
                        {
                            "¡Haz recibido un pergamino con la formulación \npara hacer una esquiva de agua!",
                            "Nómada: \nCon esto podrás cruzar los charcos de lava. \n¡Buena suerte, yo solo necesito mis buenas patas para cruzar!"
                        };

                        // Le damos el hechizo
                        Spell spell{ "Dash de agua", "Esquivas a la velocidad de la marea", Spells::WaterDash, 20.0, 4 };
                        plfi.addSpell(spell);
                        plfi.showBook = true;

                        // Metemos el texto en el stack de texto
                        for (std::size_t i = 0; i < msgs.size(); i++)
                            txti.addText(msgs[i]);

                        auto& playerPhy = em.getComponent<PhysicsComponent>(*em.getEntityByID(li.playerID));
                        playerPhy.notMove = false;
                        break;
                    }
                    case EventCodes::DialogCatVolcano1:
                    {
                        auto& txti = em.getSingleton<TextInfo>();

                        std::array<std::string, 12> msgs =
                        {
                            "Gato: \n¡Miau! ¡Miau! ¡Miau!",
                            "Mago: \n¿Qué dices? ¿Qué quieres decirme?",
                            "Gato: \n¿Acaso no hablas gatuno? Qué falta de respeto",
                            "Mago: \nLo siento, le pediré a mi maestro que me lo enseñe.",
                            "Gato: \nNo te preocupes niño, si me estaba quedando contigo. \nSoy un investigador",
                            "Mago: \n¡Ah vale! Oye, ¿sabes cómo salir de aquí? \nTengo que encontrar a mi maestro.",
                            "Investigador: \nNo eres el único que quiere salir de aquí, \npero igual deberías preocuparte más por el volcán \n a punto de estallar que de tu maestro.",
                            "Mago: \n¿¡Quéeeee!? ¡¿Estallar!? ¡¿Cómo?!",
                            "Investigador: \nAsí es, pero no te alarmes, porque me vas a ayudar a \nreconstruir mi barca para salir de aquí.",
                            "Mago: \n¿Una barca?",
                            "Investigador: \nEfectivamente, los monstruos me desperdigaron \nlas piezas por todo el lugar, ayúdame a recuperarlas\ny saldremos de aquí antes de la explosión.",
                            "Mago: \n¡Entiendo, déjalo en mis manos!"
                        };

                        // Metemos el texto en el stack de texto
                        for (std::size_t i = 0; i < msgs.size(); i++)
                            txti.addText(msgs[i]);

                        scheduleEvent(Event{ EventCodes::InitBoatParts });
                        out = true;
                        break;
                    }
                    case EventCodes::InitBoatParts:
                    {
                        // Ponemos a true la variable de enseñar número las piezas de barco
                        auto& li = em.getSingleton<LevelInfo>();
                        li.volcanoMission = true;
                        break;
                    }
                    default:
                        break;
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
