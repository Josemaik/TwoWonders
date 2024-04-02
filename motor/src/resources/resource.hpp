#pragma once
#include <iostream>

struct Resource{
    std::size_t id;

    virtual ~Resource() {};
    virtual bool load() = 0;
    virtual void unload() = 0;
    virtual bool isLoaded() const = 0;
};