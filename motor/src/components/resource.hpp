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