#include "resource_texture.hpp"

#pragma GCC diagnostic ignored "-Wconversion"
#define STB_IMAGE_IMPLEMENTATION
#include "../libs/stb_image.h"
#pragma GCC diagnostic warning "-Wconversion"

// PUBLIC

namespace DarkMoon {
    bool Texture::load(const char* filePath) {
        this->m_filePath = filePath;
        setup();

        isLoaded() ? std::cout << "Load a texture (ID: " << m_id << ") -> " << m_filePath << std::endl
            : std::cout << "Error loading a texture -> " << m_filePath << std::endl;

        return isLoaded();
    }

    void Texture::unload() {
        glDeleteTextures(1, &m_idTexture);
        if (isLoaded())
            std::cout << "Unload a texture (ID: " << m_id << ")" << std::endl;
        m_isLoad = false;
    }

    // PRIVATE

    void Texture::setup() {
        glGenTextures(1, &m_idTexture);
        glBindTexture(GL_TEXTURE_2D, m_idTexture);

        // Set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Load image, create texture and generate mipmaps
        unsigned char* data = stbi_load(m_filePath, &m_width, &m_height, &m_nrChannels, STBI_rgb_alpha);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            m_isLoad = true;
        }
        stbi_image_free(data);
    }
}