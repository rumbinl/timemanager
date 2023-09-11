#include <TM_Core.hpp>
#include <TM_UI.hpp>

TM_TextBox::TM_TextBox(SkRect bounds, std::string placeholder="", TM_ViewSetting viewSetting) : TM_TextView(placeholder, bounds, viewSetting, false)
{
    this->placeholder = placeholder;
}

void TM_TextBox::Render(TM_RenderInfo renderInfo)
{
	SkFont* font = renderInfo.textFont;
    if(!this->fitted)
    {
        TM_TextView::setHeightFont(font);
        this->fitted = true;
    }
    if(this->content == "")
	{
		TM_TextView::setText(this->placeholder);
        TM_TextView::setColorOpacity(150);
	}
    else
	{
		TM_TextView::setText(this->content);
        TM_TextView::setColorOpacity(255);
	}
	SkFontMetrics fontMetrics;
	font->getMetrics(&fontMetrics);
    TM_TextView::Render(renderInfo);
	TM_TextView::setColorOpacity(255);
	if(this->select) TM_TextView::invertColors();
	if(this->select)
	{
		SkPaint paint;
		paint.setColor(this->viewSetting.textColor);
		paint.setStrokeWidth(2);
		paint.setStyle(SkPaint::kStrokeAndFill_Style);
		SkRect textBounds;
		font->measureText(this->content.substr(0,this->cursorIndex).c_str(), this->cursorIndex*sizeof(char), SkTextEncoding::kUTF8, &textBounds);
		this->cursorX = textBounds.width()-TM_TextView::getTextXOffset();
		renderInfo.canvas->drawLine(this->bounds.x()+this->cursorX, this->bounds.y(), this->bounds.x()+this->cursorX, this->bounds.y()+this->viewSetting.fontSize-fontMetrics.fDescent, paint);
	}
	if(this->select) TM_TextView::invertColors();
}

void TM_TextBox::locatePosition(SkScalar mouseX, std::string text, SkFont* font)
{
	SkRect textBounds;
	int l=0,h=text.size()-1;
	while(h-l>1)
	{
		int mid = (l+h)/2;
		font->measureText(text.c_str(), (mid)*sizeof(char), SkTextEncoding::kUTF8, &textBounds);
		if(textBounds.width()>=mouseX)
			h = mid;
		else
			l = mid;
	}
	int lb = fabs(mouseX-font->measureText(text.c_str(),(l)*sizeof(char),SkTextEncoding::kUTF8,&textBounds));
	int ub = fabs(mouseX-font->measureText(text.c_str(),(h)*sizeof(char),SkTextEncoding::kUTF8,&textBounds));
	cursorIndex = h;
	if(lb<ub)
		cursorIndex = l;
}

bool TM_TextBox::PollEvents(TM_EventInput eventInput)
{
	bool contains = TM_TextView::getBounds().contains(eventInput.mouseX,eventInput.mouseY);
	bool ret = false;

	if(this->select == true && !contains && eventInput.mousePressed)
	{
		this->select = false;
		ret = true;
	}

	if(contains)
	{
		if(eventInput.mousePressed)
		{
			eventInput.font->setSize(this->viewSetting.fontSize);
			this->locatePosition(eventInput.mouseX-this->bounds.x()+TM_TextView::getTextXOffset(),this->content+" ",eventInput.font);
			this->select = true;
			ret = true;
		}
		if(eventInput.scrollX != 0)
		{
			TM_TextView::setTextXOffset(eventInput.scrollX);
			ret = true;
		}
	}

    if(this->select)
    {
		if(eventInput.inputText.size()>0 && this->content.size()<512)
		{
			std::string s0 = this->content.substr(this->cursorIndex,this->content.size()), s1 = this->content.substr(0,cursorIndex);
			this->content = s1 + eventInput.inputText + s0;
			this->cursorIndex ++;
			ret = true;
		}
		else if(eventInput.keyPressed && eventInput.key == SDL_SCANCODE_BACKSPACE && cursorIndex)
		{
			std::string s0 = this->content.substr(this->cursorIndex,this->content.size()),s1 = this->content.substr(0,cursorIndex-1);
			this->content = s1+s0;
			this->cursorIndex --;
			ret = true;
		}
    }
    return ret;
}

SkScalar TM_TextBox::getCharWidth(char a, SkFont* font)
{
	SkGlyphID glyph;
	font->textToGlyphs(&a, sizeof(char), SkTextEncoding::kUTF8, &glyph, 1);
	SkScalar width;
	font->getWidths(&glyph, 1, &width);
	return width;
}
