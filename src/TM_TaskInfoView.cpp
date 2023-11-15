#include <TM_TaskInfoView.hpp>

TM_TaskInfoView::TM_TaskInfoView(SkRect bounds, TM_TaskManager* taskManPtr, std::pair<TM_TaskManIt,TM_TaskManIt> (*getItFunc)(TM_TaskManager* taskManPtr), TM_ViewSetting viewSetting) : TM_View(bounds, {}, {}, viewSetting)
{
    this->taskManPtr = taskManPtr;
    this->getItFunc = getItFunc;
}

void TM_TaskInfoView::addTaskInfoObject(TM_TaskManIt taskIt)
{
    this->taskInfoSectionList.push_back(new TM_TaskInfoSection(SkRect::MakeWH(0,150), taskIt, taskManPtr));
    this->addRenderObject(this->taskInfoSectionList[this->taskInfoSectionList.size()-1]);
}

void TM_TaskInfoView::Render(TM_RenderInfo renderInfo)
{
    if(getItFunc != NULL)
    {
        std::pair<TM_TaskManIt,TM_TaskManIt> itRange = getItFunc(this->taskManPtr);
        TM_TaskManIt currentIt = itRange.first;

        SkScalar yPos = this->getBounds().y()-this->yOffset;

        int count = 0;

        renderInfo.canvas->save();
        renderInfo.canvas->clipRect(this->bounds);

        while(currentIt != itRange.second && (*currentIt != NULL))
        {
            if(count>=this->renderObjects.size())
                this->addTaskInfoObject(currentIt);
            else
                this->taskInfoSectionList[count]->setTaskIt(currentIt);
            this->renderObjects[count]->setBounds(SkRect::MakeXYWH(this->bounds.x(), yPos, this->bounds.width(), this->renderObjects[count]->getBounds().height()));
            this->renderObjects[count]->Render(renderInfo);
            yPos += this->renderObjects[count]->getBounds().height() + this->viewSetting.paddingY;
            currentIt++;
            count ++;
        }
        this->srcBounds.setWH(0,yPos-this->viewSetting.paddingY);

        renderInfo.canvas->restore();
    }
}

bool TM_TaskInfoView::PollEvents(TM_EventInput eventInput)
{
    bool select = false;
    if(this->bounds.contains(eventInput.mouseX, eventInput.mouseY))
    {
  
        if(getItFunc != NULL)
        {
            std::pair<TM_TaskManIt,TM_TaskManIt> itRange = getItFunc(this->taskManPtr);
            TM_TaskManIt currentIt = itRange.first;

            SkScalar yPos = this->getBounds().y()-this->yOffset;
            int count = 0;

            while(currentIt != itRange.second && (*currentIt != NULL))
            {
                if(count>=this->taskInfoSectionList.size())
                    this->addTaskInfoObject(currentIt);
                else
                    this->taskInfoSectionList[count]->setTaskIt(currentIt);
                this->renderObjects[count]->setBounds(SkRect::MakeXYWH(this->bounds.x(), yPos, this->bounds.width(), this->renderObjects[count]->getBounds().height()));

                currentIt++;
                select+=this->renderObjects[count]->PollEvents(eventInput);

                yPos += this->renderObjects[count]->getBounds().height() + this->viewSetting.paddingY;
                
                count++;
            }
        }   
        if(eventInput.scrollY != 0)
		{
			SkScalar newY = this->yOffset+eventInput.scrollY;
			this->yOffset = fmin(fmax(0, newY),fmax(0, this->srcBounds.height()-this->bounds.height()));
		}
    }
    return select;
}

TM_ImportTaskInfoView::TM_ImportTaskInfoView(SkRect bounds, TM_TaskManager* importTaskManPtr, TM_TaskManager* mainTaskManPtr, std::pair<TM_TaskManIt,TM_TaskManIt> (*getItFunc)(TM_TaskManager* taskManPtr),  TM_ViewSetting viewSetting) : TM_TaskInfoView(bounds, importTaskManPtr, getItFunc, viewSetting)
{
    this->mainTaskManPtr = mainTaskManPtr;
}

void TM_ImportTaskInfoView::addTaskInfoObject(TM_TaskManIt taskIt)
{
    this->taskInfoSectionList.push_back(new TM_ImportTaskInfoSection(SkRect::MakeWH(0,150), taskIt, taskManPtr, mainTaskManPtr));
    this->addRenderObject(this->taskInfoSectionList[this->taskInfoSectionList.size()-1]);
}