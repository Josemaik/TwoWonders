#pragma once

namespace DarkMoon {
    struct Color {
        unsigned char r = 0;
        unsigned char g = 0;
        unsigned char b = 0;
        unsigned char a = 255;
    };

    // Custom engine color palette (UI Two Wonders)
#define D_WHITE           Color { 255, 255, 255, 255 }
#define D_BLACK           Color {   0,   0,   0, 255 }
#define D_GRAY            Color { 100, 100, 100, 255 }

#define D_AQUA            Color { 108, 198, 215, 255 }
#define D_AQUA_LIGHT      Color { 154, 222, 235, 255 }
#define D_AQUA_DARK       Color {  40, 141, 160, 255 }

#define D_BLUE            Color { 121, 145, 221, 255 }
#define D_BLUE_LIGHT      Color { 164, 181, 238, 255 }
#define D_BLUE_DARK       Color {  56,  84, 175, 255 }

#define D_YELLOW          Color { 255, 215, 125, 255 }
#define D_YELLOW_LIGHT    Color { 255, 227, 165, 255 }
#define D_YELLOW_DARK     Color { 255, 194,  59, 255 }

#define D_ORANGE          Color { 255, 188, 125, 255 }
#define D_ORANGE_LIGHT    Color { 255, 209, 165, 255 }
#define D_ORANGE_DARK     Color { 255, 155,  59, 255 }

#define D_KAIWA           Color { 113, 230, 138, 255 }
#define D_KAIWA_LIGHT     Color { 157, 242, 176, 255 }
#define D_KAIWA_DARK      Color {  45, 195,  78, 255 }

#define D_CORAL_PINK      Color { 255, 173, 165, 255 }
#define D_CORAL_PINK_LIGHTColor { 255, 216, 212, 255 }
#define D_CORAL_PINK_DARK Color { 255, 137, 125, 255 }

#define D_RED             Color { 255,  78,  59, 255 }

#define D_LAVENDER        Color { 219, 170, 205, 255 }
#define D_LAVENDER_LIGHT  Color { 238, 212, 231, 255 }
#define D_LAVENDER_DARK   Color { 162,  78, 139, 255 }

#define D_ROSE            Color { 247, 192, 201, 255 }
#define D_ROSE_LIGHT      Color { 251, 223, 228, 255 }
#define D_ROSE_DARK       Color { 206,  98, 115, 255 }

#define D_GREEN           Color { 196, 234, 182, 255 }
#define D_GREEN_LIGHT     Color { 225, 245, 218, 255 }
#define D_GREEN_DARK      Color { 115, 186,  89, 255 }

#define D_VIOLET          Color { 234, 248, 192, 255 }
#define D_VIOLET_LIGHT    Color { 244, 252, 223, 255 }
#define D_VIOLET_DARK     Color { 179, 206,  99, 255 }

#define D_LEMON           Color { 255, 252, 198, 255 }
#define D_LEMON_LIGHT     Color { 255, 254, 226, 255 }
#define D_LEMON_DARK      Color { 218, 213, 104, 255 }

#define D_PALE_PINK       Color { 255, 208, 198, 255 }
#define D_PALE_PINK_LIGHT Color { 255, 231, 226, 255 }
#define D_PALE_PINK_DARK  Color { 218, 124, 104, 255 }

#define D_MINT            Color { 168, 216, 186, 255 }
#define D_MINT_LIGHT      Color { 210, 237, 220, 255 }
#define D_MINT_DARK       Color {  75, 158, 106, 255 }

#define D_PINK            Color { 251, 162, 183, 255 }
#define D_PINK_LIGHT      Color { 254, 211, 221, 255 }
#define D_PINK_DARK       Color { 247, 121, 150, 255 }

#define D_MAGENTA         Color { 235,  55,  97, 255 }
}