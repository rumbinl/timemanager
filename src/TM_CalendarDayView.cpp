#include <TM_UI.hpp>

TM_CalendarDayView::TM_CalendarDayView(SkScalar x, SkScalar y, SkScalar width, SkScalar height, SkScalar hourHeight, TM_ViewSetting viewSettings)
{
	this->bounds = SkRect::MakeXYWH(x,y,width,height);
	this->viewSettings = viewSettings;
	this->hourHeight = hourHeight;
}

void TM_CalendarDayView::Render(SkCanvas* skia_canvas, SkFont* font)
{
	SkPaint paint;
	paint.setStyle(SkPaint::kStroke_Style);
	paint.setColor(this->viewSettings.borderColor);
	skia_canvas->drawRect(this->bounds, paint);
	skia_canvas->save();
	skia_canvas->clipRect(this->bounds);
	paint.setStyle(SkPaint::kStrokeAndFill_Style);

	for(int i=0;i<24;i++)
	{
		SkScalar y = this->bounds.y() + (i+1)*this->hourHeight;
		skia_canvas->drawLine(this->bounds.x(), y, this->bounds.left(), y, paint);
	}

	skia_canvas->restore();
}

TM_CalendarDayView::~TM_CalendarDayView()
{
}
