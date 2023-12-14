#include "zone_system.hpp"

void ZoneSystem::update(EntityManager& em, ENGI::GameEngine& engine,Ia_man& iam){
    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& ent, ZoneComponent& zon)
    {
        if (zon.changeZone) {
            // Comprobar en que zona estamos
            auto& li = em.getSingleton<LevelInfo>();
            if (li.num_zone != zon.zone) {
                //Crear enemigos de la zona nueva
                // if(!iam.checkEnemiesCreaeted(zon.zone)){
                     iam.createEnemiesZone(em,zon.zone);
                // }
                //borro enemigos si cambio de zona
                iam.deleteEnemiesZone(em,li.num_zone);
                // Es una zona
                li.num_zone = zon.zone;
                if(zon.zone <= 12)
                {
                    if (ent.hasComponent<RenderComponent>()) {
                        auto& r = em.getComponent<RenderComponent>(ent);
                        engine.setPositionCamera({ r.position.x(), 30.0f, r.position.z() + 12.0f });
                        engine.setTargetCamera({ r.position.x(), r.position.y() + 3.0f, r.position.z() });
                    }
                }
                // Es un TP
                else
                {
                    auto* playerEn = em.getEntityByID(li.playerID);
                    auto& p = em.getComponent<PhysicsComponent>(*playerEn);
                    switch (zon.zone)
                    {

                    // SWORD //

                    case 13: // TP a la cueva de la espada
                        p.position.setX(49.0f);
                        p.position.setZ(85.0f);
                        break;

                    case 14: // TP desde la cueva de la espada
                        p.position.setX(-5.0f);
                        p.position.setZ(-4.5f);
                        break;

                    // COINS //

                    case 15: // TP a la cueva de las monedas
                        p.position.setX(71.0f);
                        p.position.setZ(85.0f);
                        break;

                    case 16: // TP desde la cueva de las monedas
                        p.position.setX(3.0f);
                        p.position.setZ(-20.0f);
                        break;

                    // SHOP //

                    case 17: // TP a la cueva de la tienda
                        p.position.setX(93.0f);
                        p.position.setZ(85.0f);
                        break;

                    case 18: // TP desde la cueva de la tienda
                        p.position.setX(-17.0f);
                        p.position.setZ(-20.0f);
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