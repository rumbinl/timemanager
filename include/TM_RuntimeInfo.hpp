#pragma once

#include <TM_Core.hpp>

typedef struct { SkScalar mouseX, mouseY, scrollX, scrollY, dpi; bool mousePressed, mouseHeld, keyPressed, fileDropped; std::string inputText, fileName; SkFont* font; SDL_Scancode key; } TM_EventInput;

typedef struct { SkCanvas* canvas; SkFont* textFont, *iconFont; SkScalar dpi; } TM_RenderInfo;