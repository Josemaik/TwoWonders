#pragma once
#include <raylib.h>

// Sujeto a cambios cuando dejemos de usar Raylib
struct InputComponent
{
    int up{ KEY_W };
    int down{ KEY_S };
    int left{ KEY_A };
    int right{ KEY_D };
    int space{ KEY_SPACE };
    int enter{ KEY_ENTER };
    // int arrive { KEY_R };
    // int seek   { KEY_K };
    // int Flee   { KEY_F };
    // int Pursue { KEY_P };
    // int Avoid  { KEY_V };
    // int pathfollow  { KEY_T };
    int attarea{ KEY_F };

    int last_key{};
};
