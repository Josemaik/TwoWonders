#pragma once
#include <iostream>

struct Resource{
    virtual ~Resource() {};

    virtual bool load() = 0;
    virtual void unload() = 0;
    virtual bool isLoaded() const = 0;
};

// Struct Mesh
struct Mesh : public Resource{
    bool load() override{
        // std::cout << "Loading Mesh" << std::endl;
        // Mesh loading logic
        return true;
    }

    void unload() override{
        // std::cout << "Unloading Mesh" << std::endl;
        // Mesh unloading logic
    }

    bool isLoaded() const override{
        // Logic to check if Mesh is loaded
        return true;
    }
};

// Struct Texture
struct Texture : public Resource{
    bool load() override{
        // std::cout << "Loading Texture" << std::endl;
        // Texture loading logic
        return true;
    }

    void unload() override{
        // std::cout << "Unloading Texture" << std::endl;
        // Texture unloading logic
    }

    bool isLoaded() const override{
        // Logic to check if Texture is loaded
        return true;
    }
};

// Struct Material
struct Material : public Resource{
    bool load() override{
        // std::cout << "Loading Material" << std::endl;
        // Material loading logic
        return true;
    }

    void unload() override{
        // std::cout << "Unloading Material" << std::endl;
        // Material unloading logic
    }

    bool isLoaded() const override{
        // Logic to check if Material is loaded
        return true;
    }
};

// Struct Shader
struct Shader : public Resource{
    bool load() override{
        // std::cout << "Loading Shader" << std::endl;
        // Shader loading logic
        return true;
    }

    void unload() override{
        // std::cout << "Unloading Shader" << std::endl;
        // Shader unloading logic
    }

    bool isLoaded() const override{
        // Logic to check if Shader is loaded
        return true;
    }
};