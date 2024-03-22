#pragma once

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

#include "resource.hpp"

struct Font : public Resource {
public:
    Font(std::size_t id){ this->id = id; }

    bool load() override { return true; };
    bool load(const char* filePath);
    void unload() override;
    bool isLoaded() const override { return isLoad; };

    FT_Face getFace(){ return face; };

private:
    FT_Library library;
    FT_Face face;
    bool isLoad { false };

    void setupFont(const char* filePath);
};