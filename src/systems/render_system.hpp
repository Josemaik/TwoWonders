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

    void drawLogoGame(ENGI::GameEngine& engine, EntityManager& em, SoundSystem& ss);
    void drawLogoKaiwa(ENGI::GameEngine& engine);
    void drawOptions(ENGI::GameEngine& engine, EntityManager& em, SoundSystem& ss);
    void drawEnding(ENGI::GameEngine& engine);
    void drawStory(ENGI::GameEngine& engine);
    void unloadModels(EntityManager& em, ENGI::GameEngine& engine);
    void drawEditorInGameIA(ENGI::GameEngine& engine, EntityManager& em);
    void drawDebuggerInGameIA(ENGI::GameEngine& engine, EntityManager& em, double dt);
    void drawRay(vec3d origin,vec3d end);
    void drawVisionCone(vec3d pos_enemy,double orientation,double horizontalFOV);
    void drawPauseMenu(ENGI::GameEngine& engine, EntityManager& em, SoundSystem& ss);
    void drawInventory(ENGI::GameEngine& engine, EntityManager& em);
    void drawItemDescription(ENGI::GameEngine& engine, EntityManager& em, Item& item);
    void setShader(Shader& shader) { shaderPtr = &shader; }

    // Funciones double dtprivadas para organizar el codigo
private:
    void init();
    void beginFrame(ENGI::GameEngine& engine);
    void endFrame(ENGI::GameEngine& engine, EntityManager& em, double dt);
    void drawHUD(EntityManager& em, ENGI::GameEngine& engine, bool debugphy);
    void drawEntities(EntityManager& em, ENGI::GameEngine& engine);
    void drawDeath(ENGI::GameEngine& engine);
    void drawCoinBar(ENGI::GameEngine& engine, EntityManager& em);
    void drawHealthBar(ENGI::GameEngine& engine, EntityManager& em, const Entity& e);
    void drawManaBar(ENGI::GameEngine& engine, EntityManager& em);
    void loadModels(Entity& e, ENGI::GameEngine& engine, RenderComponent& r);
    void loadShaders(Model& model);
    void drawTextBox(ENGI::GameEngine& engine, EntityManager& em);

    bool isSelected{ false };
    bool isSelectedfordebug{ false }, fullScreen{ false };
    std::size_t pointedEntity{ std::numeric_limits<std::size_t>::max() };
    // bool chunk0Charged{ false };
    // bool chunk1Charged{ false };
    Shader* shaderPtr{ nullptr };

    float elapsed{ 0.f }, elapsed_limit{ 0.4f };
    float elapsed_WASD{ 0.f }, elapsed_limit_WASD{ 5.0f };
    float elapsed_CoinBar{ 0.f }, elapsed_limit_CoinBar{ 5.0f };

    int coinBarX{}, coinNumberX{};

    float endangle{0.0f};
};

#endif // !RENDER_SYSTEM
