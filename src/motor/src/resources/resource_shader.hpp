#pragma once

#include "resource.hpp"

#include <string>
#include <cstring>

#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace DarkMoon {
    struct Shader : public Resource {
    public:
        Shader(std::size_t, const char*, const char*);
        ~Shader() { unload(); };

        void use();
        bool load(const char* filePath) override;
        void unload() override;

        // Getters

        GLuint getIDShader() { return m_idShader; };

    private:
        GLuint m_idShader;
        const char* m_vertexPath;
        const char* m_fragmentPath;

        void setup() override;
    };
}