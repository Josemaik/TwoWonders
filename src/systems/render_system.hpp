#pragma once
#ifndef RENDER_SYSTEM
#define RENDER_SYSTEM
#include <map>
#include "../utils/types.hpp"
#include "../managers/game_engine.hpp"
#include "../systems/sound_system.hpp"
#include "../utils/pf/Graph.hpp"

struct RenderSystem
{
    struct AnimatedTexture
    {
        std::string textureName;
        int targetPosX;
        int targetPosY;
        int width;
        int height;
        float scaleChange;
        float scaleFactor{};
        float lerpFactor{ 0.0f };
        float lerpSpeed{ 0.02f };
        float elapsed{ 0.0f };
    };

    struct ButtonRect
    {
        Rectangle rect{};
        const char* text{};
        int action{};
    };

    // Se van a buscar las entidad que tengan estos componentes y tags
    using SYSCMPs = MP::TypeList<PhysicsComponent, RenderComponent>;
    using SYSTAGs = MP::TypeList<>;

    RenderSystem() { init(); };

    void update(EntityManager& em, GameEngine& engine);
    void drawLogoGame(GameEngine& engine, EntityManager& em, SoundSystem& ss);
    void drawLogoKaiwa(GameEngine& engine);
    void drawOptions(GameEngine& engine, EntityManager& em, SoundSystem& ss);
    void drawEnding(GameEngine& engine);
    void drawStory(GameEngine& engine);
    void drawChargeScreen(GameEngine& engine, EntityManager& em);
    void drawControls(EntityManager& em, GameEngine& engine);
    void unloadModels(EntityManager& em, GameEngine& engine);
    void drawEditorInGameIA(GameEngine& engine, EntityManager& em);
    void drawDebuggerInGameIA(GameEngine& engine, EntityManager& em);
    void drawTestPathfindinf(ENGI::GameEngine& engine, EntityManager& em);
    void drawRay(vec3d origin, vec3d end);
    void drawVisionCone(vec3d pos_enemy, double orientation, double horizontalFOV);
    void drawPauseMenu(GameEngine& engine, EntityManager& em);
    void drawInventory(GameEngine& engine, EntityManager& em);
    void drawItemDescription(GameEngine& engine, EntityManager& em, Item& item);
    // void setShader(Shader& shader) { shaderPtr = &shader; }
    void resetAnimatedTexture() { animatedTextures.clear(); }
    // Funciones double dtprivadas para organizar el codigo
private:
    void init();
    void beginFrame(GameEngine& engine, EntityManager& em);
    void endFrame(GameEngine& engine, EntityManager& em);
    void drawHUD(EntityManager& em, GameEngine& engine);
    void initHUD(EntityManager& em, GameEngine& engine);
    void drawEntities(EntityManager& em, GameEngine& engine);
    void drawParticles(EntityManager& em, GameEngine& engine);
    void drawDeath(GameEngine& engine);
    void drawCoinBar(GameEngine& engine, EntityManager& em);
    void drawSpellSlots(GameEngine& engine, EntityManager& em);
    void drawSpellExp(GameEngine& engine, std::string name);
    void drawStaff(GameEngine& engine, EntityManager& em);
    void drawLockInfo(GameEngine& engine, EntityManager& em);
    void drawBoatParts(GameEngine& engine, EntityManager& em);
    void drawDebugPhysics(GameEngine& engine, EntityManager& em, LevelInfo& li);
    void drawAnimatedTextures(GameEngine& engine);
    void drawSmallButtons(GameEngine& engine, const std::string& name, int posX, int posY, int width, int height);
    void handleAnimatedTexture(const std::string& name, const std::string& textureName, int x, int y, const Texture2D& texture, float scaleFactor);
    void drawAlerts_IA(EntityManager& em, GameEngine& engine);
    void drawFPSCounter(GameEngine& engine);
    void loadModels(Entity& e, GameEngine& engine, EntityManager& em, RenderComponent& r);
    // void loadShaders(Model& model);
    void drawTextBox(GameEngine& engine, EntityManager& em);
    void displayGif(GameEngine& engine, TextureType& copy, GameEngine::Gif& gif, int& posX, int& posY);
    double shakeDouble(double value);

    void updateHealthBar(GameEngine& engine, EntityManager& em, const Entity& e);
    void updateManaBar(GameEngine& engine, EntityManager& em);
    void updateAnimatedTextures(GameEngine& engine);

    bool isSelected{ false };
    bool isSelectedfordebug{ false }, fullScreen{ false };
    std::size_t pointedEntity{ std::numeric_limits<std::size_t>::max() };
    std::map<std::string, AnimatedTexture> animatedTextures{};
    std::map<std::string, DarkMoon::Node*> HUDElements{};
    std::map<std::string, DarkMoon::Node*> debugIA{};
    std::map<std::string, DarkMoon::Node*> buttons{};
    std::map<std::string, DarkMoon::Node*> textElements{};
    // bool chunk0Charged{ false };
    // bool chunk1Charged{ false };
    // ShaderType* shaderPtr{ nullptr };

    void restartScene(GameEngine& engine);
    void changeScene(GameEngine& engine);

    float elapsed{ 0.0f }, elapsed_limit{ 0.4f };
    float elapsed_WASD{ 0.0f }, elapsed_limit_WASD{ 5.0f };
    float elapsed_CoinBar{ 0.0f }, elapsed_limit_CoinBar{ 5.0f };
    float elapsed_Lock{ 0.0f }, elapsed_limit_Lock{ 5.0f };
    float elapsed_spell{ 0.0f }, elapsed_limit_spell{ 5.0f };
    float elapsed_boat{ 0.0f }, elapsed_limit_boat{ 1.0f };
    float elapsed_book{ 0.0f };

    int coinBarX{}, coinNumberX{};
};

#endif // !RENDER_SYSTEM
