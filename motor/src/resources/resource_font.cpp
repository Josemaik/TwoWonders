#include "resource_font.hpp"

bool Font::load(const char* filePath){
    setupFont(filePath);

    isLoaded() ? std::cout << "Load a font (ID: " << m_id <<") -> " << filePath << std::endl : std::cout << "Error loading a font" << std::endl;

    return isLoaded();
}

void Font::unload(){
    // Release resources
    if (face) {
        FT_Done_Face(face);
        face = nullptr;
    }

    if (library) {
        FT_Done_FreeType(library);
        library = nullptr;
    }
    std::cout << "Unload a font (ID: " << m_id <<")" << std::endl; 
}

// PRIVATE

void Font::setupFont(const char* filePath){
    // Initialize FreeType
    if(FT_Init_FreeType(&library))
        std::cerr << "Error initializing FreeType" << std::endl;

    // Load the font
    if(FT_New_Face(library, filePath, 0, &face)){
        FT_Done_FreeType(library);
        std::cerr << "Error loading the font" << std::endl;
    }

    isLoad = (library != nullptr) && (face != nullptr);
}