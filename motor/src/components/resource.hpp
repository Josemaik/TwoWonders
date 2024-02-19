#pragma once
#include <iostream>
#include "entity.hpp"

struct Resource : public Entity{
    virtual ~Resource() {};

    virtual bool load() = 0;
    virtual void unload() = 0;
    virtual bool isLoaded() const = 0;
};

// Struct Mesh
struct Mesh : public Resource{
    bool load() override { return true; }
    void unload() override {}
    bool isLoaded() const override { return true; }

    void draw(glm::mat4) const override { std::cout << "Draw a mesh" << std::endl; };
};

// Struct Texture
struct Texture : public Resource{
    bool load() override { return true; }
    void unload() override {}
    bool isLoaded() const override { return true; }
};

// Struct Material
struct Material : public Resource{
    bool load() override { return true; }
    void unload() override {}
    bool isLoaded() const override { return true; }
};

// Struct Shader
struct Shader : public Resource{
    bool load() override { return true; }
    void unload() override {}
    bool isLoaded() const override { return true; }
};