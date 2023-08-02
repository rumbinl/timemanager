#include <TM_UI.hpp>

TM_TextView::TM_TextView(std::string text, SkScalar width, SkScalar height, TM_ViewSetting viewSetting)
{
    this->text = text;
    this->width = width;
    this->height = height;
    this->viewSetting = viewSetting;
}

void TM_TextView::Render(SkCanvas* skia_canvas, SkFont* font)
{
    SkPaint paint;
    paint.setColor(this->viewSetting.backgroundColor);
    paint.setStyle(SkPaint::kFill_Style);
    skia_canvas->drawRect(SkRect::MakeWH(this->width,this->height),paint);
    paint.setStyle(SkPaint::kStroke_Style);
    paint.setColor(this->viewSetting.borderColor);
    paint.setStrokeWidth(this->viewSetting.borderThickness);
    skia_canvas->drawRect(SkRect::MakeWH(this->width,this->height),paint);

    paint.setColor(this->viewSetting.textColor);
    paint.setStyle(SkPaint::kFill_Style);
    font->setSize(this->viewSetting.fontSize);

    SkFontMetrics font_metrics;
    font->getMetrics(&font_metrics);
    SkScalar fontHeight = font_metrics.fDescent;
    SkRect text_bounds;
    font->measureText(this->text.c_str(), this->text.length()*sizeof(char), SkTextEncoding::kUTF8, &text_bounds, &paint);
    skia_canvas->drawString(this->text.c_str(), this->width/2 - text_bounds.width()/2, this->height/2+fontHeight, *font, paint);
}

void TM_TextView::setText(std::string newText)
{
    this->text = newText;
}

SkScalar TM_TextView::getWidth()
{
    return this->width;
}

SkScalar TM_TextView::getHeight()
{
    return this->height;
}

void TM_TextView::setHeight(SkScalar newHeight)
{
    this->height = newHeight;
}

void TM_TextView::setHeightFont(SkFont* font)
{
    font->setSize(this->viewSetting.fontSize);
    SkFontMetrics fontMetrics;
    font->getMetrics(&fontMetrics);
    this->height = fontMetrics.fDescent-fontMetrics.fAscent;
}

void TM_TextView::setWidth(SkScalar newWidth)
{
    this->width = newWidth;
}

std::string TM_TextView::getText()
{
    return this->text;
}

TM_TextView::~TM_TextView()
{

}