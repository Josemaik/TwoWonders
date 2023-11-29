#include "render_system.hpp"

void RenderSystem::update(EntityManager& em, ENGI::GameEngine& engine)
{

    // Actualizamos la posicion de render del componente de fisicas
    em.forEach<SYSCMPs, SYSTAGs>([&em](Entity&, PhysicsComponent& phy, RenderComponent& ren)
    {
        ren.setPosition(phy.position);
    });

    beginFrame(engine);

    //engine.drawAll(em);
    for (auto const& e : em.getEntities()){
        if (e.hasComponent<RenderComponent>()){
            auto const& r{ em.getComponent<RenderComponent>(e) };
            // Revisamos si es el jugador para mover la cámara
            if (e.hasTag<PlayerTag>()){
                engine.setPositionCamera({ r.position.x, 25.0f, r.position.z + 25.0f });
                engine.setTargetCamera(r.position);
            }

            DrawCube(r.position, r.scale.x, r.scale.y, r.scale.z, r.color);
            DrawCubeWires(r.position, r.scale.x, r.scale.y, r.scale.z, MAROON);
        }
    }

    endFrame(engine);
}

// Empieza el dibujado y se limpia la pantalla
void RenderSystem::beginFrame(ENGI::GameEngine&  engine){
    engine.beginDrawing();
    engine.clearBackground(RAYWHITE);
    engine.beginMode3D();
    engine.drawGrid(20, 1.0f);
}

// Se termina el dibujado
void RenderSystem::endFrame(ENGI::GameEngine&  engine){
    engine.endMode3D();
    engine.endDrawing();
}