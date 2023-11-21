#include "game_engine.hpp"

GameEngine::GameEngine(u16 const width, u16 const height)
    : width_{ width }, height_{ height }
{
    InitWindow(width_, height_, "Zelda");
    SetTargetFPS(30);

    camera.position = { 0.0f, 25.0f, 25.0f };
    camera.target = { 0.0f, 03.0f, .0f };
    camera.up = { 0.0f, 01.0f, 0.0f };
    camera.fovy = 30.0f;
    camera.projection = CAMERA_PERSPECTIVE;
}

void GameEngine::beginFrame()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    BeginMode3D(camera);
    DrawGrid(20, 1.0f);
}

void GameEngine::drawAll(EntityManager<PhysicsComponent, RenderComponent, Entity>& em)
{
    for (auto const& e : em.getEntities())
    {
        auto const& r{ e.render };

        camera.position = { r.position.x, 25.0f, r.position.z + 25.0f };
        camera.target = r.position;
        DrawCube(r.position, r.scale.x, r.scale.y, r.scale.z, r.color);
        DrawCube({ 2, 0, -2 }, r.scale.x, r.scale.y, r.scale.z, GREEN);
        DrawCubeWires(r.position, r.scale.x, r.scale.y, r.scale.z, MAROON);
    }
}

void GameEngine::endFrame()
{
    EndMode3D();
    EndDrawing();
}

void GameEngine::endWindow()
{
    CloseWindow();
}

bool GameEngine::run()
{
    return !WindowShouldClose();
}