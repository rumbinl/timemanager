#pragma once

#include <TM_DateTime.hpp>
#include <TM_RenderObject.hpp>
#include <TM_Button.hpp>
#include <TM_TextView.hpp>

#include <thread>

class TM_TimerView : public TM_RenderObject
{
    public:
        TM_TimerView(SkRect bounds=SkRect::MakeEmpty(), SkScalar dialThickness=0, void* contextPtr=NULL, std::pair<TM_Time,TM_Time> (*getProgress)(void* contextPtr)=NULL, void (*setProgress)(void* contextPtr, TM_Time time)=NULL, TM_ViewSetting viewSetting = {colorScheme[3],colorScheme[2],colorScheme[1],0,24,0,0});
        void Render(TM_RenderInfo renderInfo) override;
        void startCountdown();
        void stopCountdown();
    private:
        void* contextPtr;
        std::pair<TM_Time,TM_Time> (*getProgress)(void* contextPtr);
        void (*setProgress)(void* contextPtr, TM_Time time);
        TM_TextView* progressText;
        SkScalar dialThickness;

        std::thread countdownThread;
        bool countingDown = false;
};