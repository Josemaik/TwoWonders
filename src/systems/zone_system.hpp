#pragma once 
#ifndef ZONE_SYSTEM
#define ZONE_SYSTEM
#include "../utils/types.hpp"
#include "../managers/game_engine.hpp"
#include "../managers/ia_manager.hpp"

struct ZoneSystem
{
    using SYSCMPs = MP::TypeList<ZoneComponent>;
    using SYSTAGs = MP::TypeList<>;

    void update(EntityManager& em, ENGI::GameEngine& engine,Ia_man& iam) {
        em.forEach<SYSCMPs, SYSTAGs>([&](Entity& ent, ZoneComponent& zon)
        {
            if (zon.changeZone) {
                // Comprobar en que zona estamos
                auto& li = em.getSingleton<LevelInfo>();
                if (li.num_zone != zon.zone) {
                    iam.createEnemiesZone(em,zon.zone);
                    //borro enemigos si cambio de zona
                    iam.deleteEnemiesZone(em,li.num_zone);
                    //std::cout << "Acabo de entrar a la zona: " + std::to_string(zon.zone) << std::endl;
                    li.num_zone = zon.zone;
                    if (ent.hasComponent<RenderComponent>()) {
                        auto& r = em.getComponent<RenderComponent>(ent);
                        engine.setPositionCamera({ r.position.x(), 30.0f, r.position.z() + 12.0f });
                        engine.setTargetCamera({ r.position.x(), r.position.y() + 3.0f, r.position.z() });
                    }
                }
                zon.changeZone = false;
            }

            //if(/*(li.num_zone != zon.zone) && */zon.changeZone && ent.hasComponent<RenderComponent>()){
            //    //std::cout << "Cambio a la zona: " + zon.zone << std::endl;
            //    auto& r = em.getComponent<RenderComponent>(ent);
            //    engine.setPositionCamera({ r.position.x(), 25.0f, r.position.z() + 25.0f });
            //    engine.setTargetCamera({r.position.x(), r.position.y() + 3.0f, r.position.z()});
            //    zon.zone = false;
            //    // li.num_zone = zon.zone;
            //}
        });
    }

};

#endif // !ZONE_SYSTEM