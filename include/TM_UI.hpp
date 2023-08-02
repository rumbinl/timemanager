#pragma once

#include <TM_Core.hpp>

typedef struct { SkColor backgroundColor, borderColor, textColor; SkScalar borderThickness, fontSize; } TM_ViewSetting;

// Render order:
// 1. Background
// 2. Border
// 3. Text

class TM_TextView 
{
    public:
        TM_TextView(std::string text, SkScalar width, SkScalar height, TM_ViewSetting viewSetting);
        void Render(SkCanvas* skia_canvas, SkFont* font);
        void setText(std::string newText);
        SkScalar getWidth();
        SkScalar getHeight();
        std::string getText();
        void setHeight(SkScalar newHeight);
        void setHeightFont(SkFont* font);
        void setWidth(SkScalar newWidth);
        ~TM_TextView();
    private:
        TM_ViewSetting viewSetting;
        std::string text;
        SkScalar width, height;
};

class TM_CalendarMonthView
{
    public:
        TM_CalendarMonthView(int month, int year, SkScalar width, SkScalar height);
        void Render(SkCanvas* skia_canvas, SkFont* font);
        ~TM_CalendarMonthView();
    private:
        std::vector<TM_TextView> dayViewList; 
        int month,year,firstDay,numDays;
        TM_TextView* dataView;
        SkScalar width, height;
};