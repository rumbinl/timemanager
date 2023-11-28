#pragma once

#include <TM_Core.hpp>
#include <TM_Task.hpp>
#include <TM_RuntimeInfo.hpp>
#include <TM_ViewSetting.hpp>
#include <TM_RenderObject.hpp>
#include <TM_TextView.hpp>
#include <TM_TaskManager.hpp>

#include <include/effects/SkGradientShader.h>

class TM_CalendarWeekView : public TM_RenderObject
{
    public:
        TM_CalendarWeekView(SkRect bounds, std::chrono::year_month_day* focusDate, TM_TaskManager* taskManPtr, int numDays = 7, SkScalar hourHeight = 50.0, TM_ViewSetting viewSettings={colorScheme[1],colorScheme[2],colorScheme[3],0,24,0,0});
        void Render(TM_RenderInfo renderInfo) override;
        void RenderTimes(TM_RenderInfo renderInfo);
        void RenderTask(TM_Task* task, TM_YMD startDate, SkColor color, TM_RenderInfo renderInfo);
        bool PollEvents(TM_EventInput eventInput) override;
        bool PollTask(TM_Task* task, TM_YMD startDate, TM_EventInput eventInput);
        bool DateRangeInView(TM_Task* task);
        TM_YMD RepeatFirstOccurence(TM_Task* task);
        TM_YMD RepeatLastOccurence(TM_Task* task);
        void setDaySpan(int daySpan);
        std::chrono::year_month_day getDateFromMouseX(TM_EventInput eventInput);
        TM_Time getTimeFromMouseY(TM_EventInput eventInput);
        int getDaySpan();
        std::chrono::year_month_day* getCurrentDatePtr();
    private:
        TM_TaskManager* taskManPtr;
        TM_Task newTask = TM_Task("", {},{},{0,0},{0,0});
        std::vector<TM_TextView> dayLabels;
        int pressWeekIndexStart = -1, pressWeekIndexEnd = -1,pressDayIndexStart = -1, pressDayIndexEnd = -1;
        int scrollY=0.0f, pressIndexStart=-1, pressIndexEnd=-1,numDays=1;
        SkScalar hourHeight,labelHeight,dayWidth,yOff,xOff=0.0f,radius=20;
        std::chrono::year_month_day* focusDate;
};