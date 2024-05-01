#pragma once
#ifndef GAME_ENGINE
#define GAME_ENGINE
#include <cstdint>
#include <iostream>
#include <map>
#include "../utils/types.hpp"
#include "../utils/vec3D.hpp"
#include "../motor/src/darkmoon.hpp"

namespace ENGI {

    struct GameEngine
    {
        using CL = MP::TypeList<PhysicsComponent, RenderComponent>;
        using TL = MP::TypeList<>;

        using u16 = std::uint16_t;

        GameEngine(u16 const width, u16 const height);

        // Timing Related Functions
        void setTargetFPS(int fps);
        float getFrameTime();

        // Drawing
        void beginDrawing();
        void clearBackground(Color color);
        void drawGrid(int slices, float spacing);
        void endDrawing();
        void beginMode3D();
        void endMode3D();
        void drawLine3D(vec3d startPos, vec3d endPos, Color color);
        void drawPoint3D(vec3d position, float pointSize, Color color);
        void drawCube(vec3d pos, float width, float height, float lenght, Color color);
        void drawCubeWires(vec3d position, vec3d size, Color color);
        // void drawModel(ModelType model, vec3d position, vec3d rotationAxis, float rotationAngle, vec3d scale, Color tint);
        // void drawModelWires(ModelType model, vec3d position, vec3d rotationAxis, float rotationAngle, vec3d scale, Color tint);
        void drawLine(int startPosX, int startPosY, int endPosX, int endPosY, Color color);

        // Rectangle
        void drawRectangle(int posX, int posY, int width, int height, Color color);
        DarkMoon::Node* createRectangle(vec2d pos, vec2d size, Color color, const char* name, DarkMoon::Node* parentNode);
        void drawNode(DarkMoon::Node* node, vec2i pos = { -1, -1 }, vec2f scale = { 1.0f, 1.0f });
        void drawCircle(int posX, int posY, float radius, Color color);
        void drawCircleSector(vec2d center, float radius, float startAngle, float endAngle, int segments, Color color);
        void drawTriangle(vec2d v1, vec2d v2, vec2d v3, Color color);

        // Text
        void drawText(const char* text, int posX, int posY, int fontSize, Color color, Align aligned = Align::LEFT);
        Font* getFontDefault();

        // Window
        void initWindow(int width, int height, const char* title);
        void closeWindow();
        bool windowShouldClose();
        int getScreenWidth();
        int getScreenHeight();
        void setWindowSize(int width, int height);
        void setWindowFullScreen();
        void setExitKey(int key);

        // Camera
        void setPositionCamera(vec3d pos);
        void setTargetCamera(vec3d tar);
        void setUpCamera(vec3d up);
        void setFovyCamera(float fovy);
        void setProjectionCamera(DarkMoon::CameraProjection proj);
        vec3d getPositionCamera();
        vec3d getTargetCamera();
        vec3d getUpCamera();
        float getFovyCamera();

        // Input Handling
        bool isKeyPressed(int key);
        bool isKeyDown(int key);
        bool isKeyReleased(int key);
        bool isMouseButtonPressed(int button);
        bool isMouseButtonDown(int button);
        bool isGamepadAvailable(int gamepad);
        bool isGamepadButtonPressed(int gamepad, int button);
        bool isGamepadButtonDown(int gamepad, int button);
        bool isGamepadButtonReleased(int gamepad, int button);
        float getGamepadAxisMovement(int gamepad, int axis);

        DarkMoon::Node* loadModel(const char* filename);
        float getWorldToScreenX(vec3d pos);
        float getWorldToScreenY(vec3d pos);
        RayCast getMouseRay();
        void loadAndResizeImage(const char* name, const char* path, DarkMoon::Node* parentNode);
        void loadAndResizeImageGif(const char* name, const char* filePath);
        void unloadGifsAndTextures();
        void setReplayMode(bool replay, GameData& gd);
        double getTime();
        float getWidthRate();
        float getHeightRate();
        float getAspectRat();
        DarkMoon::Node* get2D();
        DarkMoon::Node* get3D();
        DarkMoon::Node* createTextBox(glm::vec2 position, glm::vec2 size, Color boxColor, std::string text, DarkMoon::Font* font, int fontSize, Color textColor, Align verticalAligned, Align horizontalAligned, const char* nodeName, DarkMoon::Node* parentNode);
        DarkMoon::Node* createText(glm::vec2 position, std::string text, DarkMoon::Font* font, int fontSize, Color textColor, const char* nodeName, DarkMoon::Node* parentNode, Align align = Align::LEFT);
        DarkMoon::Font* getDefaultFont();

        // DarkMoon Engine //

        DarkMoon::DarkMoonEngine dmeg;

        struct cmp_str
        {
            bool operator()(char const* a, char const* b) const
            {
                return std::strcmp(a, b) < 0;
            }
        };
        std::map<const char*, DarkMoon::Node*, cmp_str> nodes;

        DarkMoon::Node* createNode(const char* name, DarkMoon::Node* parentNode);
        DarkMoon::Node* createNode(DarkMoon::Node* copyNode, DarkMoon::Node* parentNode);

        DarkMoon::Texture* loadTexture2D(const char* filePath);
        std::vector<DarkMoon::Texture*> loadTextures2DAnim(const char* filePath);

    private:
        u16 const width_{}, height_{};
        static float widthRate, heightRate;
        bool replayMode{ false };
        GameData* gameData{ nullptr };

        DarkMoon::Camera* camera;
    };

#endif // !GAME_ENGINE
}