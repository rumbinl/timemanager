#include <TM_TaskInfoView.hpp>

TM_TaskInfoView::TM_TaskInfoView(TM_TaskManIt startIt, TM_TaskManIt endIt, SkRect bounds, TM_TaskManager* taskManPtr, TM_ViewSetting viewSetting) : TM_HorizontalView(bounds, {}, {}, viewSetting)
{
    this->taskName = new TM_Button<TM_TaskManager, TM_TaskManIt>("", SkRect::MakeEmpty(), startIt, taskManPtr, [](TM_TaskManager* taskManPtr, TM_TaskManIt task) {
        taskManPtr->setCurrentTask(task);
    });
    this->startDateView = new TM_View(SkRect::MakeEmpty(), {0.5,0.5}, {&this->startTime, &this->startDate}, {colorScheme[3],colorScheme[2],colorScheme[1],0,24,0,0});
    this->endDateView = new TM_View(SkRect::MakeEmpty(), {0.5,0.5}, {&this->endTime, &this->endDate}, {colorScheme[3],colorScheme[2],colorScheme[1],0,24,0,0});
    this->addRenderObject(this->taskName);
    this->addRenderObject(this->startDateView);
    this->addRenderObject(this->endDateView);
    setTaskRange(startIt, endIt);
    this->blockHeight = bounds.height();
}

void TM_TaskInfoView::setTaskRange(TM_TaskManIt startIt, TM_TaskManIt endIt)
{
    this->startIt = startIt;
    this->endIt = endIt;
}

void TM_TaskInfoView::Render(TM_RenderInfo renderInfo)
{
    if(*this->startIt != NULL)
    {
        TM_TaskManIt currentIt = this->startIt;

        SkScalar yPos = this->getBounds().y();

        int count = 0;

        while(currentIt != endIt && (*currentIt != NULL))
        {
            this->taskName->setText((*currentIt)->getName());
            this->taskName->setData(currentIt);
            this->startDate.setText(TM_GetDateString((*currentIt)->getStartDate()));
            this->startTime.setText(TM_GetTimeString((*currentIt)->getStartTime()));
            this->endDate.setText(TM_GetDateString((*currentIt)->getEndDate()));
            this->endTime.setText(TM_GetTimeString((*currentIt)->getEndTime()));

            this->setBounds(SkRect::MakeXYWH(this->bounds.x(), yPos, this->bounds.width(), blockHeight));

            TM_HorizontalView::Render(renderInfo);

            yPos += blockHeight + this->viewSetting.paddingY;
            
            currentIt++;
            count ++;
        }
        this->bounds.setXYWH(this->bounds.x(), this->bounds.y(), this->bounds.width(), (this->blockHeight)*count - this->viewSetting.paddingY*(count-1));
    }
}

bool TM_TaskInfoView::PollEvents(TM_EventInput eventInput)
{
    bool select = false;
    if(this->bounds.contains(eventInput.mouseX, eventInput.mouseY))
    {
        if(*this->startIt != NULL)
        {
            TM_TaskManIt currentIt = this->startIt;

            SkScalar yPos = this->getBounds().y();

            while(currentIt != endIt && (*currentIt != NULL))
            {
                this->taskName->setData(currentIt);
                this->setBounds(SkRect::MakeXYWH(this->bounds.x(), yPos, this->bounds.width(), blockHeight));

                select += TM_HorizontalView::PollEvents(eventInput);

                yPos += this->blockHeight + this->viewSetting.paddingY;
                
                currentIt++;
            }
        }   
    }
    return select;
}