#pragma once

#include <TM_RenderObject.hpp>
#include <TM_RuntimeInfo.hpp>

#include <include/effects/SkGradientShader.h>

class TM_GradientView : public TM_RenderObject
{
    public:
        TM_GradientView(SkRect bounds);
        void Render(TM_RenderInfo renderInfo) override;
};