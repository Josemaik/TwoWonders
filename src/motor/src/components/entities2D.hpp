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

static constexpr double K_PI2 = 3.14159265358979323846;

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
        Texture2D(const Texture2D& other);
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
            // WindowsManager wm = WindowsManager::getInstance();

            elapsedTime += 1 / 60.0f;
            if (elapsedTime > frameDuration) {
                if (currentFrame + 1 > static_cast<int>(frames.size() - 1))
                    currentFrame = 0;
                else
                    currentFrame += 1;

                elapsedTime = 0;
            }
            auto texture = frames[currentFrame];

            // Draw texture
            RenderManager& rm = RenderManager::getInstance();

            rm.useShader(rm.shaders["texture"]);

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

            rm.useShader(rm.shaders["color"]);
        };
    };

    // GUI
    enum struct Aligned { LEFT, CENTER, RIGHT, TOP, BOTTOM };

    struct Text : Entity {
        glm::vec2 position{};
        Font* font{};
        int fontSize{};
        Color color{};

        float scale{};
        float maxWidth{};
        float maxHeight{};
        std::vector<float> widths{};
        Aligned alignment{};

        GLuint VAO{}, VBO{};
        GLint textColorLocation{};
        RenderManager* rm{ nullptr };
        WindowsManager* wm{ nullptr };
        float ratio{};

        Text(glm::vec2 pos = { 0.0f, 0.0f }, std::string txt = "", Font* f = nullptr, int fS = 10, Color col = D_BLACK, Aligned al = Aligned::LEFT)
            : position(pos), font(f), fontSize(fS), color(col), alignment(al)
        {
            // Get ratio
            wm = &WindowsManager::getInstance();
            ratio = wm->getHeightRatio();

            setText(txt);

            // Buffers
            setupBuffers();

            // Get uniform location
            rm = &RenderManager::getInstance();
            rm->useShader(rm->shaders["text"]);
            textColorLocation = glGetUniformLocation(rm->getShader()->getIDShader(), "textColor");
        };

        void setText(std::string text)
        {
            std::wstring textW{};
            textW.resize(text.size());
            std::mbstowcs(&textW[0], text.c_str(), text.size());
#ifdef _WIN32
            bool checkSpecial = false;
#endif
            if (font && !text.empty() && this->text != textW) {
                // Reset values
                widths.clear();
                float lineWidth = position.x;

                // Seteamos la escala con el ratio
                scale = static_cast<float>(fontSize) / 34.0f * ratio;

                for (wchar_t c : textW) {
                    if (c == '\n') {
                        widths.push_back(lineWidth - position.x);
                        lineWidth = position.x;
                        if (maxWidth < lineWidth)
                            maxWidth = lineWidth;

                        continue;
                    }
#ifdef _WIN32
                    else if (checkSpecial)
                    {
                        c += 64;
                        checkSpecial = false;
                    }
                    else if (c == 195)
                    {
                        checkSpecial = true;
                        continue;
                    }
#endif

                    // Max Height
                    Character ch = font->characters[c];
                    auto chY = static_cast<float>(ch.size.y);
                    if (maxHeight < chY)
                        maxHeight = chY;
                    lineWidth += static_cast<float>(ch.advance >> 6) * scale;
                }
                maxHeight *= scale;
                widths.push_back(lineWidth - position.x);
                if (maxWidth < lineWidth)
                    maxWidth = lineWidth;

                this->text = textW;
            }
        }

        void setupBuffers() {
            // Enable blending
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            // Generate VAO and VBO
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);

            // Configure VAO/VBO for texture quads
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
        }

        ~Text() {
            // Delete VAO and VBO
            glDeleteBuffers(1, &VBO);
            glDeleteVertexArrays(1, &VAO);
        };

        void draw(glm::mat4 transMatrix) override {
            RenderManager& rm = RenderManager::getInstance();

            if (ratio != wm->getHeightRatio())
            {
                auto lastRatio = ratio;
                ratio = wm->getHeightRatio();
                scale = static_cast<float>(fontSize) / 34.0f * ratio;
                for (auto& w : widths)
                    w *= ratio / lastRatio;
            }

            rm.useShader(rm.shaders["text"]);

            position = glm::vec2(transMatrix[3][0], transMatrix[3][1]);

            if (!font)
                font = rm.defaultFont;
            if (text.empty())
                return;

            auto normColor = rm.normalizeColor(color);
            glUniform3f(textColorLocation, normColor.r, normColor.g, normColor.b);

            // Blend
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            // Assign the position
            float aux_x = position.x;

            if (alignment == Aligned::CENTER)
                aux_x -= widths[0] / 2;
            else if (alignment == Aligned::RIGHT)
                aux_x -= -widths[0];

            int numLines = 1;

            for (wchar_t& c : text) {
                Character ch = font->characters[c];
                auto chY = static_cast<float>(ch.size.y);
                if (maxHeight < chY)
                    maxHeight = chY;

                if (c == '\n')
                    numLines++;
            }
            maxHeight *= scale;

            // Iterate through all characters
            auto offSetY = maxHeight * 0.8f;
            if (numLines != 1)
            {
                offSetY = maxHeight / 2;
                position.y -= maxHeight * static_cast<float>(numLines) * 0.6f;
            }

            int i{ 1 };
#ifdef _WIN32
            bool checkSpecial = false;
#endif
            for (wchar_t& c : text) {
                if (c == '\n') {
                    // Reset the x position to the start of the line
                    aux_x = position.x;

                    if (alignment == Aligned::CENTER)
                        aux_x -= widths[i] / 2;
                    else
                        aux_x -= widths[i];

                    i += 1;
                    position.y += maxHeight * 1.2f;
                    // Skip the rest of the loop
                    continue;
                }
#ifdef _WIN32
                else if (checkSpecial)
                {
                    c += 64;
                    checkSpecial = false;
                }
                else if (c == 195)
                {
                    checkSpecial = true;
                    continue;
                }
#endif

                Character ch = font->characters[c];

                float posX = aux_x + static_cast<float>(ch.bearing.x) * scale;
                float posY = position.y - static_cast<float>(ch.bearing.y) * scale + offSetY;

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
                glBindVertexArray(VAO);
                glBindBuffer(GL_ARRAY_BUFFER, VBO);

                glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

                glBindTexture(GL_TEXTURE_2D, ch.textureID);
                glDrawArrays(GL_TRIANGLES, 0, 6);

                // Advance cursors for next glyph
                aux_x += static_cast<float>(ch.advance >> 6) * scale;
            }

            // Clean up resources
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
            glBindTexture(GL_TEXTURE_2D, 0);

            rm.useShader(rm.shaders["color"]);
        };

        std::wstring getText() {
            return text;
        }

    private:
        std::wstring text{};
    };

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
            box(pos, sz, bCol), boxBackground({ pos.x - 1, pos.y - 1 }, { sz.x + 2, sz.y + 2 }, D_GRAY), text(pos, txt, f, fS, tCol, horAl), verAligned(verAl), horAligned(horAl) {};

        void draw(glm::mat4 transMatrix) override {
            if (drawBox) {
                // Rectangle background
                glm::mat4 aux = transMatrix;
                aux[3][0] -= 1;   aux[3][1] -= 1;
                boxBackground.draw(aux);
                // Rectangle
                box.draw(transMatrix);
            }

            WindowsManager& wm = WindowsManager::getInstance();
            auto wRat = wm.getWidthRatio();
            auto hRat = wm.getHeightRatio();

            // Vertical Aligned
            switch (verAligned) {
            case Aligned::TOP:
                transMatrix[3][1] = box.position.y + padding;
                break;
            case Aligned::CENTER:
                transMatrix[3][1] = box.position.y + (box.size.y - text.maxHeight) * hRat / 2;
                break;
            case Aligned::BOTTOM:
                transMatrix[3][1] = box.position.y + box.size.y * hRat - text.maxHeight - padding;
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
                transMatrix[3][0] = box.position.x + box.size.x * wRat / 2;
                break;
            case Aligned::RIGHT:
                transMatrix[3][0] = box.position.x + box.size.x * wRat - text.maxWidth - padding;
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
        WindowsManager& wm = WindowsManager::getInstance();
        InputManager& im = InputManager::getInstance();

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
            textBox(pos, sz, bCol, txt, f, fS, tCol, verAl, horAl), normalColor(nColor), hoverColor(hColor), clickColor(cColor)
        {
            textBox.box.position = pos;
            checkMouse();
        };

        void draw(glm::mat4 transMatrix) override {
            // Color
            checkMouse();
            // Text box
            textBox.draw(transMatrix);
        };

        void checkMouse()
        {
            glm::vec2 mPos = { im.getMouseX(wm.getWindow()), im.getMouseY(wm.getWindow()) };
            glm::vec2 topLeft = textBox.box.position;
            glm::vec2 bottomRight = { topLeft.x + textBox.box.size.x * wm.getWidthRatio(), topLeft.y + textBox.box.size.y * wm.getHeightRatio() };

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
        }
    };

    struct Slider : Entity {
        Rectangle background;
        Rectangle boxBackground;
        Rectangle slider;
        WindowsManager& wm = WindowsManager::getInstance();
        InputManager& im = InputManager::getInstance();
        float valor{}; // 0 -> 1

        Slider(glm::vec2 pos = { 0.0f, 0.0f },
            glm::vec2 sz = { 100.0f, 50.0f },
            float value = 0.5f,
            Color bCol = D_GRAY,
            Color sCol = D_WHITE) :
            background(pos, sz, bCol), boxBackground({ pos.x - 1, pos.y - 1 }, { sz.x + 2, sz.y + 2 }, D_GRAY), slider({ pos.x + 1, pos.y + 1 }, { sz.x - 2, sz.y - 2 }, sCol), valor{ value }
        {
            slider.position = { pos.x + 1, pos.y + 1 };
            checkMouse();
        };

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

            checkMouse();

            aux[0][0] = valor;
            slider.draw(aux);
        };

        void checkMouse()
        {
            glm::vec2 mPos = { im.getMouseX(wm.getWindow()), im.getMouseY(wm.getWindow()) };
            glm::vec2 topLeft = slider.position;
            glm::vec2 bottomRight = { topLeft.x + slider.size.x, topLeft.y + slider.size.y };

            if (mPos.x >= topLeft.x && mPos.x <= bottomRight.x &&
                mPos.y >= topLeft.y && mPos.y <= bottomRight.y)
                if (im.isMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT))
                    valor = (mPos.x - topLeft.x) / (bottomRight.x - topLeft.x);
        }
    };

    // TODO

    struct CheckBox : Entity {

    };
}