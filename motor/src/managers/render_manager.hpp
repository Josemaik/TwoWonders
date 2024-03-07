#pragma once

#include "../components/resource_shader.hpp"
#include "../components/resource_texture.hpp"
#include "../components/entity_camera.hpp"
#include "../components/resource_mesh.hpp"

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
    std::shared_ptr<Shader> m_shaderProgram;
    int m_width, m_height;

    void draw(float vertices[], std::size_t vertSize, GLuint indices[], std::size_t indSize,glm::vec4 color);
    float normalizeX(float x){ return (x / static_cast<float>(m_width)) * 2 - 1; };
    float normalizeY(float y){ return -((y / static_cast<float>(m_height)) * 2 - 1); };

public:
    std::shared_ptr<Camera> m_camera;

    // Screen width and height
    void setWindowSize(int width, int height){ m_width = width; m_height = height; };

    // Drawing
    void beginMode3D();
    void endMode3D();

    // Camera
    void setCamera(std::shared_ptr<Camera> camera){ m_camera = camera; };

    // Basic drawing functions
    void clearBackground(Color color);
    void drawPixel(glm::vec2 pos, Color color);
    void drawLine(glm::vec2 startPos, glm::vec2 endPos, Color color);
    void drawTriangle(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3, Color color);
    void drawRectangle(glm::vec2 pos, glm::vec2 size, Color color);
    void drawCircle(glm::vec2 pos, float radius, int segments, Color color);

    // Texture drawing functions
    void drawTexture(std::shared_ptr<Texture> texture, glm::vec2 pos, glm::vec4 color);
    void drawTextureExtra(std::shared_ptr<Texture> texture, glm::vec2 pos, float rotation, float scale, glm::vec4 color);

    // Basic geometric 3D shapes drawing functions
    void drawPoint3D(glm::vec3 position, float pointSize, glm::vec4 color);
    void drawLine3D(glm::vec3 startPos, float lineSize, glm::vec3 endPos, glm::vec4 color);
    void drawGrid(int slices, float spacing, glm::vec4 color);
    void drawPlane(glm::vec3 centerPos, glm::vec2 size, glm::vec4 color); // XZ
    void drawCube(glm::vec3 position, glm::vec3 size, glm::vec4 color);
    void drawCubeWires(glm::vec3 position, glm::vec3 size, glm::vec4 color);
    // TODO
    void drawMesh(std::shared_ptr<Mesh> mesh);
    void drawTexture3D(std::shared_ptr<Texture> texture, glm::vec2 pos, float rotate, float scale, glm::vec4 color);

    // Mesh generation functions
    // TODO
    std::shared_ptr<Mesh> genMeshCube();

    // ChangeShader
    void useShader(std::shared_ptr<Shader> shader){ 
        m_shaderProgram = shader; 
        glUseProgram(m_shaderProgram->id_shader);
    };

    // Text
    void drawText(const char*, glm::vec2, int, glm::vec4);

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
};