#include "render_system.hpp"

#include <iomanip>

bool RenderSystem::update(EntityManager& em, ENGI::GameEngine& engine)
{

    // Actualizamos la posicion de render del componente de fisicas
    em.forEach<SYSCMPs, SYSTAGs>([](Entity&, PhysicsComponent& phy, RenderComponent& ren)
    {
        ren.setPosition(phy.position);
    });

    beginFrame(engine);

    // Dibuja todas las entidades con componente de render
    bool notReset = drawEntities(em, engine);

    endFrame(engine, em);

    return notReset;
}

bool RenderSystem::drawEntities(EntityManager& em, ENGI::GameEngine& engine)
{
    auto const& entities = em.getEntities();
    if (entities.empty())
        return false;

    for (auto const& e : em.getEntities())
    {
        if (e.hasComponent<RenderComponent>())
        {
            auto const& r{ em.getComponent<RenderComponent>(e) };
            if (r.visible) {
                // Revisamos si es el jugador para mover la cámara
                if (e.hasTag<PlayerTag>())
                {
                    engine.setPositionCamera({ r.position.x(), 25.0f, r.position.z() + 25.0f });
                    engine.setTargetCamera(r.position);
                }
                // Comprobar si tiene el componente vida
                Color colorEntidad = r.color;
                if (e.hasComponent<LifeComponent>()) {
                    auto& l{ em.getComponent<LifeComponent>(e) };
                    if (l.elapsed < l.countdown)
                        colorEntidad = YELLOW;
                }
                if (!e.hasTag<ZoneTag>())
                    engine.drawCube(r.position, r.scale.x(), r.scale.y(), r.scale.z(), colorEntidad);
                engine.drawCubeWires(r.position, r.scale.x(), r.scale.y(), r.scale.z(), BLACK);
            }
        }
    }

    return true;
}

// Empieza el dibujado y se limpia la pantalla
void RenderSystem::beginFrame(ENGI::GameEngine& engine)
{
    engine.beginDrawing();

    engine.clearBackground(RAYWHITE);

    engine.beginMode3D();
    //engine.drawGrid(50, 1.f);
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
    if (not playerEn) { drawDeath(engine); return; }
    else if (!playerEn->hasTag<PlayerTag>()) { drawDeath(engine); return; }

    // Visualizar las vidas del player
    for (auto const& e : em.getEntities())
    {
        if (e.hasTag<PlayerTag>())
        {
            // Dibujar background HUD
            engine.drawRectangle(0, 0, 525, 60, WHITE);
            engine.drawRectangle(0, 50, 100, 120, WHITE);

            // Dibujar vidas restantes del player en el HUD
            if (e.hasComponent<LifeComponent>())
            {
                auto const& l{ em.getComponent<LifeComponent>(e) };
                std::string vida = "Vidas: " + std::to_string(l.life) + " (max " + std::to_string(l.maxLife) + ") - ";
                engine.drawText(vida.c_str(), 10, 10, 20, BLACK);
            }

            // Dibujar las bombas y monedas actuales
            if (e.hasComponent<InformationComponent>())
            {
                auto const& info{ em.getComponent<InformationComponent>(e) };
                std::string info_text = "Bombas: " + std::to_string(info.bombs) + " (max " + std::to_string(info.max_bombs) + ") - Monedas: " + std::to_string(info.coins);
                engine.drawText(info_text.c_str(), 200, 10, 20, BLACK);
            }

            // Dibujar el countdown restante del ataque del player en el HUD
            if (e.hasComponent<AttackComponent>())
            {
                auto const& a{ em.getComponent<AttackComponent>(e) };
                std::string countdown_ataque;
                if (a.elapsed > a.countdown)
                    countdown_ataque = "Ataque listo";
                else
                    countdown_ataque = "Ataque listo en: " + std::to_string(-1 * (a.elapsed - 1.0f)) + " segundos";

                engine.drawText(countdown_ataque.c_str(), 10, 35, 20, BLACK);
            }

            // Dibujar la posicion del player en el HUD // DEBUG
            if (e.hasComponent<RenderComponent>())
            {
                engine.drawText("Posicion", 10, 70, 20, BLACK);
                auto const& r{ em.getComponent<RenderComponent>(e) };
                std::string posX = "X: " + std::to_string(static_cast<int>(r.position.x()));
                engine.drawText(posX.c_str(), 10, 95, 20, BLACK);
                std::string posY = "Y: " + std::to_string(static_cast<int>(r.position.y()));
                engine.drawText(posY.c_str(), 10, 120, 20, BLACK);
                std::string posZ = "Z: " + std::to_string(static_cast<int>(r.position.z()));
                engine.drawText(posZ.c_str(), 10, 145, 20, BLACK);
            }
        }

        // Dibujar la vida restante encima de las entidades // DEBUG
        if (e.hasComponent<LifeComponent>() && em.getComponent<RenderComponent>(e).visible)
        {
            auto const& r{ em.getComponent<RenderComponent>(e) };
            auto const& l{ em.getComponent<LifeComponent>(e) };

            engine.drawText(std::to_string(l.life).c_str(),
                static_cast<int>(engine.getWorldToScreenX(r.position) - 5),
                static_cast<int>(engine.getWorldToScreenY(r.position) - r.scale.y() * 50),
                20,
                BLACK);
        }

        // Dibujar el ID de las entidades // DEBUG
        // auto const& r{ em.getComponent<RenderComponent>(e) };
        // engine.drawText(std::to_string(e.getID()).c_str(),
        //     static_cast<int>(engine.getWorldToScreenX(r.position) - 5),
        //     static_cast<int>(engine.getWorldToScreenY(r.position) - r.scale.y() * 50),
        //     20,
        //     BLACK);
    }
}

void RenderSystem::drawDeath(ENGI::GameEngine& engine)
{
    engine.drawRectangle(0, 0, engine.getScreenWidth(), engine.getScreenHeight(), Fade(BLACK, 0.5f));
    engine.drawText("HAS MUERTO", 250, 250, 40, RED);
    engine.drawText("[ENTER] para volver a jugar", 165, 300, 30, RED);
}