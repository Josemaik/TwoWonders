#pragma once

#include "resource.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct Shader : public Resource {
public:
    GLuint id_shader;

    // reads and build the shader
    Shader(std::size_t, const char*, const char*);

    void use();

    bool load() override { return true; };
    void unload() override;
    bool isLoaded() const override { return true; };
};