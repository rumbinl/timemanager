#pragma once

#include <TM_Core.hpp>

typedef struct { SkScalar mouseX, mouseY, scrollX, scrollY, dpi; bool mousePressed, mouseHeld, keyPressed; std::string inputText; SkFont* font; SDL_Scancode key; } TM_EventInput;

typedef struct { SkCanvas* canvas; SkFont* textFont, *iconFont; } TM_RenderInfo;