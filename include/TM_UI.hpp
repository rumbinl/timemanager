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
        TM_TextView(std::string text, SkScalar width, SkScalar height, SkScalar x=0.0f, SkScalar y=0.0f, TM_ViewSetting viewSetting={colorScheme[1],colorScheme[2],colorScheme[3],1,24});
        void Render(SkCanvas* skia_canvas, SkFont* font);
        void setText(std::string newText);
        SkScalar getWidth();
        SkScalar getHeight();
        std::string getText();
        void setHeight(SkScalar newHeight);
        void setHeightFont(SkFont* font);
        void setWidth(SkScalar newWidth);
        ~TM_TextView();
    protected:
        TM_ViewSetting viewSetting;
        SkRect bounds;
    private:
        std::string text;
};

class TM_Button : public TM_TextView
{
    public:
        TM_Button(std::string text, SkScalar x, SkScalar y, SkScalar width, SkScalar height, TM_ViewSetting viewSettings={colorScheme[1],colorScheme[2],colorScheme[3],1,24});
        bool PollEvent(float mouseX, float mouseY);
        void setX(SkScalar x);
        void setY(SkScalar y);
        SkScalar getX();
    private:
        bool select;
};

class TM_CalendarMonthView
{
    public:
        TM_CalendarMonthView(SkScalar x, SkScalar y, int month, int year, SkScalar width, SkScalar height);
        void Render(SkCanvas* skia_canvas, SkFont* font);
        bool PollEvents(float x, float y);
        ~TM_CalendarMonthView();
    private:
        std::vector<TM_Button> dayViewList; 
        int month,year,firstDay,numDays,numRows,numColumns;
        TM_TextView* dataView;
        SkRect bounds;
};

