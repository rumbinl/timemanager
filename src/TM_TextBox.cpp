#include <TM_Core.hpp>
#include <TM_UI.hpp>

TM_TextBox::TM_TextBox(SkRect bounds, std::string placeholder="", TM_ViewSetting viewSetting) : TM_TextView(placeholder, bounds, viewSetting, false)
{
    this->placeholder = placeholder;
}

void TM_TextBox::Render(SkCanvas* skia_canvas, SkFont* font)
{
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
    TM_TextView::Render(skia_canvas, font);
	SkPaint paint;
	paint.setColor(this->viewSetting.textColor);
	paint.setStrokeWidth(5);
	paint.setStyle(SkPaint::kStrokeAndFill_Style);
	skia_canvas->drawLine(this->bounds.x()+this->cursorX, this->bounds.y(), this->bounds.x()+this->cursorX, this->bounds.y()+this->viewSetting.fontSize, paint);
}

void TM_TextBox::locatePosition(SkScalar mouseX, std::string text, SkFont* font)
{
	std::vector<SkGlyphID> textGlyphs(text.size());
	std::vector<SkScalar> xPos(text.size());
	font->textToGlyphs(text.c_str(), text.size()*sizeof(char), SkTextEncoding::kUTF8, &textGlyphs[0],512);
	font->getXPos(&textGlyphs[0], text.size(), &xPos[0], 0);
	
	this->cursorIndex = 0;	
	this->cursorX = 0.0f;
	for(int i=1; i<textGlyphs.size(); i++)
	{
		SkScalar newX = textGlyphs[i];
		if(fabs(newX-mouseX)>=fabs(this->cursorIndex-mouseX))
			break;
		cursorIndex = i;
		cursorX = newX;
	}
}

bool TM_TextBox::PollEvents(TM_EventInput eventInput)
{
	bool contains = TM_TextView::getBounds().contains(eventInput.mouseX,eventInput.mouseY);
	if(this->selected == true && !contains && eventInput.mousePressed)
	{
		this->selected = false;
		return true;
	}
    if((contains && eventInput.mousePressed) || this->selected)
    {
		this->selected = true;
		bool ret = false;
		if(eventInput.scrollX != 0)
		{
			TM_TextView::setTextXOffset(eventInput.scrollX);
			ret = true;
		}

		if(eventInput.inputText.size()>0)
		{
			TM_TextView::setText(TM_TextView::getText()+eventInput.inputText);
			ret = true;
		}
		return false;
    }
	if( this->selected == true)
	{
		this->selected = false;
		return true;
	}
    return false;
}
