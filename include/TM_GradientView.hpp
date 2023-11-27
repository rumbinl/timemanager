#pragma once

#include <TM_RenderObject.hpp>
#include <TM_RuntimeInfo.hpp>

#include <include/effects/SkGradientShader.h>
#include <include/core/SkSurface.h>
#include <include/core/SkImage.h>

class TM_GradientView : public TM_RenderObject
{
    public:
        TM_GradientView(SkRect bounds, void* contextPtr=NULL, uint32_t (*getColor)(void* contextPtr)=NULL, void (*setColor)(void* contextPtr, uint32_t color)=NULL);
        void Render(TM_RenderInfo renderInfo) override;
        bool PollEvents(TM_EventInput eventInput) override;
        SkColor getColorFromX(SkScalar x);
        SkScalar getXFromColor(SkColor color);
    private:
        sk_sp<SkSurface> surface;
        sk_sp<SkImage> image;
        SkColor colors[7] = { 0xFF0000ff, 0xFF00ffff, 0xFF00ff00, 0xFFffff00, 0xFFff0000, 0xFFff00ff, 0xFF0000ff};
        SkScalar x=0.0f,y=0.0f;
        void* contextPtr;
        uint32_t (*getColor)(void* contextPtr);
        void (*setColor)(void* contextPtr, uint32_t color);
};