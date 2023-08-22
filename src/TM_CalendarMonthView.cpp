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
    this->weekList = std::vector(7, TM_TextView("XX", width/7.0f, 0.0f));
}

void TM_CalendarMonthView::Render(SkCanvas* skia_canvas, SkFont* font)
{
    SkFontMetrics fontMetrics;
    font->getMetrics(&fontMetrics);
    this->dataView->setText(monthNames[this->month-1]+' '+std::to_string(this->year));
    this->dataView->setHeightFont(font);
    this->dataView->Render(skia_canvas, font);
    SkScalar x=this->firstDay*(this->dayViewList[0].getWidth())+this->bounds.x(),y=this->dataView->getHeight()+this->bounds.y();
    for(int i=0;i<7;i++)
    {
        this->weekList[i].setText(dayNames[i].substr(0,2).c_str());
        this->weekList[i].setX((float)i*this->bounds.width()/7.0f);
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
        this->dayViewList[i].setHeight((this->bounds.height()-this->dataView->getHeight())/numRows);
        this->dayViewList[i].setX(x);
        this->dayViewList[i].setY(y);
        this->dayViewList[i].setText(std::to_string(i+1));
        if(i == hoverDayButton || i == selectDayButton)
            this->dayViewList[i].invertColors();
        this->dayViewList[i].Render(skia_canvas, font);
        if(i == hoverDayButton || i == selectDayButton)
            this->dayViewList[i].invertColors();
        x+=this->dayViewList[0].getWidth();
    }
}

bool TM_CalendarMonthView::PollEvents(float x, float y, bool isPressed)
{
    bool select=false;
    if(this->bounds.contains(x,y))
    {
        for(int i = 0; i<this->dayViewList.size(); i++)
        {
            if(this->dayViewList[i].PollEvent(x,y,isPressed))
            {
                this->hoverDayButton = i;
                select = true;
                if(isPressed)
                {
                    if(this->selectDayButton==i)
                        this->selectDayButton = -1;
                    else
                        this->selectDayButton = i;
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

TM_CalendarMonthView::~TM_CalendarMonthView()
{

}