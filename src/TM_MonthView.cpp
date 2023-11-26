#include <TM_MonthView.hpp>

TM_MonthView::TM_MonthView(SkRect bounds, void* contextPtr, void (*setDateFunc)(void* contextPtr, TM_YMD date), TM_YMD (*getDateFunc)(void* contextPtr), TM_ViewSetting viewSetting) : TM_RenderObject(bounds, viewSetting)
{
    this->idx = indexCal++;
    this->contextPtr = contextPtr;
    this->setDateFunc = setDateFunc;
    this->getDateFunc = getDateFunc;
    
    this->dayViewList = std::vector(31, 
        TM_SelectButton<int>("0", 
            SkRect::MakeWH(bounds.width()/7.0f, 0), 0, this, 
            [](void* monthViewPtr, int dayIndex,TM_Button<int>* self)
            { 
                TM_MonthView* monthView = (TM_MonthView*)monthViewPtr;
                monthView->setDate({monthView->getMonthYear().year(), monthView->getMonthYear().month(), std::chrono::day{(unsigned)dayIndex}}); 
            }, 
            [](void* monthViewPtr, int dayIndex) -> bool 
            {
                TM_MonthView* monthView = (TM_MonthView*)monthViewPtr;
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

void TM_MonthView::Render(TM_RenderInfo renderInfo)
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

bool TM_MonthView::PollEvents(TM_EventInput eventInput) 
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

void TM_MonthView::setDate(TM_YMD date)
{
    if(date != ZeroDate)
    {
        setDateFunc(this->contextPtr, date);
        this->month = std::chrono::year_month{date.year(), date.month()};
    }
}

void TM_MonthView::updateDate()
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

TM_YMD TM_MonthView::getDate()
{
    return this->currentDate;
}

std::chrono::year_month TM_MonthView::getMonthYear()
{
    return month;
}

void TM_MonthView::setMonthYear(std::chrono::year_month monthYear)
{
    this->month = monthYear;
}