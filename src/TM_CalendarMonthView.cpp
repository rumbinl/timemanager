#include <TM_CalendarMonthView.hpp>

template <typename T> TM_CalendarMonthView<T>::TM_CalendarMonthView(SkRect bounds, T* datePtr, void (*setDateFunc)(T* taskManPtr, TM_YMD date), TM_YMD (*getDateFunc)(T* taskManPtr), TM_ViewSetting viewSetting) : TM_RenderObject(bounds, viewSetting)
{
    this->datePtr = datePtr;
    this->setDateFunc = setDateFunc;
    this->getDateFunc = getDateFunc;
    this->dayViewList = std::vector(31, TM_TextView("0", SkRect::MakeWH(bounds.width()/7.0f, 0), {colorScheme[0],colorScheme[2],colorScheme[3],0,24,0,0}));
    this->dataView = new TM_TextView("", bounds, {colorScheme[1],colorScheme[2],colorScheme[3],0,24,0,10});
    this->datePlaceholder = getCurrentDate();
    this->weekList = std::vector(7, TM_TextView("XX", SkRect::MakeWH(this->bounds.width()/7.0f, 0.0f), {colorScheme[1],colorScheme[2],colorScheme[3],0,20,0,10}));
}

template <typename T> void TM_CalendarMonthView<T>::Render(TM_RenderInfo renderInfo)
{
    renderInfo.textFont->setSize(this->viewSetting.fontSize);

    TM_YMD currentDate = this->getDateFunc(this->datePtr);

    if(currentDate == ZeroDate)
        currentDate = this->datePlaceholder;

    int firstDay = weekDayFromDate({currentDate.year(),currentDate.month(),std::chrono::day{1}}),
        numDays  = TM_NumMonthDays(currentDate),
        numRows  = (numDays+firstDay+6)/7;

    this->dataView->setX(this->bounds.x());
    this->dataView->setY(this->bounds.y());
    this->dataView->setText(monthNames[static_cast<unsigned>(currentDate.month())]+' '+std::to_string(static_cast<int>(currentDate.year())));
    this->dataView->setWidth(this->bounds.width());
    this->dataView->setHeightFont(renderInfo.textFont);
    this->dataView->Render(renderInfo);

    SkScalar x=firstDay*(this->bounds.width()/7.0f)+this->bounds.x(),y=this->dataView->getHeight()+this->bounds.y();
    for(int i=0;i<7;i++)
    {
        this->weekList[i].setText(dayNames[i].substr(0,2).c_str());
        this->weekList[i].setWidth(this->bounds.width()/7.0f);
        this->weekList[i].setX(this->bounds.x()+(float)i*this->bounds.width()/7.0f);
        this->weekList[i].setY(y);
        this->weekList[i].setHeightFont(renderInfo.textFont);
        this->weekList[i].Render(renderInfo);
    }

    y+=this->weekList[0].getHeight();

    for(int i=0;i<numDays;i++)
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
        TM_YMD day = {currentDate.year(), currentDate.month(), std::chrono::day{(unsigned)i+1}};
        if(day == currentDate)
            this->dayViewList[i].invertColors();
        this->dayViewList[i].Render(renderInfo);
        if(day == currentDate)
            this->dayViewList[i].invertColors();
        x+=this->bounds.width()/7.0f;
    }
}

template<typename T> bool TM_CalendarMonthView<T>::PollEvents(TM_EventInput eventInput) 
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
                    TM_YMD currentDate = this->getDateFunc(this->datePtr);
                    if(currentDate == ZeroDate) 
                        currentDate = datePlaceholder;
                    if(this->selectDayButton==i)
                    {
                        this->selectDayButton = -1;
                        if(this->getDateFunc(this->datePtr) != ZeroDate)
                            this->setDateFunc(this->datePtr, {currentDate.year(),currentDate.month(),std::chrono::day{1}});
                        else
                            this->datePlaceholder = {currentDate.year(),currentDate.month(),std::chrono::day{1}};
                    }
                    else
                    {
                        this->selectDayButton = i;
                        if(this->getDateFunc(this->datePtr) != ZeroDate)
                            this->setDateFunc(this->datePtr, {currentDate.year(),currentDate.month(),std::chrono::day{(unsigned)i+1}});
                        else
                            this->datePlaceholder = {this->month.year(),this->month.month(),std::chrono::day{(unsigned)i+1}};
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

template <typename T> void TM_CalendarMonthView<T>::setDate(TM_YMD date)
{
    this->taskManPtr->setStartDate(date);
}

template <typename T> std::chrono::year_month TM_CalendarMonthView<T>::getMonthYear()
{
    return month;
}

#include <TM_CalendarView.hpp>

void dummyFunc()
{
    TM_CalendarMonthView<TM_TaskManager> monthView1(SkRect::MakeEmpty());
    TM_CalendarMonthView<TM_CalendarView> monthView2(SkRect::MakeEmpty());
}