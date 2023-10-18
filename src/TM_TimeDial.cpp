#include <TM_TimeDial.hpp>

TM_TimeDial::TM_TimeDial(SkRect bounds, TM_ViewSetting viewSetting) : TM_RenderObject(bounds, viewSetting)
{
}

void TM_TimeDial::Render(TM_RenderInfo renderInfo)
{
    SkPaint paint;
    paint.setColor(this->viewSetting.backgroundColor);
    paint.setStyle(SkPaint::kStroke_Style);
    paint.setAntiAlias(true);
    paint.setStrokeWidth(this->dialThickness);
    renderInfo.canvas->save();
    renderInfo.canvas->clipRect(this->bounds);
    renderInfo.canvas->translate(this->bounds.x(), this->bounds.y());
    SkScalar cx = this->bounds.width()/2.0f,cy = this->bounds.height()/2.0f, length = std::fmin(this->bounds.width(), this->bounds.height())/2.0f-this->dialThickness/2.0f;
    renderInfo.canvas->drawCircle(cx,cy, length, paint);

    paint.setColor(this->viewSetting.borderColor);
    SkRect arcTemplate = SkRect::MakeXYWH(cx - length, cy - length, 2*length, 2*length);
    SkScalar hourDivider = 6;
    SkScalar stepDialPercentage = round(dialProgressPercentage*24.0f*hourDivider/200.0f)/(12.0f*hourDivider);
    SkScalar stepDialPercentage2ndOrder = round((std::fmod(dialProgressPercentage,100))*12.0f*hourDivider/100.0f)/(12.0f*hourDivider);
    renderInfo.canvas->drawArc(arcTemplate, -90, 360.0f * stepDialPercentage, false, paint);
    paint.setStyle(SkPaint::kFill_Style);
    renderInfo.canvas->drawCircle(cx, cy-length, this->dialThickness/2.0f, paint);
    paint.setColor(SkColor4f::FromBytes_RGBA(0xff0055ff));
    paint.setStyle(SkPaint::kStroke_Style);
    if(this->dialProgressPercentage > 100)
    {
        renderInfo.canvas->drawArc(arcTemplate, -90, 360.0f * stepDialPercentage2ndOrder, false, paint);
        paint.setStyle(SkPaint::kFill_Style);
        renderInfo.canvas->drawCircle(cx, cy-length, this->dialThickness/2.0f, paint);
    }
    paint.setStyle(SkPaint::kFill_Style);
    paint.setColor(this->viewSetting.textColor);
    paint.setStrokeWidth(0);
    cx = cx + sin(2 * M_PI * stepDialPercentage) * length, cy = cy - length * cos(2 * M_PI * stepDialPercentage);
    renderInfo.canvas->drawCircle(cx,cy, this->dialThickness/2.0f, paint);
    renderInfo.canvas->restore();
}

SkScalar TM_TimeDial::GetAnglePercentage(TM_EventInput eventInput)
{
    SkScalar xDif = eventInput.mouseX - this->bounds.width()/2.0f, yDif = eventInput.mouseY - this->bounds.height()/2.0f;
    SkScalar angle;
    angle = acos(-yDif/sqrt(xDif*xDif + yDif*yDif));
    if(xDif < 0)
        angle = 2*M_PI-angle;
    return angle*100.0f/(2*M_PI);
}

SkScalar TM_TimeDial::SubtractAnglePercentages(SkScalar angle1, SkScalar angle2)
{
    if(std::fabs(angle1-angle2)>=50)
    {
        return -100 - angle1+angle2;
    }
    return angle1-angle2;
}

SkScalar TM_TimeDial::AngularMod(SkScalar angle1, SkScalar mod)
{
    if(angle1<0)
        return angle1+mod;
    return std::fmod(angle1,mod);
}

SkScalar TM_TimeDial::NormalizeAngle(SkScalar angle)
{
    if(std::fabs(angle) >= 50)
    {
        if(angle < 0)
            return angle + 100;
        return angle - 100; 
    }
    return angle;
}

bool TM_TimeDial::PollEvents(TM_EventInput eventInput)
{
    bool should_update = false;
    eventInput.mouseX -= this->bounds.x();
    eventInput.mouseY -= this->bounds.y();
    SkScalar length = std::fmin(this->bounds.width(), this->bounds.height())/2.0f-this->dialThickness/2.0f;
    SkScalar cx = this->bounds.width()/2.0f + sin(2 * M_PI * dialProgressPercentage/100.0f)*length, cy = this->bounds.height()/2.0f - length * cos(2 * M_PI * dialProgressPercentage/100.0f); 
    SkRect rectBounds = SkRect::MakeXYWH(cx-this->dialThickness/2.0f, cy-this->dialThickness/2.0f, this->dialThickness, this->dialThickness);
    SkRRect circleBounds = SkRRect::MakeRectXY(rectBounds, this->dialThickness/2.0f, this->dialThickness/2.0f);
    if(eventInput.mouseHeld==false && this->select)
        this->select = false;

    if(select)
    {
        SkScalar dAngle = NormalizeAngle(GetAnglePercentage(eventInput)-AngularMod(lastAngle,100));
        this->dialProgressPercentage += dAngle;
        this->dialProgressPercentage = AngularMod(this->dialProgressPercentage, 200);
        this->lastAngle = GetAnglePercentage(eventInput);
        should_update = true;
    }

    if(circleBounds.contains(SkRect::MakeXYWH(eventInput.mouseX,eventInput.mouseY, 1, 1)) && eventInput.mousePressed)
    {
        this->lastAngle = GetAnglePercentage(eventInput);
        this->select = true;
        should_update = true;
    }
    
    return should_update;
}
