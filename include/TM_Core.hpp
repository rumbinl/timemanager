#pragma once

#include <GL/gl3w.h>

#include <SDL3/SDL.h>

#include <iostream>

#include <string>

#define SK_GANESH
#define SK_GL
#include <skia/include/core/SkCanvas.h>
#include <skia/include/gpu/gl/GrGLInterface.h>
#include <skia/include/gpu/GrDirectContext.h>
#include <skia/include/core/SkSurface.h>
#include <skia/include/core/SkColor.h>
#include <skia/include/core/SkColorSpace.h>
#include <skia/include/core/SkRect.h>
#include <skia/include/core/SkFont.h>
#include <skia/include/core/SkFontMetrics.h>
#include <skia/include/core/SkTypeface.h>
#include <skia/include/core/SkTextBlob.h>

#define BACKGROUND_COLOR_INDEX 0
static SkColor colorScheme[4] = {SkColorSetRGB(0xee,0xf3,0xf9),SkColorSetRGB(0xb3,0xcd,0xe4),SkColorSetRGB(0x53,0x76,0x92),SkColorSetRGB(0x1d,0x3f,0x58)};
static std::string monthNames[12] = {"January","February","March","April","May","June","July","August","September","November","October","December"};
