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
                auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = em.getComponent<RenderComponent>(ent).position, .scale = { 1.0f, 1.0f, 1.0f}, .color = BROWN });
                auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position = { r.position }});
                em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::SHIELD });

                // Se asigna a shi.shield
                shi.shield = e.getID();
            }
            // Se posiciona respecto a la entidad y a la direccion
            if(shi.shield && ent.hasComponent<PhysicsComponent>() && em.getEntityByID(shi.shield)->hasComponent<PhysicsComponent>())
            {
                auto& ep = em.getComponent<PhysicsComponent>(*em.getEntityByID(shi.shield));
                ep.position = em.getComponent<PhysicsComponent>(ent).position;
                if(ent.hasComponent<InputComponent>() && em.getEntityByID(shi.shield)->hasComponent<RenderComponent>() && em.getEntityByID(shi.shield)->hasComponent<ColliderComponent>())
                {
                    auto& inp = em.getComponent<InputComponent>(ent);
                    auto& r =  em.getComponent<RenderComponent>(*em.getEntityByID(shi.shield));
                    auto& c =  em.getComponent<ColliderComponent>(*em.getEntityByID(shi.shield));
                    if(inp.last_key == inp.up)
                    {
                        ep.position += {0.0f, 0.0f, -0.75f};
                        r.scale = { 1.0f, 1.0f, 0.5f};
                    }
                    else if(inp.last_key == inp.down)
                    {
                        ep.position += {0.0f, 0.0f, 0.75f};
                        r.scale = { 1.0f, 1.0f, 0.5f};
                    }
                    else if(inp.last_key == inp.right)
                    {
                        ep.position += {0.75f, 0.0f, 0.0f};
                        r.scale = { 0.5f, 1.0f, 1.0f};
                    }
                    else if(inp.last_key == inp.left)
                    {
                        ep.position += {-0.75f, 0.0f, 0.0f};
                        r.scale = { 0.5f, 1.0f, 1.0f};
                    }
                }
            }
        }
        else
        {
            // Se elimina shi.shield
            std::cout << "Se borra el escudo" << std::endl;
            //em.destroyEntity(shi.shield);
        }
    });
}