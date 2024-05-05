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
        void drawCubeWires(vec3d position, vec3d size, Color color);
        // void drawModel(ModelType model, vec3d position, vec3d rotationAxis, float rotationAngle, vec3d scale, Color tint);
        // void drawModelWires(ModelType model, vec3d position, vec3d rotationAxis, float rotationAngle, vec3d scale, Color tint);
        void drawLine(int startPosX, int startPosY, int endPosX, int endPosY, Color color);

        void drawNode(Node* node, vec2i pos = { -1, -1 }, vec2f scale = { 1.0f, 1.0f });
        void drawCircle(int posX, int posY, float radius, Color color);
        void drawCircleSector(vec2d center, float radius, float startAngle, float endAngle, int segments, Color color);
        void drawTriangle(vec2d v1, vec2d v2, vec2d v3, Color color);

        // Rectangle
        Node* drawRectangle(vec2i pos, vec2i size, Color color);
        Node* drawRectangle(vec2i pos, vec2i size, Color color, Node* parent);
        Node* createRectangle(vec2i pos, vec2i size, Color color, const char* name, Node* parentNode);

        // Slider
        Node* drawSlider(vec2i position, vec2i size, float value, Color backColor, Color sliderColor);
        Node* createSlider(vec2i position, vec2i size, float value, Color backColor, Color sliderColor, const char* nodeName, Node* parentNode);

        // Option Slider
        Node* drawOptionSlider(vec2i pos, vec2i sz, Color bCol, std::string txt, Font* f, int fS, int fsArrows, Color tCol, Aligned verAl, Aligned horAl, Color nColor, Color hColor, Color cColor, std::vector<std::string> opts, std::string firstOption);
        Node* createOptionSlider(vec2i pos, vec2i sz, Color bCol, std::string txt, Font* f, int fS, int fsArrows, Color tCol, Aligned verAl, Aligned horAl, Color nColor, Color hColor, Color cColor, std::vector<std::string> opts, std::string firstOption, const char* nodeName, Node* parentNode);

        // Float Slider
        Node* drawFloatSlider(vec2i pos, vec2i sz, Color bCol, std::string txt, Font* f, int fS, int fsArrows, Color tCol, Aligned verAl, Aligned horAl, Color nColor, Color hColor, Color cColor, float initialValue);
        Node* createFloatSlider(vec2i pos, vec2i sz, Color bCol, std::string txt, Font* f, int fS, int fsArrows, Color tCol, Aligned verAl, Aligned horAl, Color nColor, Color hColor, Color cColor, float initialValue, const char* nodeName, Node* parentNode);

        // Button
        Node* drawButton(vec2i position, vec2i size, std::string text, Font* font, int fontSize, Color textColor, Aligned verticalAligned, Aligned horizontalAligned, Color normalColor, Color hoverColor, Color clickColor);
        Node* createButton(vec2i position, vec2i size, std::string text, Font* font, int fontSize, Color textColor, Aligned verticalAligned, Aligned horizontalAligned, Color normalColor, Color hoverColor, Color clickColor, const char* nodeName, Node* parentNode);
        Node* createButton(vec2i position, vec2i size, std::string text, const char* nodeName, Node* parentNode);

        // CheckBox
        Node* drawCheckBox(vec2i position, float size, bool checked, Color backgroundColor, Color normalColor, Color hoverColor);
        Node* createCheckBox(vec2i position, float size, bool checked, Color backgroundColor, Color normalColor, Color hoverColor, const char* nodeName, Node* parentNode);
        Node* createCheckBoxPtr(vec2i position, float size, bool* checked, Color backgroundColor, Color normalColor, Color hoverColor, const char* nodeName, Node* parentNode);

        // Cube
        Node* drawCube(vec3d position, vec3d size, DarkMoon::Color color);
        Node* createCube(vec3d position, vec3d size, DarkMoon::Color color, const char* nodeName, Node* parentNode);

        // Text
        Node* drawText(const char* text, int posX, int posY, int fontSize, Color color, Aligned aligned = Aligned::LEFT);
        Node* createText(vec2i position, std::string text, Font* font, int fontSize, Color textColor, const char* nodeName, Node* parentNode, Aligned align = Aligned::LEFT);
        Node* createText(vec2i position, std::string text, Color c, const char* nodeName, Node* parentNode, int fontSize = 20);
        Node* createTextBox(vec2i position, vec2i size, Color boxColor, std::string text, Font* font, int fontSize, Color textColor, Aligned verticalAligned, Aligned horizontalAligned, const char* nodeName, Node* parentNode);
        Font* getFontDefault();

        // Line 3D
        Node* drawLine3D(vec3d startPos, vec3d endPos, float lSize, Color color);
        Node* createLine3D(vec3d startPos, vec3d endPos, float lSize, Color color, const char* nodeName, Node* parentNode);

        // Point 3D
        Node* drawPoint3D(vec3d position, float pointSize, Color color);
        Node* createPoint3D(vec3d position, float pointSize, Color color, const char* nodeName, Node* parentNode);

        // Lights
        void toggleLights();
        void activateLights();
        void deactivateLights();

        // Puntual Light
        Node* drawPuntualLight(vec3d position, Color color);
        Node* createPuntualLight(vec3d position, Color color, const char* nodeName, Node* parentNode);

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
        void setProjectionCamera(CameraProjection proj);
        vec3d getPositionCamera();
        vec3d getTargetCamera();
        vec3d getUpCamera();
        float getFovyCamera();

        // Input Handling
        bool isKeyPressed(int key);
        bool isAnyKeyPressed();
        bool isKeyDown(int key);
        bool isAnyKeyDown();
        bool isKeyReleased(int key);
        bool isAnyKeyReleased();
        bool isMouseButtonPressed(int button);
        bool isMouseButtonDown(int button);
        bool isGamepadAvailable(int gamepad);
        bool isGamepadButtonPressed(int gamepad, int button);
        bool isGamepadButtonDown(int gamepad, int button);
        bool isGamepadButtonReleased(int gamepad, int button);
        float getGamepadAxisMovement(int gamepad, int axis);

        Node* loadModel(const char* filename);
        float getWorldToScreenX(vec3d pos);
        float getWorldToScreenY(vec3d pos);
        RayCast getMouseRay();
        void loadAndResizeImage(const char* name, const char* path, Node* parentNode);
        void loadAndResizeImageGif(const char* name, const char* filePath);
        void unloadGifsAndTextures();
        void setReplayMode(bool replay, GameData& gd);
        double getTime();
        float getWidthRate();
        float getHeightRate();
        float getAspectRat();
        Node* get2D();
        Node* get3D();
        Font* getDefaultFont();
        void traverseRoot();
        void nodeClear(Node* node);

        // DarkMoon Engine //

        DarkMoonEngine dmeg;

        std::map<std::string, Node*> nodes;
        std::map<Node*, std::pair<vec2i, vec2f>> nodesToDraw{};

        Node* createNode(const char* name, Node* parentNode);
        Node* createNode(Node* copyNode, Node* parentNode);

        Texture* loadTexture2D(const char* filePath);
        std::vector<Texture*> loadTextures2DAnim(const char* filePath);

    private:
        u16 const width_{}, height_{};
        static float widthRate, heightRate;
        bool replayMode{ false };
        GameData* gameData{ nullptr };

        Camera* camera;
    };

#endif // !GAME_ENGINE
}