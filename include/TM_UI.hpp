#pragma once

#include <TM_Core.hpp>
#include <TM_Task.hpp>

typedef struct { SkColor backgroundColor, borderColor, textColor; SkScalar borderThickness, fontSize, padding; } TM_ViewSetting;

typedef struct { SkScalar mouseX, mouseY, scrollX, scrollY; bool mousePressed, mouseHeld; } TM_EventInput;

// Render order:
// 1. Background
// 2. Border
// 3. Text

class TM_RenderObject 
{
    public:
        TM_RenderObject(SkRect bounds, TM_ViewSetting viewSetting={colorScheme[1],colorScheme[2],colorScheme[3],1,24,5});
        virtual void Render(SkCanvas* skia_canvas, SkFont* font);
        virtual bool PollEvents(TM_EventInput eventInput);
        SkRect getBounds();
        SkRect getSrcBounds();
        void setBounds(SkRect newBounds);
        void setSrcBounds(SkRect srcBounds);
        bool exists();
        void setExistence(bool existence);
        ~TM_RenderObject();
    protected:
        SkRect bounds, srcBounds;
        TM_ViewSetting viewSetting;
        bool existence=true;
};

class TM_TextView : public TM_RenderObject
{
    public:
        TM_TextView(std::string text, SkRect bounds, TM_ViewSetting viewSetting={colorScheme[1],colorScheme[2],colorScheme[3],1,24,5}, bool centered=true);
        void Render(SkCanvas* skia_canvas, SkFont* font);
        static void Render(std::string text, SkRect bounds, SkCanvas* skia_canvas, SkFont* font, TM_ViewSetting viewSetting={colorScheme[1],colorScheme[2],colorScheme[3],1,16,5}, bool centered=true);
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
        ~TM_TextView();
    protected:
        bool centered;
        SkSurface* renderSurface;
        SkScalar textXOffset=0.0f;
    private:
        std::string text;
};

class TM_CalendarWeekView : public TM_RenderObject
{
    public:
        TM_CalendarWeekView(SkRect bounds, std::chrono::year_month_day* focusDate, std::set<TM_Task>* tasks, int numDays = 7, SkScalar hourHeight = 50.0, TM_ViewSetting viewSettings={colorScheme[1],colorScheme[2],colorScheme[3],1,24,1});
        void Render(SkCanvas* skia_canvas, SkFont* font) override;
        void RenderTimes(SkCanvas* skia_canvas, SkFont* font);
        bool PollEvents(TM_EventInput eventInput) override;
        void setDaySpan(int daySpan);
        int getDaySpan();
        ~TM_CalendarWeekView();
    private:
        std::set<TM_Task>* tasks;
        std::vector<TM_TextView> dayLabels;
        int pressWeekIndexStart = -1, pressWeekIndexEnd = -1,pressDayIndexStart = -1, pressDayIndexEnd = -1;
        int scrollY=0.0f, pressIndexStart=-1, pressIndexEnd=-1,numDays=1;
        SkScalar hourHeight,yOff,xOff=0.0f;
        bool selected=false;
        std::chrono::year_month_day* focusDate;
};

class TM_TextBox : public TM_TextView
{
    public:
        TM_TextBox(SkRect bounds, std::string placeholder, TM_ViewSetting viewSetting ={colorScheme[1],colorScheme[2],colorScheme[3],1,24,5});
        void Render(SkCanvas* skia_canvas, SkFont* font) override;
        bool PollEvents(TM_EventInput eventInput) override;
        ~TM_TextBox();
    private:
        std::string placeholder, content="";
        bool fitted = false;
        int cursorIndex=0;
};

template<class T> class TM_Button : public TM_TextView 
{
    public:
        TM_Button(std::string text, SkRect bounds, void (*actionFunc)(T* context)=NULL, T* context=NULL) : TM_TextView(text, bounds)
        {
            this->actionFunc = actionFunc;
            this->context = context;
        }

        bool PollEvents(TM_EventInput eventInput) 
        {
            if(this->bounds.contains(eventInput.mouseX, eventInput.mouseY) && eventInput.mousePressed)
            {
                if(actionFunc != NULL)
                    (*actionFunc)(this->context);
                return true;
            }
            return false;
        }

        ~TM_Button()
        {

        }
    private:
        T* context;
        void (*actionFunc)(T* context);
};

class TM_NumberBox : public TM_TextView
{
    public:
    private:
        int numLimit;
};

class TM_View : public TM_RenderObject
{
    public:
        TM_View(SkRect bounds, std::vector<TM_RenderObject*> objects, TM_ViewSetting viewSetting={colorScheme[1],colorScheme[2],colorScheme[3],1,24,10});
        void Render(SkCanvas* skia_canvas, SkFont* font) override;
        void setRenderObjectExistence(int index, bool existence);
        bool PollEvents(TM_EventInput eventInput) override;
        ~TM_View();
    protected:
        std::vector<TM_RenderObject*> renderObjects;
        SkScalar yOffset=0.0f;
};



