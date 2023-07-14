#include <iostream>
#include <string>
#include <skia/include/core/SkCanvas.h>
#include <skia/include/core/SkFont.h>
#include <skia/include/core/SkFontMetrics.h>

using namespace std;

class Button 
{
	public:
		Button(float x, float y, float w, float h, float padding, string str)
		{
			this->x=x;	
			this->y=y;
			this->w=w;
			this->h=h;
			this->padding=padding;
			this->str = str;
		}

		bool pollPress(int mouseX, int mouseY)
		{
			return mouseX>x&&mouseX<x+w&&mouseY>y&&mouseY<y+h;
		}

		void renderButton(SkCanvas* c, SkPaint* p, SkFont* f)
		{
			SkFontMetrics fMet;
			f->getMetrics(&fMet);
			p->setColor(SK_ColorGREEN);
			c->drawRect(SkRect::MakeXYWH(x,y,w,h), *p);
			p->setColor(SK_ColorBLACK);
			p->setStyle(SkPaint::kStroke_Style);
			c->drawRect(SkRect::MakeXYWH(x,y,w,h), *p);
			p->setStyle(SkPaint::kFill_Style);
			c->drawString(str.c_str(),x+padding,y+fMet.fCapHeight+padding,*f,*p);
		}
	private:
		float x,y,w,h,padding;
		string str;
};
