#include <TM_MonthView.hpp>

TM_MonthView::TM_MonthView(SkRect bounds, void* contextPtr, void (*setDateFunc)(void* contextPtr, TM_YMD date), TM_YMD (*getDateFunc)(void* contextPtr), TM_ViewSetting viewSetting) : TM_View(bounds, {}, {}, viewSetting)
{
    this->idx = indexCal++;
    this->contextPtr = contextPtr;
    this->setDateFunc = setDateFunc;
    this->getDateFunc = getDateFunc;
    this->weekDayLabels = new TM_HorizontalView(SkRect::MakeEmpty(), {});
    for(std::string weekName : dayNames)
        weekDayLabels->addRenderObject(new TM_TextView(weekName.substr(0,2), SkRect::MakeEmpty(), viewSetting={colorScheme[0],colorScheme[2],colorScheme[3],0,24,0,0}));

    this->dayViewList = std::vector(31, 
        TM_SelectButton<int>("0", 
            SkRect::MakeWH(bounds.width()/7.0f, 0), 0, this, 
            [](void* monthViewPtr, int dayIndex)
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
        this->addRenderObject(&dayViewList[i]);
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
    SkScalar length = std::fmin(this->bounds.width(), this->bounds.height());
    SkScalar xOffset = (this->bounds.width()-length)/2.0f, yOffset = (this->bounds.height()-length)/2.0f;
    SkScalar originX = this->bounds.x() + xOffset, originY = this->bounds.y()+yOffset;
    renderInfo.canvas->clipRect(SkRect::MakeXYWH(originX, originY, length, length));
    renderInfo.canvas->translate(this->bounds.x(), this->bounds.y());
    SkScalar xStep,yStep;xStep = yStep = length/std::fmax(7.0f, (SkScalar)(numRows+1));
    SkScalar startX, x = startX = xOffset + (length - (xStep * 7.0f))/2.0f + firstDay*xStep,
             y = yStep + yOffset;

    this->weekDayLabels->setBounds(SkRect::MakeXYWH(xOffset, yOffset, length, yStep));
    this->weekDayLabels->Render(renderInfo);

    for(unsigned i=0; i<numDays; i++)
    {
        if((i+firstDay)%7==0 && i)
        {
            x -= xStep*7.0f;
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
    return TM_View::PollEvents(eventInput);
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