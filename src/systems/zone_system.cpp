#include "zone_system.hpp"

void ZoneSystem::update(EntityManager& em, ENGI::GameEngine& engine){
    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& ent, ZoneComponent& zon)
    {
        if (zon.changeZone) {
            // Comprobar en que zona estamos
            auto& li = em.getSingleton<LevelInfo>();
            if (li.num_zone != zon.zone) {
                li.num_zone = zon.zone;
                // Es una zona
                if(zon.zone <= 12)
                {
                    // std::cout << "Acabo de entrar a la zona: " + std::to_string(zon.zone) << std::endl;
                    if (ent.hasComponent<RenderComponent>()) {
                        auto& r = em.getComponent<RenderComponent>(ent);
                        engine.setPositionCamera({ r.position.x(), 30.0f, r.position.z() + 12.0f });
                        engine.setTargetCamera({ r.position.x(), r.position.y() + 3.0f, r.position.z() });
                    }
                }
                // Es un TP
                else
                {
                    // std::cout << "Acabo de entrar en un tp" << std::endl;
                    auto* playerEn = em.getEntityByID(li.playerID);
                    auto& p = em.getComponent<PhysicsComponent>(*playerEn);
                    switch (zon.zone)
                    {
                    case 13: // TP a la cueva de la espada
                        p.position.setX(49.0f);
                        p.position.setZ(84.0f);
                        break;

                    case 14: // TP desde la cueva de la espada
                        p.position.setX(0.0f);
                        p.position.setZ(0.0f);
                        break;

                    case 15: // TP a la cueva de la tienda
                        p.position.setX(71.0f);
                        p.position.setZ(84.0f);
                        break;

                    case 17: // TP a la cueva de las monedas
                        p.position.setX(93.0f);
                        p.position.setZ(84.0f);
                        break;
                    
                    default:
                        break;
                    }
                }
            }
            zon.changeZone = false;
        }
    });
}