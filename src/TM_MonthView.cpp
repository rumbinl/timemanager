#include <TM_MonthView.hpp>

template <typename ContextType> TM_MonthView<ContextType>::TM_MonthView(SkRect bounds, ContextType* contextPtr, void (*setDateFunc)(ContextType* contextPtr, TM_YMD date), TM_YMD (*getDateFunc)(ContextType* contextPtr), TM_ViewSetting viewSetting) : TM_RenderObject(bounds, viewSetting)
{
    this->idx = indexCal++;
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
                std::chrono::year_month month = monthView->getMonthYear();
                TM_YMD date = monthView->getDate();
                return month.year()==date.year() && month.month() == date.month() && dayIndex == static_cast<unsigned int>(monthView->getDate().day());
            }, 
        {colorScheme[0],colorScheme[2],colorScheme[3],0,24,0,0})
    );

    for(int i=0; i<31; i++)
    {
        dayViewList[i].setText(std::to_string(i+1));
        dayViewList[i].setData(i+1);
    }
    this->currentDate = getCurrentDate();
    this->month = std::chrono::year_month{getCurrentDate().year(), getCurrentDate().month()};
}

template <typename ContextType> void TM_MonthView<ContextType>::Render(TM_RenderInfo renderInfo)
{
    this->updateDate();
    renderInfo.textFont->setSize(this->viewSetting.fontSize);

    int firstDay = weekDayFromDate({this->month.year(),this->month.month(),std::chrono::day{1}}),
        numDays  = TM_NumMonthDays({this->month.year(),this->month.month(),std::chrono::day{1}}),
        numRows  = (numDays+firstDay+6)/7;

    renderInfo.canvas->save();
    renderInfo.canvas->clipRect(this->bounds);
    renderInfo.canvas->translate(this->bounds.x(), this->bounds.y());
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
    this->updateDate();
    bool select=false;
    if(this->bounds.contains(eventInput.mouseX,eventInput.mouseY))
    {
        eventInput.mouseX -= this->bounds.x();
        eventInput.mouseY -= this->bounds.y();
        for(auto& dayButton : dayViewList)
            select += dayButton.PollEvents(eventInput);
    }
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

template <typename ContextType> void TM_MonthView<ContextType>::updateDate()
{
    if(this->getDateFunc!=NULL&& this->contextPtr!=NULL)
    {
        TM_YMD fetchDate = this->getDateFunc(this->contextPtr);
        if(fetchDate != currentDate)
        {
            this->month = std::chrono::year_month{fetchDate.year(), fetchDate.month()};
        }
        currentDate = fetchDate;
    }
}

template <typename ContextType> TM_YMD TM_MonthView<ContextType>::getDate()
{
    return this->currentDate;
}

template <typename ContextType> std::chrono::year_month TM_MonthView<ContextType>::getMonthYear()
{
    return month;
}

template <typename ContextType> void TM_MonthView<ContextType>::setMonthYear(std::chrono::year_month monthYear)
{
    this->month = monthYear;
}

#include <TM_CalendarView.hpp>
#include <TM_CalendarMonthView.hpp>

template class TM_MonthView<TM_TaskManager>;
template class TM_MonthView<TM_CalendarView>;
