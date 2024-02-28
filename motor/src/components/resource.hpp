#pragma once
#include <iostream>

struct Resource{
    std::size_t id;

    virtual ~Resource() {};
    virtual bool load() = 0;
    virtual void unload() = 0;
    virtual bool isLoaded() const = 0;
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