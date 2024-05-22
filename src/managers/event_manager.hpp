#pragma once
#ifndef EVENT_MANAGER
#define EVENT_MANAGER

#include "../systems/object_system.hpp"
#include "map_manager.hpp"
#include "../utils/types.hpp"
#include <cstdint>
#include <vector>

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

                            txti.addText({ SpeakerType::NONE, msgs.front() });
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
                        break;
                    }
                    case EventCodes::SpawnWallLevel0:
                    {
                        auto& e{ em.newEntity() };
                        em.addTag<BarricadeTag>(e);
                        em.addTag<WallTag>(e);
                        em.addTag<SeparateModelTag>(e);
                        auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = vec3d::zero(), .scale = vec3d::zero(), .color = D_ORANGE_DARK, .orientation = 90.0 * DEGTORAD, .rotationVec = { 0.0, -1.0, 0.0 } });
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
                            txti.addText({ SpeakerType::NONE, msgs[i] });
                        break;
                    }
                    case EventCodes::ViewPointDoor:
                    {
                        auto& li = em.getSingleton<LevelInfo>();
                        li.viewPoint = { 15.153, 18.593, 85.767 };
                        li.viewPointSound = true;
                        break;
                    }
                    case EventCodes::ViewPointNomadDoor:
                    {
                        auto& li = em.getSingleton<LevelInfo>();
                        li.viewPoint = { -15.847, 16.593, 234.267 };
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
                        em.getSingleton<SoundSystem>().sonido_recoger_pieza();
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
                        txti.addText({ SpeakerType::NONE, msg });

                        li.insertDeath(li.boatPartFound);

                        if (plfi.boatParts.size() == 4)
                        {
                            //em.getSingleton<SoundSystem>().sonido_recoger_pieza();
                            vec3d pos{ -126.872, 7.0, 24.918 };
                            li.viewPoint = pos;
                            li.events.insert(EventCodes::BoatDialog);

                            auto& newBoat{ em.newEntity() };
                            em.addTag<BoatTag>(newBoat);
                            em.addComponent<RenderComponent>(newBoat, RenderComponent{ .position = pos, .scale = { 1.0, 1.0, 1.0 }, .color = D_WHITE, .orientation = 0.0, .rotationVec = { 0.0, 1.0, 0.0 } });
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
                            txti.addText({ SpeakerType::NONE, msg });

                        auto& li = em.getSingleton<LevelInfo>();
                        li.investigatorstartwalk = true;
                        using CMPs = MP::TypeList<PhysicsComponent, NPCComponent, AnimationComponent>;
                        using npcTAG = MP::TypeList<InvestigatorTag>;

                        em.forEach<CMPs, npcTAG>([&](Entity&, PhysicsComponent& phy, NPCComponent& npc, AnimationComponent& anc)
                        {
                            npc.pathIt_inestigador = npc.path_investigador.begin();
                            phy.position = { 27.2022,14,-104.252 };
                            npc.tp = true;
                            anc.animToPlay = static_cast<std::size_t>(InvestAnimations::WALK);
                        });

                        break;
                    }
                    case EventCodes::NPCDialog:
                    {
                        auto& li = em.getSingleton<LevelInfo>();
                        if (li.npcToTalk == li.max)
                            break;

                        auto& npc = *em.getEntityByID(li.npcToTalk);
                        auto& npcC = em.getComponent<NPCComponent>(npc);
                        auto& dc = em.getComponent<DispatcherComponent>(npc);

                        if (dc.eventCodes.size() > 0)
                        {
                            for (std::size_t i = 0; i < dc.eventCodes.size(); i++)
                                scheduleEvent(Event{ static_cast<EventCodes>(dc.eventCodes[i]) });
                        }
                        dc.eventCodes.clear();

                        auto& anc = em.getComponent<AnimationComponent>(e);
                        anc.animToPlay = static_cast<std::size_t>(PlayerAnimations::SPEAKING);

                        auto& animNpc = em.getComponent<AnimationComponent>(npc);

                        switch (npcC.type)
                        {
                        case NPCType::NOMAD:
                        {
                            animNpc.animToPlay = static_cast<std::size_t>(NomadAnimations::SPEAKING);
                            break;
                        }
                        case NPCType::INVESTIGATOR:
                        {
                            animNpc.animToPlay = static_cast<std::size_t>(InvestAnimations::SPEAKING);
                            break;
                        }
                        default:
                            break;
                        }
                        break;
                    }
                    case EventCodes::DialogPrisonNomad1:
                    {
                        auto& txti = em.getSingleton<TextInfo>();

                        std::array<std::pair<SpeakerType, std::string>, 9> msgs =
                        {
                            std::make_pair(SpeakerType::NOMAD, "Nómada: \nSaludos, no esperaba encontrarme a un \naprendiz de mago por aquí."),
                            std::make_pair(SpeakerType::PLAYER, "Mago: \n¡Buenas! ¿Sabes qué son todos estos enemigos?\nNo aparecían en los libros que estudié."),
                            std::make_pair(SpeakerType::NOMAD, "Nómada: \nCuanto más se debilita la barrera más enemigos \nentran al mundo, cosa del viejo."),
                            std::make_pair(SpeakerType::PLAYER_SAD, "Mago: \n¿Qué viejo?"),
                            std::make_pair(SpeakerType::NOMAD, "Nómada: \nEl pellejo."),
                            std::make_pair(SpeakerType::PLAYER_SAD, "Mago: \nEspera, ¿¿no será el gran mago??"),
                            std::make_pair(SpeakerType::NOMAD, "Nómada: \nEse mismo."),
                            std::make_pair(SpeakerType::PLAYER, "Mago: \n¡Ese es mi maestro! \nMe abandonó y me encomendó que lo encontrara."),
                            std::make_pair(SpeakerType::NOMAD, "Nómada: \nSi eres el aprendiz del viejo sabrás utilizar esto.")
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

                        std::array<std::pair<SpeakerType, std::string>, 2> msgs =
                        {
                            std::make_pair(SpeakerType::NONE, "¡Has recibido un pergamino con la formulación \npara crear una pompa de agua!"),
                            std::make_pair(SpeakerType::NOMAD, "Nómada: \nCon esto podremos salir de aquí, disparando a esa puerta. \nLos muñecos de por medio te servirán de práctica.")
                        };

                        // Le damos el hechizo
                        Spell spell{ "Pompa de agua", "Disparas una potente concentración de agua que explota al impacto", AttackType::WaterBombShot };
                        plfi.addSpell(spell);
                        plfi.showBook = true;

                        // Metemos el texto en el stack de texto
                        for (std::size_t i = 0; i < msgs.size(); i++)
                            txti.addText(msgs[i]);

                        li.viewPoint = { -84.847, 8.0, 234.267 };

                        auto& playerPhy = em.getComponent<PhysicsComponent>(*em.getEntityByID(li.playerID));
                        playerPhy.notMove = false;

                        // Cosas de animaciones a partir de aquí
                        auto& playerAnim = em.getComponent<AnimationComponent>(e);
                        playerAnim.animToPlay = static_cast<std::size_t>(PlayerAnimations::SPEAKING);

                        using NoCMPS = MP::TypeList<AnimationComponent>;
                        using NomadTAG = MP::TypeList<NomadTag>;

                        em.forEach<NoCMPS, NomadTAG>([&](Entity&, AnimationComponent& anc)
                        {
                            anc.animToPlay = static_cast<std::size_t>(NomadAnimations::GIVE_ITEM);
                        });
                        break;
                    }
                    case EventCodes::DialogNomadVolcano1:
                    {
                        auto& txti = em.getSingleton<TextInfo>();
                        std::array<std::pair<SpeakerType, std::string>, 3> msgs =
                        {
                            std::make_pair(SpeakerType::NOMAD, "Nómada: \nTú de nuevo, gracias por sacarme de esa mazmorra.\n ¿Ves los charcos de lava de alrededor?"),
                            std::make_pair(SpeakerType::PLAYER_SAD, "Mago: \nSí, pero no puedo apagarlos con mi agua. \nY tengo que cruzarlos."),
                            std::make_pair(SpeakerType::NOMAD, "Nómada: \n¿Qué te enseñó el viejo ese? \n Toma usa esto, te ayudará con la lava.")
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

                        std::array<std::pair<SpeakerType, std::string>, 2> msgs =
                        {
                            std::make_pair(SpeakerType::NONE, "¡Has recibido un pergamino con la formulación \npara hacer una esquiva de agua!"),
                            std::make_pair(SpeakerType::NOMAD, "Nómada: \nCon esto podrás cruzar los charcos de lava. \n¡Buena suerte, yo solo necesito mis buenas patas para cruzar!")
                        };

                        // Le damos el hechizo
                        Spell spell{ "Dash de agua", "Esquivas a la velocidad de la marea", AttackType::WaterDashArea };
                        plfi.addSpell(spell);
                        plfi.showBook = true;

                        // Metemos el texto en el stack de texto
                        for (std::size_t i = 0; i < msgs.size(); i++)
                            txti.addText(msgs[i]);

                        auto& playerPhy = em.getComponent<PhysicsComponent>(*em.getEntityByID(li.playerID));
                        playerPhy.notMove = false;

                        // Cosas de animaciones
                        auto& playerAnim = em.getComponent<AnimationComponent>(e);
                        playerAnim.animToPlay = static_cast<std::size_t>(PlayerAnimations::SPEAKING);

                        using NoCMPS = MP::TypeList<AnimationComponent>;
                        using NomadTAG = MP::TypeList<NomadTag>;

                        em.forEach<NoCMPS, NomadTAG>([&](Entity&, AnimationComponent& anc)
                        {
                            anc.animToPlay = static_cast<std::size_t>(NomadAnimations::GIVE_ITEM);
                        });
                        break;
                    }
                    case EventCodes::DialogCatVolcano1:
                    {
                        auto& txti = em.getSingleton<TextInfo>();
                        std::array<std::pair<SpeakerType, std::string>, 14> msgs =
                        {
                            std::make_pair(SpeakerType::CAT, "Gato: \n¡Miau! ¡Miau! ¡Miau!"),
                            std::make_pair(SpeakerType::PLAYER_SAD, "Mago: \n¿Qué dices? ¿Qué quieres decirme?"),
                            std::make_pair(SpeakerType::CAT, "Gato: \n¿Acaso no hablas gatuno? Qué falta de respeto."),
                            std::make_pair(SpeakerType::PLAYER_DANGER, "Mago: \nLo siento, le pediré a mi maestro que me lo enseñe."),
                            std::make_pair(SpeakerType::CAT, "Gato: \nNo te preocupes niño, si me estaba quedando contigo. \nSoy un investigador."),
                            std::make_pair(SpeakerType::PLAYER, "Mago: \n¡Ah vale! Oye, ¿sabes cómo salir de aquí? \nTengo que encontrar a mi maestro."),
                            std::make_pair(SpeakerType::INVESTIGATOR, "Investigato: \nNo eres el único que quiere salir de aquí."),
                            std::make_pair(SpeakerType::INVESTIGATOR, "Investigato: \nIgual deberías de preocuparte más por el volcán\n a punto de estallar que de tu maestro."),
                            std::make_pair(SpeakerType::PLAYER_DANGER, "Mago: \n¿¡Quéeeee!? ¿¡Estallar!? ¿¡Cómo¡?"),
                            std::make_pair(SpeakerType::INVESTIGATOR, "Investigato: \nAsí es, pero no te alarmes, porque me vas a ayudar a \nreconstruir mi barca para salir de aquí."),
                            std::make_pair(SpeakerType::PLAYER_SAD, "Mago: \n¿Una barca?"),
                            std::make_pair(SpeakerType::INVESTIGATOR, "Investigato: \nEfectivamente, los monstruos me desperdigaron \nlas piezas por todo el lugar."),
                            std::make_pair(SpeakerType::INVESTIGATOR, "Investigato: \nAyúdame a recuperarlas\ny saldremos de aquí antes de la explosión."),
                            std::make_pair(SpeakerType::PLAYER, "Mago: \n¡Entiendo, déjalo en mis manos!")
                        };

                        // Metemos el texto en el stack de texto
                        for (std::size_t i = 0; i < msgs.size(); i++)
                            txti.addText(msgs[i]);

                        scheduleEvent(Event{ EventCodes::InitBoatParts });
                        out = true;
                        break;
                    }
                    case EventCodes::DialogCatVolcano2:
                    {
                        auto& li = em.getSingleton<LevelInfo>();
                        auto& txti = em.getSingleton<TextInfo>();
                        std::array<std::pair<SpeakerType, std::string>, 5> msgs =
                        {
                            std::make_pair(SpeakerType::INVESTIGATOR, "Investigato: \n¡Miau! ¡Miau! ¡Miau!"),
                            std::make_pair(SpeakerType::PLAYER, "Mago: \nNo me vuelves a pillar con esa."),
                            std::make_pair(SpeakerType::INVESTIGATOR, "Investigato: \nBuen trabajo, ahora vamos a salir de aquí\n He terminado las reparaciones."),
                            std::make_pair(SpeakerType::PLAYER, "Mago: \n¡Perfecto!"),
                            std::make_pair(SpeakerType::INVESTIGATOR, "Investigato: \nPero antes toma una recompensa por tu trabajo.")
                        };

                        // Metemos el texto en el stack de texto
                        for (std::size_t i = 0; i < msgs.size(); i++)
                            txti.addText(msgs[i]);

                        li.npcflee = true;

                        scheduleEvent(Event{ EventCodes::DialogCatVolcano3 });
                        out = true;

                        // Cosas de animaciones a partir de aquí
                        using NoCMPS = MP::TypeList<AnimationComponent>;
                        using NomadTAG = MP::TypeList<NomadTag>;

                        em.forEach<NoCMPS, NomadTAG>([&](Entity&, AnimationComponent& anc)
                        {
                            anc.animToPlay = static_cast<std::size_t>(NomadAnimations::WALK);
                        });
                        break;
                    }
                    case EventCodes::DialogCatVolcano3:
                    {
                        auto& li = em.getSingleton<LevelInfo>();
                        auto& txti = em.getSingleton<TextInfo>();
                        auto& plfi = em.getSingleton<PlayerInfo>();

                        std::array<std::pair<SpeakerType, std::string>, 2> msgs =
                        {
                            std::make_pair(SpeakerType::NONE, "¡Has recibido un pergamino con la formulación \npara hacer una bola de fuego!"),
                            std::make_pair(SpeakerType::INVESTIGATOR, "Investigato: \nCon esto y un bizcocho, nos largamos de aquí.")
                        };

                        // Le damos el hechizo
                        Spell spell{ "Bola de fuego", "Tiras una poderosa bola de destrucción", AttackType::FireBallShot };
                        plfi.addSpell(spell);
                        plfi.showBook = true;

                        // Metemos el texto en el stack de texto
                        for (std::size_t i = 0; i < msgs.size(); i++)
                            txti.addText(msgs[i]);

                        auto& playerPhy = em.getComponent<PhysicsComponent>(*em.getEntityByID(li.playerID));
                        playerPhy.notMove = false;

                        // Animación de dar el objeto
                        auto& playerAnim = em.getComponent<AnimationComponent>(e);
                        playerAnim.animToPlay = static_cast<std::size_t>(PlayerAnimations::SPEAKING);

                        using NoCMPS = MP::TypeList<AnimationComponent>;
                        using InvestTAG = MP::TypeList<InvestigatorTag>;

                        em.forEach<NoCMPS, InvestTAG>([&](Entity&, AnimationComponent& anc)
                        {
                            anc.animToPlay = static_cast<std::size_t>(InvestAnimations::GIVE_ITEM);
                        });
                        break;
                    }
                    case EventCodes::DialogNomadVolcano3:
                    {
                        auto& txti = em.getSingleton<TextInfo>();

                        std::array<std::pair<SpeakerType, std::string>, 6> msgs =
                        {
                            std::make_pair(SpeakerType::NOMAD, "Nómada: \nYa os queríais largar sin mi, ¿eh?\n¿Acaso no os importa mi vida?"),
                            std::make_pair(SpeakerType::PLAYER_SAD, "Mago: \n¡Lo siento! Iba a buscarte cuando has aparecido."),
                            std::make_pair(SpeakerType::NOMAD, "Nómada: \nCasi se me queman las semillas \npasando por la lava, a ver \n si tenéis un poco de respeto."),
                            std::make_pair(SpeakerType::PLAYER_DANGER, "Mago: \nLo siento mucho por tus semillas."),
                            std::make_pair(SpeakerType::NOMAD, "Nómada: \nBueno venga, vámonos de aquí."),
                            std::make_pair(SpeakerType::INVESTIGATOR, "Investigato: \n¡Miau! ¡Miau! ¡Miau!")
                        };

                        // Metemos el texto en el stack de texto
                        for (std::size_t i = 0; i < msgs.size(); i++)
                            txti.addText(msgs[i]);

                        scheduleEvent(Event{ EventCodes::TPBoat });
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
                    case EventCodes::TPBoat:
                    {
                        auto& li = em.getSingleton<LevelInfo>();

                        using CMPs = MP::TypeList<PhysicsComponent>;
                        using npcTAG = MP::TypeList<NPCTag>;

                        em.forEach<CMPs, npcTAG>([&](Entity&, PhysicsComponent& phy)
                        {
                            phy.position = { -126.872, 7.0, 24.918 };
                        });

                        auto& playerPhy = em.getComponent<PhysicsComponent>(*em.getEntityByID(li.playerID));
                        playerPhy.position = { -126.872, 7.0, 24.918 };

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
