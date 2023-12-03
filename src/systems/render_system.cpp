#include "render_system.hpp"

#include <iomanip>

void RenderSystem::update(EntityManager& em, ENGI::GameEngine& engine)
{

    // Actualizamos la posicion de render del componente de fisicas
    em.forEach<SYSCMPs, SYSTAGs>([](Entity&, PhysicsComponent& phy, RenderComponent& ren)
    {
        ren.setPosition(phy.position);
    });

    beginFrame(engine, em);

    // Dibuja todas las entidades con componente de render
    size_t count = 0;
    for (auto const& e : em.getEntities())
    {
        if (e.hasComponent<RenderComponent>())
        {
            auto const& r{ em.getComponent<RenderComponent>(e) };
            // Revisamos si es el jugador para mover la cámara
            if (e.hasTag<PlayerTag>())
            {
                engine.setPositionCamera({ r.position.x(), 25.0f, r.position.z() + 25.0f });
                engine.setTargetCamera(r.position);
            }
            // Comprobar si tiene el componente vida
            Color colorEntidad = r.color;
            if(e.hasComponent<LifeComponent>()){
                auto& l { em.getComponent<LifeComponent>(e) };
                if(l.elapsed < l.countdown)
                    colorEntidad = YELLOW;
            }
            engine.drawCube(r.position, r.scale.x(), r.scale.y(), r.scale.z(), colorEntidad);
            
            // engine.drawCubeWires(r.position, r.scale.x(), r.scale.y(), r.scale.z(), MAROON);
        }
        count++;
    }

    endFrame(engine, em);
}

// Empieza el dibujado y se limpia la pantalla
void RenderSystem::beginFrame(ENGI::GameEngine& engine, EntityManager& em)
{
    engine.beginDrawing();
    engine.clearBackground(RAYWHITE);

    drawHUD(em, engine);

    engine.beginMode3D();
    // engine.drawGrid(20, 10.f);
}

// Se termina el dibujado
void RenderSystem::endFrame(ENGI::GameEngine& engine, EntityManager& em)
{
    engine.endMode3D();
    drawHUD(em, engine);
    engine.endDrawing();
}

// Se dibuja el HUD
void RenderSystem::drawHUD(EntityManager& em, ENGI::GameEngine& engine)
{
    auto& li = em.getSingleton<LevelInfo>();
    auto* playerEn = em.getEntityByID(li.playerID);
    if (not playerEn) { drawDeath(); return; }

    // Visualizar las vidas del player
    if (playerEn->hasTag<PlayerTag>() && playerEn->hasComponent<LifeComponent>())
    {
        // Dibujar vidas restantes del player en el HUD
        if(.hasTag<PlayerTag>() && e.hasComponent<LifeComponent>())
        {
            auto const& l{ em.getComponent<LifeComponent>(e) };
            std::string vida = "Vidas: " + std::to_string(l.life);
            engine.drawText(vida.c_str(), 10, 10, 20, BLACK);
        }
        // Dibujar el countdown restante del ataque del player en el HUD
        if(e.hasTag<PlayerTag>() && e.hasComponent<AttackComponent>())
        {
            auto const& a{ em.getComponent<AttackComponent>(e) };
            std::string countdown_ataque;
            if(a.elapsed > a.countdown)
                countdown_ataque = "Ataque listo";
            else
                countdown_ataque = "Ataque listo en: " + std::to_string(-1*(a.elapsed - 1.0f)) + " segundos";
            
            engine.drawText(countdown_ataque.c_str(), 10, 30, 20, BLACK);
        }
        
        // Dibujar la vida restante encima de las entidades // DEBUG
        if(e.hasComponent<LifeComponent>())
        {
            auto const& r{ em.getComponent<RenderComponent>(e) };
            auto const& l{ em.getComponent<LifeComponent>(e) };

            engine.drawText(std::to_string(l.life).c_str(), 
                            static_cast<int>(engine.getWorldToScreenX(r.position) - 5), 
                            static_cast<int>(engine.getWorldToScreenY(r.position) - r.scale.y()*50), 
                            20, 
                            BLACK);
        }
    }
}

void RenderSystem::drawDeath()
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));
    DrawText("HAS MUERTO", 250, 250, 40, RED);
}