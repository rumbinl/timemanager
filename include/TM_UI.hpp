#pragma once

#include <TM_Core.hpp>

typedef struct { SkColor backgroundColor, borderColor, textColor; SkScalar borderThickness, fontSize, padding; } TM_ViewSetting;

// Render order:
// 1. Background
// 2. Border
// 3. Text

class TM_TextView 
{
    public:
        TM_TextView(std::string text, SkScalar width, SkScalar height, SkScalar x=0.0f, SkScalar y=0.0f, TM_ViewSetting viewSetting={colorScheme[1],colorScheme[2],colorScheme[3],1,16,5}, bool centered=true);
        void Render(SkCanvas* skia_canvas, SkFont* font);
        void setText(std::string newText);
        SkScalar getWidth();
        SkScalar getHeight();
        std::string getText();
        void setHeight(SkScalar newHeight);
        void setHeightFont(SkFont* font);
        void setWidth(SkScalar newWidth);
        void setTextXOffset(SkScalar newTextXOffset);
        ~TM_TextView();
    protected:
        TM_ViewSetting viewSetting;
        SkRect bounds;
        bool centered;
        SkSurface* renderSurface;
        SkScalar textXOffset=0.0f;
    private:
        std::string text;
};

class TM_Button : public TM_TextView
{
    public:
        TM_Button(std::string text, SkScalar x, SkScalar y, SkScalar width, SkScalar height, TM_ViewSetting viewSettings={colorScheme[1],colorScheme[2],colorScheme[3],1,16,0});
        bool PollEvent(float mouseX, float mouseY, bool isPressed);
        void setX(SkScalar x);
        void setY(SkScalar y);
        void invertColors();
        SkScalar getX();
    private:
        bool select;
};

class TM_CalendarMonthView
{
    public:
        TM_CalendarMonthView(SkScalar x, SkScalar y, int month, int year, SkScalar width, SkScalar height);
        void Render(SkCanvas* skia_canvas, SkFont* font);
        bool PollEvents(float x, float y, bool isPressed);
        ~TM_CalendarMonthView();
    private:
        std::vector<TM_Button> dayViewList; 
        int month,year,firstDay,numDays,numRows,numColumns,hoverDayButton=-1,selectDayButton=-1;
        TM_TextView* dataView;
        SkSurface* renderSurface;
        SkRect bounds;
};

class TM_CalendarDayView
{
    public:
	TM_CalendarDayView(SkScalar x, SkScalar y, SkScalar width, SkScalar height, SkScalar hourHeight = 80.0, TM_ViewSetting viewSettings={colorScheme[1],colorScheme[2],colorScheme[3],1,16,1});
	void Render(SkCanvas* skia_canvas, SkFont* font);
	bool PollEvents(float x, float y, float scrollY, bool pressed);
	~TM_CalendarDayView();
    private:
	TM_ViewSetting viewSettings;
	SkRect bounds;
	SkScalar hourHeight;
	int scrollY=0.0f, pressIndexStart=-1, pressIndexEnd=-1;
	bool selected=false;
};

class TM_CalendarWeekView
{
    public:
    private:
};

class TM_TextBox : public TM_TextView
{

    public:
        TM_TextBox(SkScalar x, SkScalar y, SkScalar width, SkScalar height, std::string placeholder, TM_ViewSetting viewSetting ={colorScheme[1],colorScheme[2],colorScheme[3],1,16,5});
        void Render(SkCanvas* skia_canvas, SkFont* font);
        void PollEvents(float x, float y, bool isPressed);
        ~TM_TextBox();
    private:
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
    private:
};

class TM_CalendarView : public TM_View
{
    public:
    private:
};

