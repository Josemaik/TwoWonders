#pragma once

#include "../resources/resource_shader.hpp"
#include "../resources/resource_texture.hpp"
#include "../resources/resource_font.hpp"
#include "../resources/resource_mesh.hpp"
#include "../components/entity_camera.hpp"
#include "../components/entity_model.hpp"

#include "resource_manager.hpp"

#include "../utils/color.hpp"

#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct RenderManager{
private:
    Shader* m_shaderProgram;
    Font* m_defaultFont;
    int m_width, m_height;

    void draw(float vertices[], std::size_t vertSize, GLuint indices[], std::size_t indSize,glm::vec4 color);

public:
    Camera* m_camera;

    // Screen width and height
    void setWindowSize(int width, int height){ m_width = width; m_height = height; };

    // Drawing
    void beginMode3D();
    void endMode3D();

    // Camera
    void setCamera(Camera* camera){ m_camera = camera; };

    // Basic drawing functions
    void clearBackground(Color color);

    // Texture drawing functions
    void drawTexture(Texture* texture, glm::vec2 pos, Color color);
    void drawTextureExtra(Texture* texture, glm::vec2 pos, float rotation, float scale, Color color);

    // Basic geometric 3D shapes drawing functions
    void drawPoint3D(glm::vec3 position, float pointSize, Color color);
    void drawLine3D(glm::vec3 startPos, glm::vec3 endPos, float lineSize, Color color);
    void drawGrid(int slices, float spacing, Color color);
    void drawPlane(glm::vec3 centerPos, glm::vec2 size, Color color); // XZ
    void drawCube(glm::vec3 position, glm::vec3 size, Color color);
    void drawCubeWires(glm::vec3 position, glm::vec3 size, Color color);

    // Model drawing functions
    void drawModel(Model* model, glm::vec3 position, float scale, Color tint);
    void drawModelExtra(Model* model, glm::vec3 position, float scale, glm::vec3 rotationAxis, float rotationAngle, Color tint);
    void drawModelWires(Model* model, glm::vec3 position, float scale, Color tint);
    void drawModelWiresExtra(Model* model, glm::vec3 position, float scale, glm::vec3 rotationAxis, float rotationAngle, Color tint);

    // Mesh generation functions
    // TODO
    // Mesh* genMeshCube();

    // ChangeShader
    void useShader(Shader* shader){
        m_shaderProgram = shader;
        glUseProgram(m_shaderProgram->id_shader);
    };

    Shader* getShader(){
        return m_shaderProgram;
    }

    // Text
    void drawText(const char* text, glm::vec2 pos, int fontSize, Color color);

    static RenderManager& getInstance(){
        static RenderManager instance;
        return instance;
    }

    // Auxiliars
    glm::vec4 normalizeColor(const Color& color) {
        return glm::vec4(
            static_cast<float>(color.r) / 255.0f,
            static_cast<float>(color.g) / 255.0f,
            static_cast<float>(color.b) / 255.0f,
            static_cast<float>(color.a) / 255.0f
        );
    }
    float normalizeX(float x){ return (x / static_cast<float>(m_width)) * 2 - 1; };
    float normalizeY(float y){ return -((y / static_cast<float>(m_height)) * 2 - 1); };
    void setDefaultFont(Font* font){ m_defaultFont = font; };
};