#include <TM_CalendarMonthView.hpp>

TM_CalendarMonthView::TM_CalendarMonthView(SkRect bounds, std::chrono::year_month_day* datePtr, TM_ViewSetting viewSetting) : TM_RenderObject(bounds, viewSetting)
{
    this->datePtr = datePtr;
    this->dayViewList = std::vector(31, TM_TextView("0", SkRect::MakeWH(bounds.width()/7.0f, 0)));
    this->dataView = new TM_TextView("January 1981", bounds, {colorScheme[1],colorScheme[2],colorScheme[3],1,36,0});

    std::chrono::year_month_day today = getCurrentDate();
    this->setMonth({today.year(),today.month()});

    this->weekList = std::vector(7, TM_TextView("XX", SkRect::MakeWH(this->bounds.width()/7.0f, 0.0f)));
}

void TM_CalendarMonthView::setMonth(std::chrono::year_month ym)
{
    this->month = ym;
    this->firstDay = weekDayFromDate({ym.year(),ym.month(),std::chrono::day{1}});
    this->numDays = TM_NumMonthDays(this->month);
    this->numRows = (this->numDays+this->firstDay+6)/7;
}

void TM_CalendarMonthView::setDate(std::chrono::year_month_day date)
{
    *this->datePtr = date;
}

void TM_CalendarMonthView::setDatePtr(std::chrono::year_month_day* datePtr)
{
    this->datePtr = datePtr;
}

std::chrono::year_month TM_CalendarMonthView::getMonthYear()
{
    return month;
}

void TM_CalendarMonthView::Render(TM_RenderInfo renderInfo)
{
    SkFontMetrics fontMetrics;
    renderInfo.textFont->getMetrics(&fontMetrics);

    this->dataView->setX(this->bounds.x());
    this->dataView->setY(this->bounds.y());
    this->dataView->setText(monthNames[static_cast<unsigned>(this->month.month())-1]+' '+std::to_string(static_cast<int>(this->month.year())));
    this->dataView->setWidth(this->bounds.width());
    this->dataView->setHeightFont(renderInfo.textFont);
    this->dataView->Render(renderInfo);

    SkScalar x=this->firstDay*(this->bounds.width()/7.0f)+this->bounds.x(),y=this->dataView->getHeight()+this->bounds.y();
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
        std::chrono::year_month_day day = {this->month.year(), this->month.month(), std::chrono::day{(unsigned)i+1}};
        if((this->datePtr!=NULL && day == *this->datePtr) || (!this->datePtr&&this->datePlaceholder == day))
            this->dayViewList[i].invertColors();
        this->dayViewList[i].Render(renderInfo);
        if((this->datePtr!=NULL && day == *this->datePtr) || (!this->datePtr&&this->datePlaceholder == day))
            this->dayViewList[i].invertColors();
        x+=this->bounds.width()/7.0f;
    }
}

bool TM_CalendarMonthView::PollEvents(TM_EventInput eventInput)
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
                        if(this->datePtr!=NULL)
                            *datePtr = {this->month.year(),this->month.month(),std::chrono::day{1}};
                        else
                            this->datePlaceholder = {this->month.year(),this->month.month(),std::chrono::day{1}};
                    }
                    else
                    {
                        this->selectDayButton = i;
                        if(this->datePtr!=NULL)
                            *datePtr = {this->month.year(),this->month.month(),std::chrono::day{(unsigned)i+1}};
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