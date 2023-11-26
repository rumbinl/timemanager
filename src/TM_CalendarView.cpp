#include <TM_CalendarView.hpp>

TM_CalendarView::TM_CalendarView(SkRect bounds, TM_TaskManager* taskManPtr) : TM_View(bounds, {0.05,0.95},{}, {colorScheme[0],colorScheme[3],colorScheme[3],0,24,0,10})
{
    this->taskManPtr = taskManPtr;
    this->currentDate = getCurrentDate();
    this->weekView = new TM_CalendarWeekView(SkRect::MakeXYWH(0, 0, 0, 480), &this->currentDate, taskManPtr);
    this->monthView = new TM_CalendarMonthView(SkRect::MakeXYWH(0, 0, 250, 250), (void*)this, 
		[](void* calendarView, TM_YMD date){

			((TM_CalendarView*)calendarView)->setReferenceDate(date);
		}, 
		[](void* calendarView) -> TM_YMD
		{
			return ((TM_CalendarView*)calendarView)->getReferenceDate();
		}
	);

	this->vitalView = new TM_View(SkRect::MakeEmpty(), {}, {
		this->monthView,
		new TM_View(SkRect::MakeEmpty(), {0.10, 0.90}, { 
			new TM_HorizontalView(SkRect::MakeEmpty(), { 
				new TM_Button<int>("\ue3cb\ue8f3", SkRect::MakeEmpty(), 0, this->weekView, 
					[](void* contextPtr, int data,TM_Button<int>* self)
					{
						TM_CalendarWeekView* context = (TM_CalendarWeekView*)contextPtr;
						context->setDaySpan(context->getDaySpan()-1);
					}, {colorScheme[1],colorScheme[2],colorScheme[3],0,24,5,5,true}),

				new TM_Button<int>("\ue3cd\ue8f3", SkRect::MakeEmpty(), 0, this->weekView, 
					[](void* contextPtr, int data,TM_Button<int>* self)
					{
						TM_CalendarWeekView* context = (TM_CalendarWeekView*)contextPtr;
						context->setDaySpan(context->getDaySpan()+1);
					}, {colorScheme[1],colorScheme[2],colorScheme[3],0,24,5,5,true})
			}),
			this->weekView
		})
	});

	this->addRenderObject(new TM_Button<int>("\ue8f5\uebcc", SkRect::MakeWH(0,50), 0, (void*)this, 
		[](void* contextPtr,int data,TM_Button<int>* self) 
		{
			TM_RenderObject* context = (TM_RenderObject*)contextPtr;
			if(self->getText() == "\ue8f4\uebcc")
			{
				self->setText("\ue8f5\uebcc");
				context->getRenderObject(1)->setRenderObjectExistence(0,true);
			}
			else
			{
				self->setText("\ue8f4\uebcc");
				context->getRenderObject(1)->setRenderObjectExistence(0,false);
			}
		}, {colorScheme[1],colorScheme[2],colorScheme[3],0,24,5,5,true}));

    this->addRenderObject(this->vitalView);
}

void TM_CalendarView::setReferenceDate(TM_YMD date)
{
	this->currentDate = date;
}

TM_YMD TM_CalendarView::getReferenceDate()
{
	return this->currentDate;
}
