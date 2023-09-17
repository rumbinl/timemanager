#include <TM_UI.hpp>

TM_TextView::TM_TextView(std::string text, SkRect bounds, TM_ViewSetting viewSetting, bool centered) : TM_RenderObject(bounds, viewSetting)
{
    this->text = text;
    this->viewSetting = viewSetting;
    this->centered = centered;
    this->textXOffset = 0;
}

void TM_TextView::Render(TM_RenderInfo renderInfo)
{
	SkFont* font = renderInfo.textFont;

	if(this->select) this->invertColors();
    SkPaint paint;

    paint.setColor(this->viewSetting.backgroundColor);
    paint.setStyle(SkPaint::kFill_Style);

    renderInfo.canvas->drawRect(this->bounds,paint);

    paint.setStyle(SkPaint::kStroke_Style);
    paint.setColor(this->viewSetting.borderColor);
    paint.setStrokeWidth(this->viewSetting.borderThickness);

    renderInfo.canvas->drawRect(SkRect::MakeXYWH(this->bounds.x(),this->bounds.y(),this->bounds.width(),this->bounds.height()),paint);

    int restore = renderInfo.canvas->save();
	renderInfo.canvas->clipRect(this->bounds);
    renderInfo.canvas->translate(-this->textXOffset,0);

    paint.setColor(this->viewSetting.textColor);
    paint.setStyle(SkPaint::kFill_Style);
    font->setSize(this->viewSetting.fontSize);

    SkFontMetrics fontMetrics;
    font->getMetrics(&fontMetrics);
    SkScalar fontHeight = fontMetrics.fDescent,textX,textY;
    SkRect text_bounds;
    font->measureText(this->text.c_str(), this->text.length()*sizeof(char), SkTextEncoding::kUTF8, &text_bounds, &paint);
    srcBounds.setWH(text_bounds.width(), this->bounds.height());
    if(this->centered)
        textX = this->bounds.x()+this->bounds.width()/2 - text_bounds.width()/2, textY = this->bounds.y()+this->bounds.height()/2+this->viewSetting.fontSize/2-fontMetrics.fDescent;
    else 
        textX = this->bounds.x(), textY = this->bounds.y()+this->viewSetting.fontSize-fontMetrics.fDescent;

    renderInfo.canvas->drawSimpleText(text.c_str(), text.size()*sizeof(char), SkTextEncoding::kUTF8, textX, textY, *font, paint);
    renderInfo.canvas->restoreToCount(restore);
	if(this->select) this->invertColors();
}

void TM_TextView::Render(std::string text, SkRect bounds, TM_RenderInfo renderInfo, TM_ViewSetting viewSetting, bool centered)
{
	SkFont* font = renderInfo.textFont;
    font->setSize(viewSetting.fontSize);
    SkFontMetrics fontMetrics;
    font->getMetrics(&fontMetrics);
    SkScalar fontHeight = viewSetting.fontSize,textX,textY;

    SkPaint paint;

    paint.setColor(viewSetting.backgroundColor);
    paint.setStyle(SkPaint::kFill_Style);

    renderInfo.canvas->drawRect(bounds,paint);

    paint.setStyle(SkPaint::kStroke_Style);
    paint.setColor(viewSetting.borderColor);
    paint.setStrokeWidth(viewSetting.borderThickness);

    renderInfo.canvas->drawRect(bounds,paint);

    int restore = renderInfo.canvas->save();
	renderInfo.canvas->clipRect(bounds);

    paint.setColor(viewSetting.textColor);
    paint.setStyle(SkPaint::kFill_Style);

    SkRect text_bounds;
    font->measureText(text.c_str(), text.length()*sizeof(char), SkTextEncoding::kUTF8, &text_bounds, &paint);
	if(centered)
		textX = bounds.x()+bounds.width()/2 - text_bounds.width()/2, textY = bounds.y()+bounds.height()/2+viewSetting.fontSize/2-fontMetrics.fDescent;
	else
		textX = bounds.x(), textY = bounds.y() + fontHeight-fontMetrics.fDescent;

    renderInfo.canvas->drawSimpleText(text.c_str(), text.size()*sizeof(char), SkTextEncoding::kUTF8, textX, textY, *font, paint);
    renderInfo.canvas->restoreToCount(restore);
}

void TM_TextView::setTextXOffset(SkScalar scrollX)
{
    SkScalar newTextXOffset = this->textXOffset + scrollX;
    this->textXOffset = fmin(fmax(newTextXOffset, 0), fmax(0,srcBounds.width()-this->bounds.width()));
}

void TM_TextView::setText(std::string newText)
{
    this->text = newText;
}

void TM_TextView::setColorOpacity(uint8_t opacity)
{
    this->viewSetting.textColor = SkColorSetA(this->viewSetting.textColor, opacity);
}

void TM_TextView::invertColors()
{
    std::swap(this->viewSetting.backgroundColor, this->viewSetting.textColor);
}

SkScalar TM_TextView::getWidth()
{
    return this->bounds.width();
}

SkScalar TM_TextView::getHeight()
{
    return this->bounds.height();
}

void TM_TextView::setHeight(SkScalar newHeight)
{
    this->bounds.setXYWH(this->bounds.x(), this->bounds.y(), this->bounds.width(), newHeight);
}

void TM_TextView::setHeightFont(SkFont* font)
{
    font->setSize(this->viewSetting.fontSize);
    SkFontMetrics fontMetrics;
    font->getMetrics(&fontMetrics);
    this->bounds.setXYWH(this->bounds.x(), this->bounds.y(), this->bounds.width(), fontMetrics.fDescent-fontMetrics.fAscent);
}

void TM_TextView::setWidth(SkScalar newWidth)
{
    this->bounds.setXYWH(this->bounds.x(), this->bounds.y(), newWidth, this->bounds.height());
}

void TM_TextView::setX(SkScalar x)
{
    this->bounds.offsetTo(x,this->bounds.y());
}

void TM_TextView::setY(SkScalar y)
{
    this->bounds.offsetTo(this->bounds.x(),y);
}

std::string TM_TextView::getText()
{
    return this->text;
}

SkScalar TM_TextView::getTextXOffset()
{
    return this->textXOffset;
}
