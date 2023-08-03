#include <TM_UI.hpp>

TM_TextView::TM_TextView(std::string text, SkScalar width, SkScalar height, SkScalar x, SkScalar y, TM_ViewSetting viewSetting)
{
    this->text = text;
    this->bounds = SkRect::MakeXYWH(x,y,width,height);
    this->viewSetting = viewSetting;
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
    SkScalar fontHeight = font_metrics.fDescent;
    SkRect text_bounds;
    font->measureText(this->text.c_str(), this->text.length()*sizeof(char), SkTextEncoding::kUTF8, &text_bounds, &paint);
    skia_canvas->drawString(this->text.c_str(), this->bounds.x()+this->bounds.width()/2 - text_bounds.width()/2, this->bounds.y()+this->bounds.height()/2+fontHeight, *font, paint);
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

std::string TM_TextView::getText()
{
    return this->text;
}

TM_TextView::~TM_TextView()
{

}