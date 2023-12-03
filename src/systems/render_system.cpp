#include "render_system.hpp"

void RenderSystem::update(EntityManager& em, ENGI::GameEngine& engine)
{

    // Actualizamos la posicion de render del componente de fisicas
    em.forEach<SYSCMPs, SYSTAGs>([](Entity&, PhysicsComponent& phy, RenderComponent& ren)
    {
        ren.setPosition(phy.position);
    });

    beginFrame(engine);

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
            Color entityColor = r.color;
            if (e.hasComponent<LifeComponent>() && em.getComponent<LifeComponent>(e).countdown > 0)
            {
                entityColor = YELLOW;
                if (em.getComponent<LifeComponent>(e).life == 0)
                    em.destroyEntity(count);
            }

            engine.drawCube(r.position, r.scale.x(), r.scale.y(), r.scale.z(), entityColor);
            engine.drawCubeWires(r.position, r.scale.x(), r.scale.y(), r.scale.z(), MAROON);
        }
        count++;
    }

    endFrame(engine, em);
}

// Empieza el dibujado y se limpia la pantalla
void RenderSystem::beginFrame(ENGI::GameEngine& engine)
{
    engine.beginDrawing();
    engine.clearBackground(RAYWHITE);
    engine.beginMode3D();
    // engine.drawGrid(20, 10.f);
}

// Se termina el dibujado
void RenderSystem::endFrame(ENGI::GameEngine& engine, EntityManager& em)
{
    engine.endMode3D();
    drawHUD(em);
    engine.endDrawing();
}

// Se dibuja el HUD
void RenderSystem::drawHUD(EntityManager& em)
{
    auto& li = em.getSingleton<LevelInfo>();
    auto* playerEn = em.getEntityByID(li.playerID);
    if (not playerEn) { drawDeath(); return; }

    // Visualizar las vidas del player
    if (playerEn->hasTag<PlayerTag>() && playerEn->hasComponent<LifeComponent>())
    {
        auto const& l{ em.getComponent<LifeComponent>(*playerEn) };
        std::string vida = "Vidas: " + std::to_string(l.life);
        DrawText(vida.c_str(), 10, 10, 20, BLACK);
    }
}

void RenderSystem::drawDeath()
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));
    DrawText("HAS MUERTO", 250, 250, 40, RED);
}