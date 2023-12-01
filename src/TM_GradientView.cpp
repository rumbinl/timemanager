#include <TM_GradientView.hpp>

TM_GradientView::TM_GradientView(SkRect bounds, void* contextPtr, uint32_t (*getColor)(void* contextPtr), void (*setColor)(void* contextPtr, uint32_t color)) : TM_RenderObject(bounds, {colorScheme[0],colorScheme[3],colorScheme[3],0,24,0,0,false,20})
{
    this->contextPtr = contextPtr;
    this->getColor = getColor;
    this->setColor = setColor;
}

void TM_GradientView::Render(TM_RenderInfo renderInfo)
{
    if(this->getColor != NULL)
        this->x = getXFromColor(this->getColor(this->contextPtr));
    renderInfo.canvas->save();
    SkPaint paint;
    paint.setAntiAlias(true);
    renderInfo.canvas->clipRRect(SkRRect::MakeRectXY(this->bounds,this->viewSetting.cornerRadius,this->viewSetting.cornerRadius));
    
    SkScalar cx = bounds.width()/2.0f, cy = bounds.width()/2.0f;
    SkPoint points[] = {{this->bounds.x(),cy}, {this->bounds.x()+this->bounds.width(),cy}};

    paint.setShader(SkGradientShader::MakeLinear(points, this->colors, nullptr, 7, SkTileMode::kClamp));
    renderInfo.canvas->drawPaint(paint);
    paint.setShader(nullptr);
    
    SkScalar rectWidth = 20;
    SkRRect rect = SkRRect::MakeRectXY(SkRect::MakeXYWH(this->bounds.x() + this->x - rectWidth/2.0f, this->bounds.y() + 5, rectWidth, this->bounds.height()-10),10,10);

    paint.setStyle(SkPaint::kStroke_Style);
    paint.setStrokeWidth(5);
    paint.setColor(SK_ColorWHITE);

    renderInfo.canvas->drawRRect(rect, paint);

    renderInfo.canvas->restore();
}

bool TM_GradientView::PollEvents(TM_EventInput eventInput) 
{
    if(this->bounds.contains(eventInput.mouseX, eventInput.mouseY))
    {
        if(eventInput.mousePressed || eventInput.mouseHeld)
        {
            this->x = eventInput.mouseX -= this->bounds.x();
            if(this->setColor != NULL)
            {
                this->setColor(this->contextPtr, this->getColorFromX(this->x));
            }
            return true;
        }
    }
    return false;
}

SkColor TM_GradientView::getColorFromX(SkScalar x)
{
    int index = std::fmin(std::floor(6.0f * (SkScalar)x/(SkScalar)this->bounds.width()),5.0f);

    x -= (SkScalar)index * ((SkScalar)this->bounds.width()/6.0f);

    bool swap = colors[index+1]<colors[index];

    SkColor a = colors[index+1]^colors[index],
            b = (SkScalar)(0xff)*(x/(this->bounds.width()/6.0f));

    while(!(a&1))
    {
        b <<= 8;
        a >>= 8;
    }

    if(swap)
        return this->colors[index] - b;

    return this->colors[index] + b;
}

SkScalar TM_GradientView::getXFromColor(SkColor color)
{
    for(int i=0;i<6;i++)
    {
        SkColor temp = this->colors[i+1] ^ this->colors[i];
        SkColor stationary = this->colors[i+1] & this->colors[i];

        if(((color & stationary) == stationary) && !(color & (~(colors[i+1]|colors[i]))))
        {
            while(!(temp&1) && temp)
            {
                temp>>=4;
                color>>=4;
            }
            color %= 1<<8;
            if(colors[i+1]>colors[i])
                return i * (this->bounds.width()/6.0f) + (this->bounds.width()/6.0f) * (SkScalar)(color)/(SkScalar)0xff;
            else
                return i * (this->bounds.width()/6.0f) + (this->bounds.width()/6.0f) * (SkScalar)(0xff-color)/(SkScalar)0xff;
        }
    }
    return 0.0f;
}