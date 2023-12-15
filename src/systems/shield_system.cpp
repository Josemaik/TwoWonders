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
                auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = { 0.0f, 0.0f, 0.0f}, .scale = { 1.0f, 1.0f, 1.0f}, .color = BROWN });
                auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position = { r.position }});
                em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::SHIELD });

                // Se asigna a shi.shield
                shi.shield = e.getID();
            }
            // Se posiciona respecto a la entidad y a la direccion

        }
        else
        {
            // Se elimina shi.shield
            std::cout << "Se borra el escudo" << std::endl;
            em.destroyEntity(shi.shield);
        }
    });
}