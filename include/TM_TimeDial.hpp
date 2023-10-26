#pragma once

#include <TM_RenderObject.hpp>
#include <TM_RuntimeInfo.hpp>
#include <TM_ViewSetting.hpp>
#include <TM_DateTime.hpp>
#include <TM_TextView.hpp>

class TM_TimeDial : public TM_RenderObject
{
    public:
        TM_TimeDial(SkRect bounds, TM_ViewSetting viewSetting = {colorScheme[3],colorScheme[2],colorScheme[1],1,32,0,0});
        void Render(TM_RenderInfo renderInfo);
        SkScalar GetAnglePercentage(TM_EventInput eventInput);
        static SkScalar SubtractAnglePercentages(SkScalar angle1, SkScalar angle2);
        static SkScalar NormalizeAngle(SkScalar angle);
        static SkScalar AngularMod(SkScalar angle1, SkScalar mod);
        void SetTimePtr(TM_Time* time);
        TM_Time GetTime();
        bool PollEvents(TM_EventInput eventInput);
    private:
        TM_TextView* timeLabel;
        SkScalar dialThickness = 50, dialProgressPercentage = 0, selectAngleOffset = 0, lastAngle = 0;
};