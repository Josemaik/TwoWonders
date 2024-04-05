#pragma once

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

#include "resource.hpp"

struct Font : public Resource {
public:
    Font(std::size_t id){ this->m_id = id; };
    ~Font(){ unload(); };

    bool load(const char* ) override;
    void unload() override;

    FT_Face getFace(){ return face; };

private:
    FT_Library library;
    FT_Face face;

    void setupFont(const char* filePath);
};