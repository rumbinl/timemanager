#include <TM_MonthView.hpp>

template <typename ContextType> TM_MonthView<ContextType>::TM_MonthView(SkRect bounds, ContextType* contextPtr, void (*setDateFunc)(ContextType* taskManPtr, TM_YMD date), TM_YMD (*getDateFunc)(ContextType* contextPtr), TM_ViewSetting viewSetting) : TM_RenderObject(bounds, viewSetting)
{
    this->contextPtr = contextPtr;
    this->setDateFunc = setDateFunc;
    this->getDateFunc = getDateFunc;

    this->dayViewList = std::vector(31, 
        TM_SelectButton<TM_MonthView<ContextType>,int>("0", 
            SkRect::MakeWH(bounds.width()/7.0f, 0), 0, this, 
            [](TM_MonthView<ContextType>* monthView, int dayIndex)
            { 
                monthView->setDate({monthView->getMonthYear().year(), monthView->getMonthYear().month(), std::chrono::day{(unsigned)dayIndex}}); 
            }, 
            [](TM_MonthView<ContextType>* monthView, int dayIndex) -> bool 
            {
                return monthView->getMonthYear().year()  == monthView->getDate().year() && 
                       monthView->getMonthYear().month() == monthView->getDate().month() && 
                       dayIndex == static_cast<unsigned int>(monthView->getDate().day());
            }, 
        {colorScheme[0],colorScheme[2],colorScheme[3],0,24,0,0})
    );

    for(int i=0; i<31; i++)
    {
        dayViewList[i].setText(std::to_string(i));
        dayViewList[i].setData(i+1);
    }
    this->datePlaceholder = getCurrentDate();
    this->month = std::chrono::year_month{getCurrentDate().year(), getCurrentDate().month()};
}

template <typename ContextType> void TM_MonthView<ContextType>::Render(TM_RenderInfo renderInfo)
{
    renderInfo.textFont->setSize(this->viewSetting.fontSize);

    int firstDay = weekDayFromDate({this->month.year(),this->month.month(),std::chrono::day{1}}),
        numDays  = TM_NumMonthDays({this->month.year(),this->month.month(),std::chrono::day{1}}),
        numRows  = (numDays+firstDay+6)/7;

    renderInfo.canvas->save();
    renderInfo.canvas->clipRect(this->bounds);
    SkScalar xStep = this->bounds.width()/7.0f,
             yStep = this->bounds.height()/(SkScalar)numRows,
             x=firstDay*xStep,
             y=0;

    for(unsigned i=0; i<numDays; i++)
    {
        if((i+firstDay)%7==0 && i)
        {
            x-=this->bounds.width();
            y+=yStep;
        }
        this->dayViewList[i].setHeight(yStep);
        this->dayViewList[i].setX(x);
        this->dayViewList[i].setY(y);
        this->dayViewList[i].setWidth(xStep);
        this->dayViewList[i].Render(renderInfo);

        x+=xStep;
    }
    renderInfo.canvas->restore();

}

template<typename ContextType> bool TM_MonthView<ContextType>::PollEvents(TM_EventInput eventInput) 
{
    bool select=false;
    if(this->bounds.contains(eventInput.mouseX,eventInput.mouseY))
        for(auto& dayButton : dayViewList)
            select += dayButton.PollEvents(eventInput);
    return select;
}

template <typename ContextType> void TM_MonthView<ContextType>::setDate(TM_YMD date)
{
    if(date != ZeroDate)
    {
        setDateFunc(this->contextPtr, date);
        this->month = std::chrono::year_month{date.year(), date.month()};
    }
}

template <typename ContextType> TM_YMD TM_MonthView<ContextType>::getDate()
{
    if(getDateFunc!=NULL)
    {
        return getDateFunc(this->contextPtr);
    }
}

template <typename ContextType> std::chrono::year_month TM_MonthView<ContextType>::getMonthYear()
{
    return month;
}

#include <TM_CalendarView.hpp>
#include <TM_CalendarMonthView.hpp>

template class TM_MonthView<TM_TaskManager>;
template class TM_MonthView<TM_CalendarView>;
