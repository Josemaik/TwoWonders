#include "shield_system.hpp"

void ShieldSystem::update(EntityManager& em) {
    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& ent, ShieldComponent& shi)
    {
        // shi.activeShield // booleano que muestra si el escudo esta activo o no
        // shi.direction    // direccion del escudo
        // shi.shield       // referencia a la entidad escudo

        if (shi.activeShield)
        {
            // Recuperamos el player
            auto& li = em.getSingleton<LevelInfo>();
            auto* playerEn = em.getEntityByID(li.playerID);

            if (playerEn)
            {
                if (ent.hasComponent<RenderComponent>() && ent.hasComponent<PhysicsComponent>() &&
                    playerEn->hasComponent<PhysicsComponent>() && playerEn->hasComponent<InputComponent>())
                {
                    auto& phy_pl = em.getComponent<PhysicsComponent>(*playerEn);
                    auto& p = em.getComponent<PhysicsComponent>(ent);

                    // Reposicionamos el escudo segun el player
                    p.position = phy_pl.position;
                    p.orientation = phy_pl.orientation;

                    static const double SHIELD_DISTANCE = 0.75f;

                    // Calculamos el desplazamiento basado en la orientación del jugador
                    double offsetX = sin(p.orientation) * SHIELD_DISTANCE;
                    double offsetZ = cos(p.orientation) * SHIELD_DISTANCE;

                    // Aplicamos el desplazamiento al escudo
                    p.position += { offsetX, 0.0f, offsetZ };
                }
            }
        }
        else
        {
            // Se elimina shi.shield
            std::cout << "Se borra el escudo" << std::endl;
            //em.destroyEntity(ent.getID());
        }
    });
}