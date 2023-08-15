#include <TM_UI.hpp>

TM_CalendarMonthView::TM_CalendarMonthView(SkScalar x, SkScalar y, int month, int year, SkScalar width, SkScalar height)
{
    this->dayViewList = std::vector(31, TM_Button("0", 0.0f, 0.0f, width/7.0f, 0));
    this->dataView = new TM_TextView("January 1981", width, 0, x,y,{colorScheme[3],colorScheme[2],colorScheme[0],0,36});
    this->firstDay = 2; 
    this->numDays = 31;
    this->numRows = (numDays+firstDay+6)/7;
    this->numColumns = 7.0f;
    this->month = month;
    this->year = year;
    this->bounds = SkRect::MakeXYWH(x,y,width,height);
}

void TM_CalendarMonthView::Render(SkCanvas* skia_canvas, SkFont* font)
{
    SkFontMetrics fontMetrics;
    font->getMetrics(&fontMetrics);
    this->dataView->setText(monthNames[this->month-1]+' '+std::to_string(this->year));
    this->dataView->setHeightFont(font);
    this->dataView->Render(skia_canvas, font);
    SkScalar x=this->firstDay*(this->dayViewList[0].getWidth())+this->bounds.x(),y=this->dataView->getHeight()+this->bounds.y();
    for(int i=0;i<this->numDays;i++)
    {
        if((i+firstDay)%7==0 && i)
        {
            x-=this->bounds.width();
            y+=this->dayViewList[0].getHeight();
        }
        this->dayViewList[i].setHeight((this->bounds.height()-this->dataView->getHeight())/numRows);
        this->dayViewList[i].setX(x);
        this->dayViewList[i].setY(y);
        this->dayViewList[i].setText(std::to_string(i+1));
        if(i == hoverDayButton)
            this->dayViewList[i].invertColors();
        this->dayViewList[i].Render(skia_canvas, font);
        if(i == hoverDayButton)
            this->dayViewList[i].invertColors();
        x+=this->dayViewList[0].getWidth();
    }
}

bool TM_CalendarMonthView::PollEvents(float x, float y)
{
    bool select=false;
    if(this->bounds.contains(x,y))
    {
        for(int i = 0; i<this->dayViewList.size(); i++)
        {
            if(this->dayViewList[i].PollEvent(x,y))
            {
                if(this->hoverDayButton!=i)
                {
                    this->hoverDayButton = i;
                    select = true;
                }
            }
        }
    }
    else if(this->hoverDayButton!=-1)
    {
        this->hoverDayButton = -1;
        return true;
    }
    return select;
}

TM_CalendarMonthView::~TM_CalendarMonthView()
{

}