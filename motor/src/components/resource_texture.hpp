#pragma once
#include "resource.hpp"

#include <GL/glew.h>

struct Texture : public Resource{
public:
    GLuint texture;

    Texture(std::size_t);

    bool load() override { return true; };
    bool load(const char* filePath);
    void unload() override;
    bool isLoaded() const override;

    // void draw();

private:
    GLuint id_texture;
    int width, height, nrChannels;
    bool isLoad { false };

    void setupTexture(const char* filePath);
};