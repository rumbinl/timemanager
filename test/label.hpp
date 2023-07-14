#include <iostream>
#include <string>
#include <skia/include/core/SkCanvas.h>
#include <skia/include/core/SkRect.h>
#include <skia/include/core/SkFont.h>
#include <skia/include/core/SkFontMetrics.h>

using namespace std;

class Label 
{
	public:
		Label(float x, float y, string str, SkColor color)
		{
			this->x=x;
			this->y=y;
			this->str = str;
			this->color = color;
		}

		void renderLabel(SkCanvas* canvas, SkPaint* p, SkFont* f)
		{
			SkFontMetrics fMet;
			f->getMetrics(&fMet);
			p->setColor(this->color);
			canvas->drawRect(SkRect::MakeXYWH(x,y,fMet.fCapHeight,fMet.fCapHeight),*p);
			p->setColor(SK_ColorBLACK);
			canvas->drawString(this->str.c_str(),x+fMet.fCapHeight+5,y+fMet.fCapHeight,*f,*p);
		}
	private:
		float x,y;
		string str;
		SkColor color;
};
