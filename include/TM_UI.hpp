#pragma once

#include <TM_Core.hpp>

typedef struct { SkColor backgroundColor, borderColor, textColor; SkScalar borderThickness, fontSize, padding; } TM_ViewSetting;

// Render order:
// 1. Background
// 2. Border
// 3. Text

class TM_RenderObject 
{
    public:
        TM_RenderObject(SkRect bounds);
        void Render(SkCanvas* skia_canvas, SkFont* font);
        bool PollEvents(SkScalar mouseX, SkScalar mouseY, SkScalar scrollX, SkScalar scrollY, bool pressed);
        SkRect getBounds();
        SkRect getSrcBounds();
        SkRect setBounds(SkRect bounds);
        SkRect setSrcBounds(SkRect srcBounds);
        ~TM_RenderObject();
    protected:
        SkRect bounds, srcBounds;
};

class TM_TextView : public TM_RenderObject
{
    public:
        TM_TextView(std::string text, SkRect bounds, TM_ViewSetting viewSetting={colorScheme[1],colorScheme[2],colorScheme[3],1,16,5}, bool centered=true);
        void Render(SkCanvas* skia_canvas, SkFont* font);
        void setText(std::string newText);
        SkScalar getWidth();
        SkScalar getHeight();
        std::string getText();
        SkScalar getTextXOffset();
        void setColorOpacity(uint8_t opacity);
        void setHeight(SkScalar newHeight);
        void setHeightFont(SkFont* font);
        void setWidth(SkScalar newWidth);
        void setTextXOffset(SkScalar scrollX);
        void setX(SkScalar x);
        void setY(SkScalar y);
        ~TM_TextView();
    protected:
        TM_ViewSetting viewSetting;
        bool centered;
        SkSurface* renderSurface;
        SkScalar textXOffset=0.0f;
    private:
        std::string text;
};

class TM_Button : public TM_TextView
{
    public:
        TM_Button(std::string text, SkRect bounds, TM_ViewSetting viewSettings={colorScheme[1],colorScheme[2],colorScheme[3],1,16,0});
        bool PollEvents(SkScalar mouseX, SkScalar mouseY, SkScalar scrollX, SkScalar scrollY, bool pressed);
        void invertColors();
        SkScalar getX();
    private:
        bool select;
};

class TM_CalendarMonthView : TM_RenderObject
{
    public:
        TM_CalendarMonthView(SkRect bounds, int month, int year);
        void Render(SkCanvas* skia_canvas, SkFont* font);
        bool PollEvents(SkScalar mouseX, SkScalar mouseY, SkScalar scrollX, SkScalar scrollY, bool pressed);
        ~TM_CalendarMonthView();
    private:
        std::vector<TM_Button> dayViewList; 
        int month,year,firstDay,numDays,numRows,numColumns,hoverDayButton=-1,selectDayButton=-1;
        TM_TextView* dataView;
        std::vector<TM_TextView> weekList;
        SkSurface* renderSurface;
};

class TM_CalendarWeekView : TM_RenderObject
{
    public:
        TM_CalendarWeekView(SkRect bounds, int numDays = 1, SkScalar hourHeight = 50.0, TM_ViewSetting viewSettings={colorScheme[1],colorScheme[2],colorScheme[3],1,16,1});
        void Render(SkCanvas* skia_canvas, SkFont* font);
        void RenderTimes(SkCanvas* skia_canvas, SkFont* font);
        bool PollEvents(SkScalar mouseX, SkScalar mouseY, SkScalar scrollX, SkScalar scrollY, bool pressed);
        ~TM_CalendarWeekView();
    private:
        int pressWeekIndexStart = -1, pressWeekIndexEnd = -1,pressDayIndexStart = -1, pressDayIndexEnd = -1;
        TM_ViewSetting viewSettings;
        int scrollY=0.0f, pressIndexStart=-1, pressIndexEnd=-1,numDays=1;
        SkScalar hourHeight,yOff,xOff=0.0f;
        bool selected=false;

};

class TM_TextBox : public TM_TextView
{
    public:
        TM_TextBox(SkRect bounds, std::string placeholder, TM_ViewSetting viewSetting ={colorScheme[1],colorScheme[2],colorScheme[3],1,16,5});
        void Render(SkCanvas* skia_canvas, SkFont* font);
        bool PollEvents(SkScalar mouseX, SkScalar mouseY, SkScalar scrollX, SkScalar scrollY, bool pressed);
        ~TM_TextBox();
    private:
        std::string placeholder, content="";
        bool fitted = false;
        int cursorIndex=0;
};

class TM_NumberBox : public TM_TextView
{
    public:
    private:
        int numLimit;
};

class TM_View 
{
    public:
        TM_View(SkScalar bounds, std::vector<TM_RenderObject> objects);
        ~TM_View();
    private:
};

class TM_CalendarView : public TM_View
{
    public:
        TM_CalendarView(SkRect bounds);
    private:
        TM_CalendarMonthView* monthView;
        TM_CalendarWeekView* weekView;
};

