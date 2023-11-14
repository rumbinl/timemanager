#include <TM_TaskInfoView.hpp>

TM_TaskInfoView::TM_TaskInfoView(SkRect bounds, TM_TaskManager* taskManPtr, std::pair<TM_TaskManIt,TM_TaskManIt> (*getItFunc)(TM_TaskManager* taskManPtr), TM_ViewSetting viewSetting) : TM_HorizontalView(bounds, {}, {}, viewSetting)
{
    this->taskName = new TM_Button<TM_TaskManIt>("", SkRect::MakeEmpty(), taskManPtr->getStartIt(), (void*)taskManPtr, [](void* taskManPtr, TM_TaskManIt task) {
        ((TM_TaskManager*)taskManPtr)->setCurrentTask(task);
    });
    this->startDateView = new TM_View(SkRect::MakeEmpty(), {0.5,0.5}, {&this->startTime, &this->startDate}, {colorScheme[3],colorScheme[2],colorScheme[1],0,24,0,0});
    this->endDateView = new TM_View(SkRect::MakeEmpty(), {0.5,0.5}, {&this->endTime, &this->endDate}, {colorScheme[3],colorScheme[2],colorScheme[1],0,24,0,0});
    this->addRenderObject(this->taskName);
    this->addRenderObject(this->startDateView);
    this->addRenderObject(this->endDateView);
    this->blockHeight = bounds.height();
    this->taskManPtr = taskManPtr;
    this->getItFunc = getItFunc;
}

void TM_TaskInfoView::Render(TM_RenderInfo renderInfo)
{
    if(getItFunc != NULL)
    {
        std::pair<TM_TaskManIt,TM_TaskManIt> itRange = getItFunc(this->taskManPtr);
        TM_TaskManIt currentIt = itRange.first;

        SkScalar yPos = this->getBounds().y()-this->yOffset;
        SkScalar y = yPos+this->yOffset;
        SkScalar height = this->bounds.height();

        int count = 0;

        renderInfo.canvas->save();
        renderInfo.canvas->clipRect(this->bounds);

        while(currentIt != itRange.second && (*currentIt != NULL))
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
        this->srcBounds.setWH(0,(this->blockHeight)*count + this->viewSetting.paddingY*(count-1));
        this->setBounds(SkRect::MakeXYWH(this->bounds.x(), y, this->bounds.width(), height));

        renderInfo.canvas->restore();
    }
}

bool TM_TaskInfoView::PollEvents(TM_EventInput eventInput)
{
    bool select = false;
    SkScalar height = this->bounds.height();
    SkScalar y = this->bounds.y();
    if(this->bounds.contains(eventInput.mouseX, eventInput.mouseY))
    {
        if(eventInput.scrollY != 0)
		{
			SkScalar newY = this->yOffset+eventInput.scrollY;
			this->yOffset = fmin(fmax(0, newY),fmax(0, this->srcBounds.height()-this->bounds.height()));
		}
        if(getItFunc != NULL)
        {
            std::pair<TM_TaskManIt,TM_TaskManIt> itRange = getItFunc(this->taskManPtr);
            TM_TaskManIt currentIt = itRange.first;

            SkScalar yPos = this->getBounds().y();

            while(currentIt != itRange.second && (*currentIt != NULL))
            {
                this->taskName->setData(currentIt);
                this->setBounds(SkRect::MakeXYWH(this->bounds.x(), yPos, this->bounds.width(), blockHeight));

                bool res = TM_HorizontalView::PollEvents(eventInput);
                select+=res;

                yPos += this->blockHeight + this->viewSetting.paddingY;
                
                currentIt++;
            }
        }   
    }
    this->setBounds(SkRect::MakeXYWH(this->bounds.x(), y, this->bounds.width(), height));
    return select;
}