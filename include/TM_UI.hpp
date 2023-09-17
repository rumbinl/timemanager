#pragma once

#include <TM_Core.hpp>
#include <TM_Task.hpp>

typedef struct { SkColor backgroundColor, borderColor, textColor; SkScalar borderThickness, fontSize, paddingX, paddingY; } TM_ViewSetting;

typedef struct { SkScalar mouseX, mouseY, scrollX, scrollY, dpi; bool mousePressed, mouseHeld, keyPressed; std::string inputText; SkFont* font; SDL_Scancode key; } TM_EventInput;

typedef struct { SkCanvas* canvas; SkFont* textFont, *iconFont; } TM_RenderInfo;

// Render order:
// 1. Background
// 2. Border
// 3. Text

class TM_RenderObject 
{
    public:
        TM_RenderObject(SkRect bounds, TM_ViewSetting viewSetting={colorScheme[0],colorScheme[3],colorScheme[3],1,24,5,5});
        virtual void Render(TM_RenderInfo renderInfo);
        virtual bool PollEvents(TM_EventInput eventInput);
        SkRect getBounds();
        SkRect getSrcBounds();
        void setBounds(SkRect newBounds);
        void setSrcBounds(SkRect srcBounds);
        bool exists();
        void setExistence(bool existence);
    protected:
        SkRect bounds, srcBounds;
        TM_ViewSetting viewSetting;
        bool existence=true,select=false;
};

class TM_TextView : public TM_RenderObject
{
    public:
        TM_TextView(std::string text, SkRect bounds, TM_ViewSetting viewSetting={colorScheme[1],colorScheme[2],colorScheme[3],1,24,5,5}, bool centered=true);
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

class TM_CalendarWeekView : public TM_RenderObject
{
    public:
        TM_CalendarWeekView(SkRect bounds, std::chrono::year_month_day* focusDate, std::set<TM_Task>* tasks, int numDays = 7, SkScalar hourHeight = 50.0, TM_ViewSetting viewSettings={colorScheme[1],colorScheme[3],colorScheme[3],1,24,0,0});
        void Render(TM_RenderInfo renderInfo) override;
        void RenderTimes(TM_RenderInfo renderInfo);
        bool PollEvents(TM_EventInput eventInput) override;
        void setDaySpan(int daySpan);
        int getDaySpan();
    private:
        std::set<TM_Task>* tasks;
        std::vector<TM_TextView> dayLabels;
        int pressWeekIndexStart = -1, pressWeekIndexEnd = -1,pressDayIndexStart = -1, pressDayIndexEnd = -1;
        int scrollY=0.0f, pressIndexStart=-1, pressIndexEnd=-1,numDays=1;
        SkScalar hourHeight,yOff,xOff=0.0f;
        std::chrono::year_month_day* focusDate;
};

class TM_TextBox : public TM_TextView
{
    public:
        TM_TextBox(SkRect bounds, std::string placeholder, TM_ViewSetting viewSetting ={colorScheme[1],colorScheme[2],colorScheme[3],1,24,5,5});
        void Render(TM_RenderInfo renderInfo) override;
        bool PollEvents(TM_EventInput eventInput) override;
		SkScalar getCharWidth(char a, SkFont* font);
    private:
		void locatePosition(SkScalar mouseX, std::string text, SkFont* font);
        std::string placeholder, content="";
        bool fitted = false;
		SkScalar cursorX=0.0f;
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

        bool PollEvents(TM_EventInput eventInput) override
        {
            if(this->bounds.contains(eventInput.mouseX, eventInput.mouseY))
            {
				if(eventInput.mousePressed)
					if(actionFunc != NULL)
						(*actionFunc)(this->context);
				this->select=true;
				return true;
            }
			if(this->select)
			{
				this->select = false;
				return true;
			}
			
            return false;
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
        TM_View(SkRect bounds, std::vector<TM_RenderObject*> objects, TM_ViewSetting viewSetting={colorScheme[0],colorScheme[2],colorScheme[3],0,24,10,10});
        TM_View(SkRect bounds, std::vector<SkScalar> proportionTable, std::vector<TM_RenderObject*> objects, TM_ViewSetting viewSetting={colorScheme[0],colorScheme[2],colorScheme[3],0,24,10,10});
        void Render(TM_RenderInfo renderInfo) override;
        void setRenderObjectExistence(int index, bool existence);
        bool PollEvents(TM_EventInput eventInput) override;
		void addRenderObject(TM_RenderObject* renderObject);
		int getNumExists();
    protected:
        std::vector<TM_RenderObject*> renderObjects;
		std::vector<SkScalar> proportionTable;
		bool fit =false;
		int numExists=0;
        SkScalar yOffset=0.0f;
};

class TM_HorizontalView : public TM_View 
{
	public:
        TM_HorizontalView(SkRect bounds, std::vector<TM_RenderObject*> objects, bool fit, TM_ViewSetting viewSetting={colorScheme[0],colorScheme[2],colorScheme[3],1,24,10,0});
        void Render(TM_RenderInfo renderInfo) override;
	private:
		bool fit;
};

