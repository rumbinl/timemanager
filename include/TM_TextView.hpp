#pragma once

#include <TM_RenderObject.hpp>
#include <TM_ViewSetting.hpp>
#include <TM_RuntimeInfo.hpp>

class TM_TextView : public TM_RenderObject
{
    public:
        TM_TextView(std::string text="", SkRect bounds=SkRect::MakeEmpty(), TM_ViewSetting viewSetting={colorScheme[1],colorScheme[2],colorScheme[3],0,24,5,5}, bool centered=true);
        void Render(TM_RenderInfo renderInfo);
        static void Render(std::string text, SkRect bounds, TM_RenderInfo renderInfo, TM_ViewSetting viewSetting={colorScheme[1],colorScheme[2],colorScheme[3],1,16,5}, bool centered=true);
        void setText(std::string newText);
        SkScalar getWidth();
        SkScalar getHeight();
        std::string getText();
        SkScalar getTextXOffset();
        void invertColors();
        void setColorOpacity(uint8_t opacity);
        void setHeight(SkScalar newHeight);
        void setHeightFont(SkFont* font);
        void setWidth(SkScalar newWidth);
        void setTextXOffset(SkScalar scrollX);
        void setX(SkScalar x);
        void setY(SkScalar y);
    protected:
        bool centered;
        SkSurface* renderSurface;
        SkScalar textXOffset=0.0f;
    private:
        std::string text;
};