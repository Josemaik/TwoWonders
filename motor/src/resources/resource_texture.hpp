#pragma once
#include "resource.hpp"

#include <GL/glew.h>

struct Texture : public Resource{
public:
    Texture(std::size_t);
    ~Texture(){ unload(); };

    bool load(const char* filePath) override;
    void unload() override;
    bool isLoaded() const{ return m_isLoad; }

    // Getters

    GLuint getIDTexture(){ return m_idTexture; };
    int getWidth(){ return m_width; };
    int getHeight(){ return m_height; };

private:
    GLuint m_idTexture;
    int m_width, m_height, m_nrChannels;
    bool m_isLoad { false };

    void setup(const char* filePath);
};