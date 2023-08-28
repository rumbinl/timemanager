#include <TM_UI.hpp>

TM_CalendarDayView::TM_CalendarDayView(SkScalar x, SkScalar y, SkScalar width, SkScalar height, SkScalar hourHeight, TM_ViewSetting viewSettings)
{
	this->bounds = SkRect::MakeXYWH(x,y,width,height);
	this->viewSettings = viewSettings;
	this->hourHeight = hourHeight;
	this->scrollY = 0;
	this->srcBounds = this->bounds;
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
	font->setSize(this->viewSettings.fontSize);
	SkFontMetrics fontMetrics;
	font->getMetrics(&fontMetrics);
	SkRect text_bounds;
	font->measureText("XX:XX", 5*sizeof(char), SkTextEncoding::kUTF8, &text_bounds, &paint);
	this->xOff = text_bounds.width();
	if(this->selected)
	{
		paint.setColor(this->viewSettings.backgroundColor);
		SkScalar topY=this->yOff + (this->hourHeight/12.0f)*round((this->pressIndexStart+this->scrollY-this->yOff)/(this->hourHeight/12.0f)),height = (this->hourHeight/12.0f)*round((this->pressIndexEnd-this->pressIndexStart)/(this->hourHeight/12.0f));
		SkRRect rect = SkRRect::MakeRectXY(SkRect::MakeXYWH(this->bounds.x()+xOff, topY, this->bounds.width()-xOff, height),20,20);
		skia_canvas->drawRRect(rect, paint);
	}
	paint.setColor(this->viewSettings.borderColor);

	SkScalar yOff = -this->scrollY, y = this->bounds.y();
	for(int i=0;i<24;i++)
	{
		std::string timeString = std::to_string(i)+":00";
		font->measureText(timeString.c_str(), timeString.size()*sizeof(char), SkTextEncoding::kUTF8, &text_bounds, &paint);

		if(!i)
		{
			this->viewSettings.padding = text_bounds.height()-fontMetrics.fBottom;
			y += this->viewSettings.padding;
		}

		skia_canvas->drawString(timeString.c_str(), xOff-text_bounds.width()+this->bounds.x(), y+yOff+fontMetrics.fBottom, *font, paint);
		skia_canvas->drawLine(this->bounds.x()+xOff, y+yOff, this->bounds.x()+this->bounds.width(), y+yOff, paint);
		y += this->hourHeight;
	}
	this->srcBounds.setWH(this->srcBounds.width(), y);

	skia_canvas->restore();
}

bool TM_CalendarDayView::PollEvents(float x, float y, float scrollY, bool pressed)
{
	if(this->bounds.contains(x,y))
	{
		if(scrollY!=0)
		{
			this->scrollY+=scrollY;
			SkScalar scrollLimitY = this->srcBounds.height()-this->bounds.height();
			this->scrollY = fmin(scrollLimitY, fmax(0, this->scrollY));
		}
		if(pressed&&this->selected == false)
		{
			this->pressIndexStart = y-this->scrollY;
			this->selected = true;
		}
		if(pressed)
			this->pressIndexEnd = y-this->scrollY;
		else
			selected = false;
		return true;
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
