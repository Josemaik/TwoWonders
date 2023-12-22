#include "zone_system.hpp"

void ZoneSystem::update(EntityManager& em, ENGI::GameEngine& engine, Ia_man& iam,Eventmanager& evm) {


    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& ent, ZoneComponent& zon)
    {
        if (zon.changeZone) {
            // Comprobar en que zona estamos
            auto& li = em.getSingleton<LevelInfo>();
            if (li.num_zone != zon.zone) {
                // crear una funcion que devuelva un Evento
                //Crear enemigos de la zona nueva
                // if(!iam.checkEnemiesCreaeted(zon.zone)){
                // }
                //lanzar evento
                evm.scheduleEvent(Event{EVENT_CODE_PLAYER_HIT});
                //borro enemigos si cambio de zona
                deleteEnemiesinZone(em, li.num_zone);
                iam.createEnemiesZone(em, zon.zone);
                // Es una zona
                li.num_zone = zon.zone;
                if (zon.zone <= 13)
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

                    case 14: // TP a la cueva de la espada
                        p.position.setX(49.0f);
                        p.position.setZ(85.0f);
                        break;

                    case 15: // TP desde la cueva de la espada
                        p.position.setX(-5.0f);
                        p.position.setZ(-4.5f);
                        break;

                        // COINS //

                    case 16: // TP a la cueva de las monedas
                        p.position.setX(71.0f);
                        p.position.setZ(85.0f);
                        break;

                    case 17: // TP desde la cueva de las monedas
                        p.position.setX(3.0f);
                        p.position.setZ(-20.0f);
                        break;

                        // SHOP //

                    case 18: // TP a la cueva de la tienda
                        p.position.setX(93.0f);
                        p.position.setZ(85.0f);
                        break;

                    case 19: // TP desde la cueva de la tienda
                        p.position.setX(-17.0f);
                        p.position.setZ(-20.0f);
                        break;

                        // MAZMORRA //

                    case 20: // TP a la cueva de la mazmorra
                        p.position.setX(61.0f);
                        p.position.setZ(-65.0f);
                        break;

                    case 21: // TP desde la cueva de la mazmorra
                        p.position.setX(-50.0f);
                        p.position.setZ(-3.5f);
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

void ZoneSystem::deleteEnemiesinZone(EntityManager& em, uint16_t z)
{
    auto& li = em.getSingleton<LevelInfo>();
    if (z == li.num_zone)
    {
        std::unordered_set<std::size_t> enemies = li.enemiesID;
        for (auto& enemy : enemies)
            if (em.getEntityByID(enemy)->hasTag<EnemyTag>())
                em.getComponent<LifeComponent>(*em.getEntityByID(enemy)).markedForDeletion = true;
    }
}