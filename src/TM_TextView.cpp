#include <TM_TextView.hpp>

TM_TextView::TM_TextView(std::string text, SkRect bounds, TM_ViewSetting viewSetting, bool centered) : TM_RenderObject(bounds, viewSetting)
{
    this->text = text;
    this->viewSetting = viewSetting;
    this->centered = centered;
    this->textXOffset = 0;
}

TM_TextView::TM_TextView(void* context, std::string (*getStringFunc)(void* context), SkRect bounds, TM_ViewSetting viewSetting, bool centered) : TM_TextView("", bounds, viewSetting, centered)
{
    this->context = context;
    this->getStringFunc = getStringFunc;
}

void TM_TextView::Render(TM_RenderInfo renderInfo)
{
    if(this->context != NULL && this->getStringFunc != NULL)
    {
        this->text = this->getStringFunc(this->context);
    }
	SkFont* font = (this->viewSetting.icon?renderInfo.iconFont:renderInfo.textFont);

	if(this->select) this->invertColors();
    SkPaint paint;

    paint.setColor(this->viewSetting.backgroundColor);
    paint.setStyle(SkPaint::kFill_Style);

    paint.setAntiAlias(true);
    renderInfo.canvas->drawRRect(SkRRect::MakeRectXY(this->bounds, this->viewSetting.cornerRadius, this->viewSetting.cornerRadius),paint);

    if(viewSetting.borderThickness>0)
    {
        paint.setStyle(SkPaint::kStroke_Style);
        paint.setColor(this->viewSetting.borderColor);
        paint.setStrokeWidth(this->viewSetting.borderThickness-1);
        renderInfo.canvas->drawRRect(SkRRect::MakeRectXY(this->bounds, this->viewSetting.cornerRadius, this->viewSetting.cornerRadius),paint);
    }

    renderInfo.canvas->save();
    renderInfo.canvas->translate(-this->textXOffset,0);

    paint.setColor(this->viewSetting.textColor);
    paint.setStyle(SkPaint::kFill_Style);
    font->setSize(std::fmin(this->viewSetting.fontSize*renderInfo.dpi,this->bounds.height()));

    SkFontMetrics fontMetrics;
    font->getMetrics(&fontMetrics);
    SkScalar fontHeight = fontMetrics.fDescent,textX,textY;
    SkRect text_bounds;
    font->measureText(this->text.c_str(), this->text.length()*1, SkTextEncoding::kUTF8, &text_bounds, &paint);
    if(text_bounds.width() > this->bounds.width())
    {
        font->setSize(((SkScalar)this->bounds.width()/(SkScalar)text_bounds.width())* (SkScalar)font->getSize());
        text_bounds.setWH(this->bounds.width(), text_bounds.height());
    }
    srcBounds.setWH(text_bounds.width(), this->bounds.height());
    if(this->centered)
        textX = this->bounds.x()+this->bounds.width()/2 - text_bounds.width()/2, textY = this->bounds.height()<this->viewSetting.fontSize*renderInfo.dpi?this->bounds.y()+this->bounds.height()-fontMetrics.fDescent:this->bounds.y()+this->bounds.height()/2+renderInfo.dpi*this->viewSetting.fontSize/2-fontMetrics.fDescent;
    else 
        textX = this->bounds.x(), textY = this->bounds.y()+this->viewSetting.fontSize*renderInfo.dpi-fontMetrics.fDescent;

    renderInfo.canvas->drawSimpleText(text.c_str(), text.size()*1, SkTextEncoding::kUTF8, textX, textY, *font, paint);
    renderInfo.canvas->restore();
	if(this->select) this->invertColors();
}

void TM_TextView::Render(std::string text, SkRect bounds, TM_RenderInfo renderInfo, TM_ViewSetting viewSetting, bool centered)
{
	SkFont* font = renderInfo.textFont;
    font->setSize(viewSetting.fontSize*renderInfo.dpi);
    SkFontMetrics fontMetrics;
    font->getMetrics(&fontMetrics);
    SkScalar fontHeight = viewSetting.fontSize*renderInfo.dpi,textX,textY;

    SkPaint paint;

    paint.setColor(viewSetting.backgroundColor);
    paint.setStyle(SkPaint::kFill_Style);

    renderInfo.canvas->drawRect(bounds,paint);

    if(viewSetting.borderThickness>0)
    {
        paint.setStyle(SkPaint::kStroke_Style);
        paint.setColor(viewSetting.borderColor);
        paint.setStrokeWidth(viewSetting.borderThickness-1);
    }

    renderInfo.canvas->drawRect(bounds,paint);

    int restore = renderInfo.canvas->save();
	renderInfo.canvas->clipRect(bounds);

    paint.setColor(viewSetting.textColor);
    paint.setStyle(SkPaint::kFill_Style);

    SkRect text_bounds;
    font->measureText(text.c_str(), text.length()*sizeof(char), SkTextEncoding::kUTF8, &text_bounds, &paint);
	if(centered)
		textX = bounds.x()+bounds.width()/2 - text_bounds.width()/2, textY = bounds.y()+bounds.height()/2+viewSetting.fontSize*renderInfo.dpi/2-fontMetrics.fDescent;
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

void TM_TextView::setHeightFont(SkFont* font, TM_RenderInfo renderInfo)
{
    this->bounds.setXYWH(this->bounds.x(), this->bounds.y(), this->bounds.width(), this->viewSetting.fontSize*renderInfo.dpi);
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
