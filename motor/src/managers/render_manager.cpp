#include "render_manager.hpp"

#include <iostream>

// Basic drawing functions

void RenderManager::beginMode3D(){
    useShader(shader3D);
    glEnable(GL_DEPTH_TEST);
}

void RenderManager::endMode3D(){
    //glPopMatrix();
    //glPopAttrib();

    glDisable(GL_DEPTH_TEST);
    useShader(shaderColor);
}

// Basic drawing functions

void RenderManager::clearBackground(Color color){
    glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// Text drawing functions

void RenderManager::drawText(const char*, glm::vec2, int, Color){
    /*
    // Set font size
    FT_Set_Pixel_Sizes(m_defaultFont->getFace(), 0, fontSize);

    // Set pen position
    FT_Vector pen;
    pen.x = pos.x * 64;
    pen.y = pos.y * 64;

    // Loop through each character in the text
    for (const char* c = text; *c; ++c) {
        // Load the glyph
        if (!FT_Load_Char(m_defaultFont->getFace(), *c, FT_LOAD_RENDER)) {
            FT_GlyphSlot glyphSlot = m_defaultFont->getFace()->glyph;

            glRasterPos2i(pen.x, pen.y);
            glColor4f(color.r, color.g, color.b, color.a);
            glDrawPixels(glyphSlot->bitmap.width, 
                         glyphSlot->bitmap.rows, 
                         GL_RED, GL_UNSIGNED_BYTE, 
                         glyphSlot->bitmap.buffer);

            // Move the pen to the next position
            pen.x += glyphSlot->advance.x >> 6;
            pen.y += glyphSlot->advance.y >> 6;
        }else
            std::cerr << "Error loading character: " << *c << std::endl;
    }
    */
}