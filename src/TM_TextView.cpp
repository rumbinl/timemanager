#include <TM_UI.hpp>

TM_TextView::TM_TextView(std::string text, SkScalar width, SkScalar height, SkScalar x, SkScalar y, TM_ViewSetting viewSetting, bool centered)
{
    this->text = text;
    this->bounds = SkRect::MakeXYWH(x,y,width,height);
    this->viewSetting = viewSetting;
    this->centered = centered;
}

void TM_TextView::Render(SkCanvas* skia_canvas, SkFont* font)
{
    SkPaint paint;
    paint.setColor(this->viewSetting.backgroundColor);
    paint.setStyle(SkPaint::kFill_Style);
    skia_canvas->drawRect(this->bounds,paint);
    paint.setStyle(SkPaint::kStroke_Style);
    paint.setColor(this->viewSetting.borderColor);
    paint.setStrokeWidth(this->viewSetting.borderThickness);
    skia_canvas->drawRect(this->bounds,paint);

    paint.setColor(this->viewSetting.textColor);
    paint.setStyle(SkPaint::kFill_Style);
    font->setSize(this->viewSetting.fontSize);

    SkFontMetrics font_metrics;
    font->getMetrics(&font_metrics);
    SkScalar fontHeight = font_metrics.fDescent,textX,textY;
    SkRect text_bounds;
    font->measureText(this->text.c_str(), this->text.length()*sizeof(char), SkTextEncoding::kUTF8, &text_bounds, &paint);
    if(this->centered)
        textX = this->bounds.x()+this->bounds.width()/2 - text_bounds.width()/2, textY = this->bounds.y()+this->bounds.height()/2+fontHeight;
    else 
        textX = this->bounds.x(), textY = this->bounds.y()+font_metrics.fCapHeight;
    skia_canvas->drawString(this->text.c_str(), textX, textY, *font, paint);
}

void TM_TextView::setText(std::string newText)
{
    this->text = newText;
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
    this->bounds.setWH(newWidth, this->bounds.height());
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

TM_TextView::~TM_TextView()
{

}
