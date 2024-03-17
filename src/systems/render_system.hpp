#pragma once
#ifndef RENDER_SYSTEM
#define RENDER_SYSTEM
#include "../utils/types.hpp"
#include "../managers/game_engine.hpp"
#include "../systems/sound_system.hpp"

struct RenderSystem
{
    // Se van a buscar las entidad que tengan estos componentes y tags
    using SYSCMPs = MP::TypeList<PhysicsComponent, RenderComponent>;
    using SYSTAGs = MP::TypeList<>;

    RenderSystem() { init(); };

    void update(EntityManager& em, GameEngine& engine, double dt);

    void drawLogoGame(GameEngine& engine, EntityManager& em, SoundSystem& ss);
    void drawLogoKaiwa(GameEngine& engine);
    void drawOptions(GameEngine& engine, EntityManager& em, SoundSystem& ss);
    void drawEnding(GameEngine& engine);
    void drawStory(GameEngine& engine);
    void drawChargeScreen(GameEngine& engine, EntityManager& em);
    void unloadModels(EntityManager& em, GameEngine& engine);
    void drawEditorInGameIA(GameEngine& engine, EntityManager& em);
    void drawDebuggerInGameIA(GameEngine& engine, EntityManager& em, double dt);
    void drawRay(vec3d origin, vec3d end);
    void drawVisionCone(vec3d pos_enemy, double orientation, double horizontalFOV);
    void drawPauseMenu(GameEngine& engine, EntityManager& em, SoundSystem& ss);
    void drawInventory(GameEngine& engine, EntityManager& em);
    void drawItemDescription(GameEngine& engine, EntityManager& em, Item& item);
    void setShader(Shader& shader) { shaderPtr = &shader; }

    // Funciones double dtprivadas para organizar el codigo
private:
    void init();
    void beginFrame(GameEngine& engine);
    void endFrame(GameEngine& engine, EntityManager& em, double dt);
    void drawHUD(EntityManager& em, GameEngine& engine, bool debugphy);
    void drawEntities(EntityManager& em, GameEngine& engine);
    void drawDeath(GameEngine& engine);
    void drawCoinBar(GameEngine& engine, EntityManager& em);
    void drawHealthBar(GameEngine& engine, EntityManager& em, const Entity& e);
    void drawManaBar(GameEngine& engine, EntityManager& em);
    void drawAlerts_IA(EntityManager& em, ENGI::GameEngine& engine, double dt);
    void loadModels(Entity& e, GameEngine& engine, EntityManager& em, RenderComponent& r);
    void loadShaders(Model& model);
    void drawTextBox(GameEngine& engine, EntityManager& em);
    void displayGif(GameEngine& engine, Texture2D& copy, GameEngine::Gif& gif, int& posX, int& posY);

    bool isSelected{ false };
    bool isSelectedfordebug{ false }, fullScreen{ false };
    std::size_t pointedEntity{ std::numeric_limits<std::size_t>::max() };
    // bool chunk0Charged{ false };
    // bool chunk1Charged{ false };
    Shader* shaderPtr{ nullptr };

    float elapsed{ 0.f }, elapsed_limit{ 0.4f };
    float elapsed_WASD{ 0.f }, elapsed_limit_WASD{ 50.0f };
    float elapsed_CoinBar{ 0.f }, elapsed_limit_CoinBar{ 5.0f };
    float elapsed_Lock{ 0.f }, elapsed_limit_Lock{ 5.0f };

    int coinBarX{}, coinNumberX{};
};

#endif // !RENDER_SYSTEM
