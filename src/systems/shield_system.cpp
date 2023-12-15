#include "shield_system.hpp"

void ShieldSystem::update(EntityManager& em){
    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& ent, ShieldComponent& shi)
    {
        // shi.activeShield // booleano que muestra si el escudo esta activo o no
        // shi.direction    // direccion del escudo
        // shi.shield       // referencia a la entidad escudo

        if(shi.activeShield)
        {
            // Recuperamos el player
            auto& li = em.getSingleton<LevelInfo>();
            auto* playerEn = em.getEntityByID(li.playerID);

            if(playerEn)
            {
                if(ent.hasComponent<RenderComponent>() && ent.hasComponent<PhysicsComponent>() && 
                   playerEn->hasComponent<PhysicsComponent>() && playerEn->hasComponent<InputComponent>())
                {
                    auto& inp_pl = em.getComponent<InputComponent>(*playerEn);
                    auto& phy_pl = em.getComponent<PhysicsComponent>(*playerEn);
                    auto& r = em.getComponent<RenderComponent>(ent);
                    auto& p = em.getComponent<PhysicsComponent>(ent);
                    // Reposicionamos el escudo segun el player
                    p.position = phy_pl.position;
                    // Reposicionamos y escalamos segun la direccion
                    if(inp_pl.last_key == inp_pl.left)
                    {
                        p.position += { -0.75f, 0.0f, 0.0f };
                        r.scale = { 0.5f, 1.0f, 1.0f };
                    }
                    else if(inp_pl.last_key == inp_pl.right)
                    {
                        p.position += { 0.75f, 0.0f, 0.0f };
                        r.scale = { 0.5f, 1.0f, 1.0f };
                    }
                    else if(inp_pl.last_key == inp_pl.up)
                    {
                        p.position += { 0.0f, 0.0f, -0.75f };
                        r.scale = { 1.0f, 1.0f, 0.5f };
                    }
                    else
                    {
                        p.position += { 0.0f, 0.0f, 0.75f };
                        r.scale = { 1.0f, 1.0f, 0.5f };
                    }
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