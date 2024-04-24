#pragma once
#include "entity.hpp"
#include "../utils/color.hpp"
#include "../managers/render_manager.hpp"
#include "../managers/windows_manager.hpp"
#include "../managers/input_manager.hpp"
#include "../resources/resource_texture.hpp"   

#include <GL/gl.h>
#include <glm/glm.hpp>

#include <chrono>

namespace DarkMoon {

    struct Entity2D : Entity {
    protected:
        GLuint m_VAO = {}, m_VBO = {}, m_EBO = {};
        glm::mat4 m_transMatrix = {};

        virtual void changeVAO(glm::mat4&) {};
    };

    struct Pixel : Entity2D {
    private:
        void changeVAO(glm::mat4& transMatrix) override;

    public:
        Color color = { D_BLACK };

        Pixel(Color c);
        ~Pixel();

        void draw(glm::mat4 transMatrix) override;
    };

    struct Line : Entity2D {
    private:
        void changeVAO(glm::mat4& transMatrix) override;

    public:
        glm::vec2 startPos = {}, endPos = {};
        Color color = { D_BLACK };

        Line(glm::vec2 sP, glm::vec2 eP, Color c);
        ~Line();

        void draw(glm::mat4 transMatrix) override;
    };

    struct Triangle : Entity2D {
    private:
        void changeVAO(glm::mat4& transMatrix) override;

    public:
        glm::vec2 v1 = {}, v2 = {}, v3 = {};
        Color color = { D_BLACK };

        Triangle(glm::vec2 v_1, glm::vec2 v_2, glm::vec2 v_3, Color c);
        ~Triangle();

        void draw(glm::mat4 transMatrix) override;
    };

    struct Rectangle : Entity2D {
    private:
        void changeVAO(glm::mat4& transMatrix) override;
    public:
        glm::vec2 position = {};
        glm::vec2 size = {};
        Color color = { D_BLACK };

        Rectangle(glm::vec2 p, glm::vec2 s, Color c);
        ~Rectangle();

        void draw(glm::mat4 transMatrix) override;
    };

    struct Circle : Entity2D {
    private:
        void changeVAO(glm::mat4& transMatrix) override;
    public:
        glm::vec2 position = {};
        float radius = { 10.0f };
        int segments = { 20 };
        Color color = { D_BLACK };

        Circle(glm::vec2 p, float rad, int seg, Color c);
        ~Circle();

        void draw(glm::mat4 transMatrix) override;
    };

    struct Texture2D : Entity2D {
    private:
        void changeVAO(glm::mat4& transMatrix) override;
    public:
        glm::vec2 position = {};
        Texture* texture = { nullptr };
        Color color = { D_WHITE };

        Texture2D(glm::vec2 pos, Texture* text, Color col);
        // Constructor copia
        Texture2D(const Texture2D& other) {
            position = other.position;
            texture = other.texture;
            color = other.color;
        }
        ~Texture2D();

        void draw(glm::mat4 transMatrix) override;
    };

    struct AnimatedTexture2D : Entity {
        glm::vec2 position;
        std::vector<Texture*> frames;
        Color color;
        float frameDuration;
        float elapsedTime{ 0.0f };
        int currentFrame;

        AnimatedTexture2D(glm::vec2 pos = { 0.0f, 0.0f }, Color col = D_WHITE, float framD = 1.0f, int currF = 0)
            : position(pos), color(col), frameDuration(framD), currentFrame(currF) {};

        void draw(glm::mat4 transMatrix) override {
            // Check actual texture
            WindowsManager wm = WindowsManager::getInstance();

            elapsedTime += wm.getFrameTime();
            if (elapsedTime > frameDuration) {
                if (currentFrame + 1 > static_cast<int>(frames.size() - 1))
                    currentFrame = 0;
                else
                    currentFrame += 1;

                elapsedTime = 0;
            }
            auto texture = frames[currentFrame];

            // Draw texture
            RenderManager rm = RenderManager::getInstance();

            rm.useShader(rm.shaderTexture);

            auto nColor = rm.normalizeColor(color);

            // Apply Transformation Matrix
            position = glm::vec2(transMatrix[3][0], transMatrix[3][1]);

            // Top-left corner
            float auxWidth{ 0.0f }, auxHeight{ 0.0f };
            if (texture) {
                auxWidth = static_cast<float>(texture->getWidth()) * glm::length(glm::vec2(transMatrix[0][0], transMatrix[1][0]));
                auxHeight = static_cast<float>(texture->getHeight()) * glm::length(glm::vec2(transMatrix[1][1], transMatrix[1][0]));
            }

            // Define vertices and indices
            float vertices[] = {
                // positions                                                                         // colors                       // texture coords
                rm.normalizeX(position.x)           , rm.normalizeY(position.y)            , 0.0f,   nColor.x, nColor.y, nColor.z,   0.0f, 0.0f,
                rm.normalizeX(position.x + auxWidth), rm.normalizeY(position.y)            , 0.0f,   nColor.x, nColor.y, nColor.z,   1.0f, 0.0f,
                rm.normalizeX(position.x)           , rm.normalizeY(position.y + auxHeight), 0.0f,   nColor.x, nColor.y, nColor.z,   0.0f, 1.0f,
                rm.normalizeX(position.x + auxWidth), rm.normalizeY(position.y + auxHeight), 0.0f,   nColor.x, nColor.y, nColor.z,   1.0f, 1.0f
            };

            GLuint indices[] = { 0, 1, 2, 1, 2, 3 };

            // Create and configure VAO, VBO and EBO
            GLuint VBO, VAO, EBO;
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            // position attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            // color attribute
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            // texture coord attribute
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
            glEnableVertexAttribArray(2);

            // Colors
            GLint colorUniform = glGetUniformLocation(rm.getShader()->getIDShader(), "customColor");
            glUniform4fv(colorUniform, 1, glm::value_ptr(rm.normalizeColor(color)));

            // Apply Transformation Matrix
            //GLuint transformLoc = glGetUniformLocation(rm.getShader()->getIDShader(), "transform");
            //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transMatrix));

            if (texture) {
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                // Draw Texture
                glBindTexture(GL_TEXTURE_2D, texture->getIDTexture());
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                glBindTexture(GL_TEXTURE_2D, 0);
                glDisable(GL_BLEND);
            }

            // Clean up resources
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);

            rm.useShader(rm.shaderColor);
        };
    };

    // GUI

    struct Text : Entity {
        glm::vec2 position;
        std::string text;
        Font* font;
        int fontSize;
        Color color;

        float scale;
        float maxWidth;
        float maxHeight;

        Text(glm::vec2 pos = { 0.0f, 0.0f }, std::string txt = "", Font* f = nullptr, int fS = 10, Color col = D_BLACK)
            : position(pos), text(txt), font(f), fontSize(fS), color(col) {
            scale = static_cast<float>(fontSize) / 48.0f;
            if (font && !text.empty()) {
                std::string::const_iterator c;
                float aux_x = position.x;
                for (c = text.begin(); c != text.end(); c++) {
                    // Max Height
                    Character ch = font->characters[*c];
                    auto chY = static_cast<float>(ch.size.y);
                    if (maxHeight < chY)
                        maxHeight = chY;
                    // Max Width
                    aux_x += static_cast<float>(ch.advance >> 6) * scale;
                }
                maxHeight *= scale;
                maxWidth = aux_x - position.x;
            }
        };

        void draw(glm::mat4 transMatrix) override {
            RenderManager rm = RenderManager::getInstance();

            rm.useShader(rm.shaderText);

            position = glm::vec2(transMatrix[3][0], transMatrix[3][1]);

            if (!font)
                font = rm.defaultFont;
            if (text.empty())
                return;

            auto normColor = rm.normalizeColor(color);
            glUniform3f(glGetUniformLocation(rm.getShader()->getIDShader(), "textColor"), normColor.r, normColor.g, normColor.b);

            GLuint VAO, VBO;

            // Blend
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            scale = static_cast<float>(fontSize) / 48.0f;
            float aux_x = position.x;

            //std::cout << "-----------------------------\n";
            std::string::const_iterator c;
            for (c = text.begin(); c != text.end(); c++) {
                Character ch = font->characters[*c];
                auto chY = static_cast<float>(ch.size.y);
                if (maxHeight < chY)
                    maxHeight = chY;
            }
            maxHeight *= scale;

            // Iterate through all characters
            for (c = text.begin(); c != text.end(); c++) {
                Character ch = font->characters[*c];

                float posX = aux_x + static_cast<float>(ch.bearing.x) * scale;
                float posY = position.y - static_cast<float>(ch.bearing.y) * scale + maxHeight;

                float w = static_cast<float>(ch.size.x) * scale;
                float h = static_cast<float>(ch.size.y) * scale;

                // Update VBO for each character
                float vertices[6][4] = {
                    { rm.normalizeX(posX)    , rm.normalizeY(posY + h), 0.0f, 1.0f },
                    { rm.normalizeX(posX)    , rm.normalizeY(posY)    , 0.0f, 0.0f },
                    { rm.normalizeX(posX + w), rm.normalizeY(posY)    , 1.0f, 0.0f },

                    { rm.normalizeX(posX)    , rm.normalizeY(posY + h), 0.0f, 1.0f },
                    { rm.normalizeX(posX + w), rm.normalizeY(posY)    , 1.0f, 0.0f },
                    { rm.normalizeX(posX + w), rm.normalizeY(posY + h), 1.0f, 1.0f }
                };

                // Configure VAO/VBO for texture quads
                glGenVertexArrays(1, &VAO);
                glBindVertexArray(VAO);

                glGenBuffers(1, &VBO);
                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

                glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(0);

                glBindTexture(GL_TEXTURE_2D, ch.textureID);
                glDrawArrays(GL_TRIANGLES, 0, 6);

                // Advance cursors for next glyph
                aux_x += static_cast<float>(ch.advance >> 6) * scale;
            }
            maxWidth = aux_x - position.x;

            glDisable(GL_BLEND);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
            glBindTexture(GL_TEXTURE_2D, 0);

            // Clean up resources
            glDeleteBuffers(1, &VBO);
            glDeleteVertexArrays(1, &VAO);

            rm.useShader(rm.shaderColor);
        };
    };

    enum struct Aligned { LEFT, CENTER, RIGHT, TOP, BOTTOM };

    struct TextBox : Entity {
        Rectangle box;
        Rectangle boxBackground;
        Text text;
        float padding = 10.0f;

        bool drawBox{ true };

        Aligned verAligned;
        Aligned horAligned;

        TextBox(glm::vec2 pos = { 0.0f, 0.0f },
            glm::vec2 sz = { 100.0f, 50.0f },
            Color bCol = D_WHITE,
            std::string txt = "",
            Font* f = nullptr,
            int fS = 10,
            Color tCol = D_BLACK,
            Aligned verAl = Aligned::CENTER,
            Aligned horAl = Aligned::CENTER) :
            box(pos, sz, bCol), boxBackground({ pos.x - 1, pos.y - 1 }, { sz.x + 2, sz.y + 2 }, D_GRAY), text(pos, txt, f, fS, tCol), verAligned(verAl), horAligned(horAl) {};

        void draw(glm::mat4 transMatrix) override {
            if (drawBox) {
                // Rectangle background
                glm::mat4 aux = transMatrix;
                aux[3][0] -= 1;   aux[3][1] -= 1;
                boxBackground.draw(aux);
                // Rectangle
                box.draw(transMatrix);
            }

            // Vertical Aligned
            switch (verAligned) {
            case Aligned::TOP:
                transMatrix[3][1] = box.position.y + padding;
                break;
            case Aligned::CENTER:
                transMatrix[3][1] = box.position.y + (box.size.y - text.maxHeight) / 2;
                break;
            case Aligned::BOTTOM:
                transMatrix[3][1] = box.position.y + box.size.y - text.maxHeight - padding;
                break;
            default:
                break;
            }

            // Horizontal Aligned
            switch (horAligned) {
            case Aligned::LEFT:
                transMatrix[3][0] = box.position.x + padding;
                break;
            case Aligned::CENTER:
                transMatrix[3][0] = box.position.x + (box.size.x / 2 - text.maxWidth / 2);
                break;
            case Aligned::RIGHT:
                transMatrix[3][0] = box.position.x + box.size.x - text.maxWidth - padding;
                break;
            default:
                break;
            }

            // Text
            text.draw(transMatrix);
        };

    };

    enum struct ButtonState { NORMAL, HOVER, CLICK };

    struct Button : Entity {
        TextBox textBox;
        Color normalColor;
        Color hoverColor;
        Color clickColor;
        ButtonState state{ ButtonState::NORMAL };

        Button(glm::vec2 pos = { 0.0f, 0.0f },
            glm::vec2 sz = { 100.0f, 50.0f },
            Color bCol = D_WHITE,
            std::string txt = "",
            Font* f = nullptr,
            int fS = 10,
            Color tCol = D_BLACK,
            Aligned verAl = Aligned::CENTER,
            Aligned horAl = Aligned::CENTER,
            Color nColor = D_WHITE,
            Color hColor = D_GRAY,
            Color cColor = D_BLACK) :
            textBox(pos, sz, bCol, txt, f, fS, tCol, verAl, horAl), normalColor(nColor), hoverColor(hColor), clickColor(cColor) {};

        void draw(glm::mat4 transMatrix) override {
            // Color
            WindowsManager& wm = WindowsManager::getInstance();
            InputManager& im = InputManager::getInstance();

            glm::vec2 mPos = { im.getMouseX(wm.getWindow()), im.getMouseY(wm.getWindow()) };
            glm::vec2 topLeft = textBox.box.position;
            glm::vec2 bottomRight = { topLeft.x + textBox.box.size.x, topLeft.y + textBox.box.size.y };

            if (mPos.x >= topLeft.x && mPos.x <= bottomRight.x &&
                mPos.y >= topLeft.y && mPos.y <= bottomRight.y) {
                if (im.isMouseButtonReleased(GLFW_MOUSE_BUTTON_LEFT)) {
                    textBox.box.color = clickColor;
                    state = ButtonState::CLICK;
                }
                else {
                    textBox.box.color = hoverColor;
                    state = ButtonState::HOVER;
                }
            }
            else {
                textBox.box.color = normalColor;
                state = ButtonState::NORMAL;
            }

            // Text box
            textBox.draw(transMatrix);
        };
    };

    struct Slider : Entity {
        Rectangle background;
        Rectangle boxBackground;
        Rectangle slider;
        float valor = 0.5f; // 0 -> 1

        Slider(glm::vec2 pos = { 0.0f, 0.0f },
            glm::vec2 sz = { 100.0f, 50.0f },
            Color bCol = D_GRAY,
            Color sCol = D_WHITE) :
            background(pos, sz, bCol), boxBackground({ pos.x - 1, pos.y - 1 }, { sz.x + 2, sz.y + 2 }, D_GRAY), slider({ pos.x + 1, pos.y + 1 }, { sz.x - 2, sz.y - 2 }, sCol) {};

        void draw(glm::mat4 transMatrix) override {
            // Rectangle box background
            glm::mat4 aux = transMatrix;
            aux[3][0] -= 1;   aux[3][1] -= 1;
            boxBackground.draw(aux);

            // Rectangle background
            background.draw(transMatrix);

            // Rectangle slider
            aux = transMatrix;
            aux[3][0] += 1;   aux[3][1] += 1;

            WindowsManager& wm = WindowsManager::getInstance();
            InputManager& im = InputManager::getInstance();

            glm::vec2 mPos = { im.getMouseX(wm.getWindow()), im.getMouseY(wm.getWindow()) };
            glm::vec2 topLeft = slider.position;
            glm::vec2 bottomRight = { topLeft.x + slider.size.x, topLeft.y + slider.size.y };

            if (mPos.x >= topLeft.x && mPos.x <= bottomRight.x &&
                mPos.y >= topLeft.y && mPos.y <= bottomRight.y)
                if (im.isMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT))
                    valor = (mPos.x - topLeft.x) / (bottomRight.x - topLeft.x);

            aux[0][0] = valor;
            slider.draw(aux);
        };
    };

    // TODO

    struct ButtonTexture : Entity {

    };

    struct CheckBox : Entity {

    };
}