#pragma once

#include <SDL3/SDL.h>

#include <iostream>

#include <string>

#define SK_GANESH
#define SK_GL
#include <include/core/SkCanvas.h>
#include <include/gpu/gl/GrGLInterface.h>
#include <include/gpu/GrDirectContext.h>
#include <include/core/SkSurface.h>
#include <include/core/SkColor.h>
#include <include/core/SkColorSpace.h>
#include <include/core/SkRect.h>
#include <include/core/SkRRect.h>
#include <include/core/SkFont.h>
#include <include/core/SkFontMetrics.h>
#include <include/core/SkTypeface.h>
#include <include/core/SkTextBlob.h>

#include <TM_DateTime.hpp>

#include <GL/gl3w.h>

#define BACKGROUND_COLOR_INDEX 0
static SkColor colorScheme[4] = {SkColorSetRGB(0xee,0xf3,0xf9),SkColorSetRGB(0xb3,0xcd,0xe4),SkColorSetRGB(0x53,0x76,0x92),SkColorSetRGB(0x1d,0x3f,0x58)};

static SkScalar scrollSensFactor = 5.0;

typedef std::chrono::year_month_day TM_YMD;

#define ZeroDate (TM_YMD{std::chrono::January/1/0})

#ifndef max
#define max(a,b) ((a>b)?(a):(b))
#endif

#ifndef min
#define min(a,b) ((a<b)?(a):(b))
#endif
