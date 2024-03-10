#pragma once

#include "resource.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

enum struct ShaderType { COLOR, TEXTURE, TEXTURE3D };

struct Shader : public Resource {
public:
    GLuint id_shader;
    ShaderType type;

    // reads and build the shader
    Shader(std::size_t, const char*, const char*, ShaderType);

    void use();

    bool load() override { return true; };
    void unload() override;
    bool isLoaded() const override { return true; };
};