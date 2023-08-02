#include <TM_UI.hpp>

TM_CalendarMonthView::TM_CalendarMonthView(int month, int year, SkScalar width, SkScalar height)
{
    this->dayViewList = std::vector(6*7, TM_TextView("0", width/7.0f, 0, {colorScheme[1],colorScheme[2],colorScheme[3],1,24}));
    this->dataView = new TM_TextView("January 1981",width, 0, {colorScheme[3],colorScheme[2],colorScheme[0],0,36});
    this->firstDay = 2; 
    this->numDays = 31;
    this->width = width;
    this->height = height;
    this->month = month;
    this->year = year;
}

void TM_CalendarMonthView::Render(SkCanvas* skia_canvas, SkFont* font)
{
    SkFontMetrics fontMetrics;
    font->getMetrics(&fontMetrics);
    this->dataView->setText(monthNames[this->month-1]+' '+std::to_string(this->year));
    this->dataView->setHeightFont(font);
    this->dataView->Render(skia_canvas, font);
    skia_canvas->translate(this->firstDay*(this->dayViewList[0].getWidth()),this->dataView->getHeight());
    for(int i=0;i<this->numDays;i++)
    {
        if((i+firstDay)%7==0 && i)
            skia_canvas->translate(-this->width, this->dayViewList[0].getHeight());
        this->dayViewList[i].setHeight((this->height-this->dataView->getHeight())/6.0f);
        this->dayViewList[i].setText(std::to_string(i+1));
        this->dayViewList[i].Render(skia_canvas, font);
        skia_canvas->translate(this->dayViewList[0].getWidth(),0);
    }
}

TM_CalendarMonthView::~TM_CalendarMonthView()
{

}