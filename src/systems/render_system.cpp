#include "render_system.hpp"
#include "../managers/entity_manager.hpp"

void RenderSystem::update(EntityManager& em, ENGI::GameEngine& engine)
{

    // Actualizamos la posicion de render del componente de fisicas
    em.forEach<SYSCMPs, SYSTAGs>([](Entity&, PhysicsComponent& phy, RenderComponent& ren)
    {
        ren.setPosition(phy.position);
    });

    beginFrame(engine, em);

    // Dibuja todas las entidades con componente de render
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
            engine.drawCube(r.position, r.scale.x(), r.scale.y(), r.scale.z(), r.color);
            engine.drawCubeWires(r.position, r.scale.x(), r.scale.y(), r.scale.z(), MAROON);
        }
    }

    endFrame(engine);
}

// Empieza el dibujado y se limpia la pantalla
void RenderSystem::beginFrame(ENGI::GameEngine& engine, EntityManager& em)
{
    engine.beginDrawing();
    engine.clearBackground(RAYWHITE);

    drawHUD(em);

    engine.beginMode3D();
    engine.drawGrid(20, 1.0f);
}

// Se termina el dibujado
void RenderSystem::endFrame(ENGI::GameEngine& engine)
{
    engine.endMode3D();
    engine.endDrawing();
}

// Se dibuja el HUD
void RenderSystem::drawHUD(EntityManager& em)
{
    // Visualizar las vidas del player
    for (auto const& e : em.getEntities())
    {
        if (e.hasTag<PlayerTag>() && e.hasComponent<LifeComponent>())
        {
            auto const& l{ em.getComponent<LifeComponent>(e) };
            std::string vida = "Vidas: " + std::to_string(l.life);
            DrawText(vida.c_str(), 10, 10, 20, BLACK);
        }
    }
}