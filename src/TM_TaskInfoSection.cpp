#include <TM_TaskInfoSection.hpp>

TM_TaskInfoSection::TM_TaskInfoSection(SkRect bounds, TM_TaskManIt taskIt, TM_TaskManager* taskManPtr, TM_ViewSetting viewSetting) : TM_HorizontalView(bounds, {}, {}, viewSetting)
{
    this->taskName = new TM_Button<TM_TaskManIt>("", SkRect::MakeEmpty(), taskIt, (void*)taskManPtr, [](void* taskManPtr, TM_TaskManIt task) {
        ((TM_TaskManager*)taskManPtr)->setCurrentTask(task);
    }, {colorScheme[1], colorScheme[2], colorScheme[3], 0, 24, 0, 0}, false);
    this->startDateView = new TM_View(SkRect::MakeEmpty(), {0.5,0.5}, {&this->startTime, &this->startDate}, {colorScheme[3],colorScheme[2],colorScheme[1],0,24,0,0});
    this->endDateView = new TM_View(SkRect::MakeEmpty(), {0.5,0.5}, {&this->endTime, &this->endDate}, {colorScheme[3],colorScheme[2],colorScheme[1],0,24,0,0});
    this->addRenderObject(taskName);
    this->addRenderObject(this->startDateView);
    this->addRenderObject(this->endDateView);
    this->setTaskIt(taskIt);
}

void TM_TaskInfoSection::setTaskIt(TM_TaskManIt taskIt)
{
    if(*taskIt != NULL)
    {
        this->taskName->setData(taskIt);
        this->taskName->setText((*taskIt)->getName());
        this->startDate.setText(TM_GetDateString((*taskIt)->getStartDate()));
        this->startTime.setText(TM_GetTimeString((*taskIt)->getStartTime()));
        this->endDate.setText(TM_GetDateString((*taskIt)->getEndDate()));
        this->endTime.setText(TM_GetTimeString((*taskIt)->getEndTime()));
    }
}