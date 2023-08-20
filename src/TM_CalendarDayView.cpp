#include <TM_UI.hpp>

TM_CalendarDayView::TM_CalendarDayView(SkScalar x, SkScalar y, SkScalar width, SkScalar height, SkScalar hourHeight, TM_ViewSetting viewSettings)
{
	this->bounds = SkRect::MakeXYWH(x,y,width,height);
	this->viewSettings = viewSettings;
	this->hourHeight = hourHeight;
	this->scrollY = 0;
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
	paint.setColor(this->viewSettings.backgroundColor);

	skia_canvas->drawLine(this->bounds.x(), this->pressIndexStart-this->scrollY, this->bounds.x()+this->bounds.width(), this->pressIndexStart-this->scrollY,paint);
	paint.setColor(this->viewSettings.borderColor);

	font->setSize(this->viewSettings.fontSize);

	SkFontMetrics fontMetrics;
	font->getMetrics(&fontMetrics);

	SkScalar yOff = -this->scrollY;
	for(int i=0;i<24;i++)
	{
		SkScalar y = this->bounds.y() + i*this->hourHeight;
		std::string timeString = std::to_string(i)+":00";
		SkRect text_bounds;
		
		font->measureText(timeString.c_str(), timeString.size()*sizeof(char), SkTextEncoding::kUTF8, &text_bounds, &paint);
		if(!i)
		{
			this->viewSettings.padding = text_bounds.height()-fontMetrics.fBottom;
			yOff += this->viewSettings.padding;
		}
		skia_canvas->drawString(timeString.c_str(), this->bounds.x(), y+yOff+fontMetrics.fBottom, *font, paint);
		skia_canvas->drawLine(this->bounds.x()+text_bounds.width(), y+yOff, this->bounds.x()+this->bounds.width(), y+yOff, paint);
	}

	skia_canvas->restore();
}

bool TM_CalendarDayView::PollEvents(float x, float y, float scrollY, bool pressed)
{
	if(this->bounds.contains(x,y))
	{
		if(scrollY!=0)
		{
			this->scrollY+=scrollY;
			this->scrollY = fmax(0, this->scrollY);
			return true;
		}
		if(pressed&&this->selected == false)
		{
			this->pressIndexStart = y;
			this->selected = true;
			return true;
		}
	}
	if(this->selected && !pressed)
	{
		this->selected = false;
		this->pressIndexStart = -1;
		this->pressIndexEnd = -1;
		return true;
	}
	return false;
}

TM_CalendarDayView::~TM_CalendarDayView()
{
}
