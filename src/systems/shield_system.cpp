#include "shield_system.hpp"

void ShieldSystem::update(EntityManager& em){
    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& ent, ShieldComponent& shi)
    {
        // shi.activeShield // booleano que muestra si el escudo esta activo o no
        // shi.direction    // direccion del escudo
        // shi.shield       // referencia a la entidad escudo

        if(shi.activeShield)
        {
            // Si no existe el escudo se crea
            if(!shi.shield){
                std::cout << "No existe el escudo, se va a crear" << std::endl;
                auto& e { em.newEntity() };

                // Se asigna a shi.shield
                shi.shield = e.getID();
            }
            // si no existe shi.shield, se crea
            // se posiciona shi.shield respecto a la entidad
        }
        else
        {
            // se elimina shi.shield
        }
    });
}