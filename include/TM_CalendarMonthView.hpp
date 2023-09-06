#pragma once

#include <TM_UI.hpp>

template<class T> class TM_CalendarMonthView : public TM_RenderObject
{
    public:
        TM_CalendarMonthView(SkRect bounds, void (*action)(T* context, std::chrono::year_month_day date)=NULL, T* context=NULL, TM_ViewSetting viewSetting={colorScheme[1],colorScheme[2],colorScheme[3],1,16,1});
        void Render(SkCanvas* skia_canvas, SkFont* font) override;
        bool PollEvents(TM_EventInput eventInput) override;
        void setMonth(std::chrono::year_month ym);
        std::chrono::year_month getMonthYear();
        ~TM_CalendarMonthView();
    private:
        int firstDay,numDays,numRows,numColumns,hoverDayButton=-1,selectDayButton=-1;
        TM_TextView* dataView;
        T* context;
        void (*action)(T* context, std::chrono::year_month_day date);
        std::vector<TM_TextView> weekList,dayViewList;
        std::chrono::year_month ym_date;
};

template<class T> TM_CalendarMonthView<T>::TM_CalendarMonthView(SkRect bounds, void (*action)(T* context, std::chrono::year_month_day date), T* context, TM_ViewSetting viewSetting) : TM_RenderObject(bounds, viewSetting)
{
    this->dayViewList = std::vector(31, TM_TextView("0", SkRect::MakeWH(bounds.width()/7.0f, 0)));
    this->dataView = new TM_TextView("January 1981", bounds, {colorScheme[3],colorScheme[2],colorScheme[0],0,36});
    this->numColumns = 7.0f;

    this->action = action;
    this->context = context;

    this->setMonth({getCurrentDate().year(),getCurrentDate().month()});

    this->weekList = std::vector(7, TM_TextView("XX", SkRect::MakeWH(this->bounds.width()/7.0f, 0.0f)));
}

template<class T> void TM_CalendarMonthView<T>::setMonth(std::chrono::year_month ym)
{
    this->ym_date = ym;
    this->firstDay = weekDayFromDate({ym.year(),ym.month(),std::chrono::day{1}});
    this->numDays = TM_NumMonthDays(this->ym_date);
    this->numRows = (this->numDays+this->firstDay+6)/7;
    if(this->action != NULL)
        (*this->action)(this->context, {ym.year(),ym.month(),std::chrono::day{1}});
}

template <class T> std::chrono::year_month TM_CalendarMonthView<T>::getMonthYear()
{
    return ym_date;
}

template <class T> void TM_CalendarMonthView<T>::Render(SkCanvas* skia_canvas, SkFont* font)
{
    SkFontMetrics fontMetrics;
    font->getMetrics(&fontMetrics);
    this->dataView->setX(this->bounds.x());
    this->dataView->setY(this->bounds.y());
    this->dataView->setText(monthNames[static_cast<unsigned>(this->ym_date.month())-1]+' '+std::to_string(static_cast<int>(this->ym_date.year())));
    this->dataView->setWidth(this->bounds.width());
    this->dataView->setHeightFont(font);
    this->dataView->Render(skia_canvas, font);
    SkScalar x=this->firstDay*(this->bounds.width()/7.0f)+this->bounds.x(),y=this->dataView->getHeight()+this->bounds.y();
    for(int i=0;i<7;i++)
    {
        this->weekList[i].setText(dayNames[i].substr(0,2).c_str());
        this->weekList[i].setWidth(this->bounds.width()/7.0f);
        this->weekList[i].setX(this->bounds.x()+(float)i*this->bounds.width()/7.0f);
        this->weekList[i].setY(y);
        this->weekList[i].setHeightFont(font);
        this->weekList[i].Render(skia_canvas, font);
    }
    y+=this->weekList[0].getHeight();
    for(int i=0;i<this->numDays;i++)
    {
        if((i+firstDay)%7==0 && i)
        {
            x-=this->bounds.width();
            y+=this->dayViewList[0].getHeight();
        }
        this->dayViewList[i].setHeight((this->bounds.height()-this->dataView->getHeight()-this->weekList[0].getHeight())/numRows);
        this->dayViewList[i].setX(x);
        this->dayViewList[i].setY(y);
        this->dayViewList[i].setText(std::to_string(i+1));
        this->dayViewList[i].setWidth(this->bounds.width()/7.0f);
        if(i == hoverDayButton || i == selectDayButton)
            this->dayViewList[i].invertColors();
        this->dayViewList[i].Render(skia_canvas, font);
        if(i == hoverDayButton || i == selectDayButton)
            this->dayViewList[i].invertColors();
        x+=this->bounds.width()/7.0f;
    }
}

template<class T> bool TM_CalendarMonthView<T>::PollEvents(TM_EventInput eventInput)
{
    bool select=false;
    if(this->bounds.contains(eventInput.mouseX,eventInput.mouseY))
    {
        for(int i = 0; i<this->dayViewList.size(); i++)
        {
            if(this->dayViewList[i].PollEvents(eventInput))
            {
                this->hoverDayButton = i;
                select = true;
                if(eventInput.mousePressed)
                {
                    if(this->selectDayButton==i)
                    {
                        this->selectDayButton = -1;
                        if(action != NULL)
                            (action)(this->context,{this->ym_date.year(),this->ym_date.month(),std::chrono::day{1}});
                    }
                    else
                    {
                        this->selectDayButton = i;
                        if(action != NULL)
                            (action)(this->context,{this->ym_date.year(),this->ym_date.month(),std::chrono::day{(unsigned)i+1}});
                    }
                }
            }
        }
    }
    if(select == false && this->hoverDayButton!=-1)
    {
        this->hoverDayButton = -1;
        return true;
    }
    return select;
}

template<class T> TM_CalendarMonthView<T>::~TM_CalendarMonthView()
{

}