#pragma once
#include <iostream>

struct Resource{
    std::string name;

    virtual ~Resource() {};
    virtual bool load() = 0;
    virtual void unload() = 0;
    virtual bool isLoaded() const = 0;
};

// Struct Texture // .png
struct Texture : public Resource{
    // int id (glGenTextures)
    // int width, heigh

    bool load() override { return true; }
    void unload() override {}
    bool isLoaded() const override { return true; }
};

// Struct Material // .mtl
struct Material : public Resource{
    // coeficientes light
    // resources Textures

    bool load() override { return true; }
    void unload() override {}
    bool isLoaded() const override { return true; }
};

// Struct Shader
struct Shader : public Resource{
    // int id (glCreateProgram)
    // setInt, setFloat, setMat4

    bool load() override { return true; }
    void unload() override {}
    bool isLoaded() const override { return true; }
};