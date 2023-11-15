#pragma once

#include <TM_View.hpp>
#include <TM_TextView.hpp>
#include <TM_TaskManager.hpp>
#include <TM_Button.hpp>
#include <TM_HorizontalView.hpp>
#include <TM_Task.hpp>
#include <TM_RuntimeInfo.hpp>
#include <TM_TaskInfoSection.hpp>
#include <set>

class TM_TaskInfoView : public TM_View
{
    public:
        TM_TaskInfoView(SkRect bounds = SkRect::MakeEmpty(), TM_TaskManager* taskManPtr=NULL, std::pair<TM_TaskManIt,TM_TaskManIt> (*getItFunc)(TM_TaskManager* taskManPtr)=NULL,  TM_ViewSetting viewSetting = {colorScheme[1],colorScheme[2],colorScheme[3],0,24,0,10});
        virtual void addTaskInfoObject(TM_TaskManIt taskIt);
        void Render(TM_RenderInfo renderInfo) override;
        bool PollEvents(TM_EventInput eventInput) override;
<<<<<<< HEAD
    protected:
=======
    private:
>>>>>>> 356156d1cb0065254fa2233aca710f9794f61463
        SkScalar blockHeight = 0.0f;
        TM_TaskManager* taskManPtr;
        std::pair<TM_TaskManIt,TM_TaskManIt> (*getItFunc)(TM_TaskManager* taskManPtr);
        std::vector<TM_TaskInfoSection*> taskInfoSectionList;
<<<<<<< HEAD
};

class TM_ImportTaskInfoView : public TM_TaskInfoView
{
    public:
        TM_ImportTaskInfoView(SkRect bounds = SkRect::MakeEmpty(), TM_TaskManager* importTaskManPtr=NULL, TM_TaskManager* mainTaskManPtr=NULL, std::pair<TM_TaskManIt,TM_TaskManIt> (*getItFunc)(TM_TaskManager* taskManPtr)=NULL,  TM_ViewSetting viewSetting = {colorScheme[1],colorScheme[2],colorScheme[3],0,24,0,10});
        void addTaskInfoObject(TM_TaskManIt taskIt) override;
    private:
        TM_TaskManager* mainTaskManPtr;
=======
>>>>>>> 356156d1cb0065254fa2233aca710f9794f61463
};