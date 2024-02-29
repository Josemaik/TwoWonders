#include "resource_texture.hpp"

#pragma GCC diagnostic ignored "-Wconversion"
#define STB_IMAGE_IMPLEMENTATION
#include "../libs/stb_image.h"
#pragma GCC diagnostic warning "-Wconversion"

Texture::Texture(std::size_t id){
    this->id = id;
}

bool Texture::load(const char* filePath){
    setupTexture(filePath);

    isLoaded() ? std::cout << "Load a texture (ID: " << id <<") -> " << filePath << std::endl : std::cout << "Error loading a texture" << std::endl;

    return isLoaded();
}

void Texture::unload(){
    glDeleteTextures(1, &id_texture);
    isLoad = false;
    std::cout << "Unload a texture (ID: " << id <<")" << std::endl; 
}

bool Texture::isLoaded() const{ return isLoad; }

// PRIVATE

void Texture::setupTexture(const char* filePath){
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load image, create texture and generate mipmaps
    unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        isLoad = true;
    }
    stbi_image_free(data);
}