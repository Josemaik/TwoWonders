#pragma once

#include "../components/resource_shader.hpp"
#include "../components/resource_texture.hpp"
#include "../components/entity_camera.hpp"
#include "../components/resource_mesh.hpp"

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

    void draw(float vertices[], std::size_t vertSize, GLuint indices[], std::size_t indSize,glm::vec4 color);
    // TODO: 800 / 600 --> getScreenWidth() / getScreenHeight()
    float normalizeX(float x){ return (x / 800.0f) * 2 - 1; };
    float normalizeY(float y){ return -((y / 600.0f) * 2 - 1); };

public:
    std::shared_ptr<Camera> m_camera;
    
    // Drawing
    void beginMode3D();
    void endMode3D();

    // Camera
    void setCamera(std::shared_ptr<Camera> camera){ m_camera = camera; };

    // Basic drawing functions
    void clearBackground(glm::vec4 color);
    void drawPixel(glm::vec2 pos, glm::vec4 color);
    void drawLine(glm::vec2 startPos, glm::vec2 endPos, glm::vec4 color);
    void drawTriangle(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3, glm::vec4 color);
    void drawRectangle(glm::vec2 pos, glm::vec2 size, glm::vec4 color);
    void drawCircle(glm::vec2 pos, float radius, int segments, glm::vec4 color);

    // Texture drawing functions
    void drawTexture(std::shared_ptr<Texture> texture, glm::vec2 pos, glm::vec4 color);
    void drawTextureExtra(std::shared_ptr<Texture> texture, glm::vec2 pos, float rotation, float scale, glm::vec4 color);

    // Basic geometric 3D shapes drawing functions
    void drawPoint3D(glm::vec3 position, float pointSize, glm::vec4 color);
    void drawLine3D(glm::vec3 startPos, glm::vec3 endPos, glm::vec4 color);
    void drawGrid(int slices, float spacing, glm::vec4 color);
    void drawPlane(glm::vec3 centerPos, glm::vec2 size, glm::vec4 color); // XZ
    // TODO
    void drawCube();
    void drawCubeWires();
    void drawSphere();
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
};