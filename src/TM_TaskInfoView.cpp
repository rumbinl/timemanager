#include <TM_TaskInfoView.hpp>

TM_TaskInfoView::TM_TaskInfoView(SkRect bounds, TM_TaskManager* taskManPtr, std::pair<TM_TaskItIt,TM_TaskItIt> (*getItFunc)(TM_TaskManager* taskManPtr), TM_ViewSetting viewSetting) : TM_View(bounds, {}, {}, viewSetting)
{
    this->taskManPtr = taskManPtr;
    this->getItFunc = getItFunc;
}

TM_TaskInfoView::TM_TaskInfoView(SkRect bounds, TM_TaskManager* taskManPtr, TM_ViewSetting viewSetting) : TM_View(bounds, {}, {}, viewSetting)
{
    this->taskManPtr = taskManPtr;
    this->subtaskList = true;
}

void TM_TaskInfoView::setBounds(SkRect newBounds)
{
    this->bounds = newBounds;
}

SkRect TM_TaskInfoView::getBounds() 
{
    return SkRect::MakeXYWH(this->bounds.x(), this->bounds.y(), this->bounds.width(), this->bounds.height());
}

void TM_TaskInfoView::addTaskInfoObject(TM_TaskItIt taskIt)
{
    this->taskInfoSectionList.push_back(new TM_TaskInfoSection(SkRect::MakeWH(0,100), taskManPtr));
    this->addRenderObject(this->taskInfoSectionList[this->taskInfoSectionList.size()-1]);
}

void TM_TaskInfoView::Render(TM_RenderInfo renderInfo)
{
    std::pair<TM_TaskItIt,TM_TaskItIt> itRange;
    TM_SubtaskIt taskIt;
    TM_TaskItIt currentIt;
    int length=0;

    if(subtaskList && this->taskManPtr->getCurrentTask()!=NULL)
    {
        taskIt = this->taskManPtr->getCurrentTask()->getSubtaskList().begin();
        length = this->taskManPtr->getCurrentTask()->getSubtaskCount();
    }
    else if(this->getItFunc != NULL && this->taskManPtr != NULL)
    {
        itRange = getItFunc(this->taskManPtr);
        currentIt = itRange.first;
    }
    else
    {
        return;
    }

    SkScalar yPos = this->bounds.y()-this->yOffset;

    int count = 0;

    renderInfo.canvas->save();
    renderInfo.canvas->clipRect(this->getBounds());

    while((!subtaskList && currentIt != itRange.second && **currentIt != NULL) || (subtaskList && count < length))
    {
        if(this->subtaskList)
            currentIt = *(taskIt++);
        if(count>=this->renderObjects.size())
            this->addTaskInfoObject(currentIt);
        this->taskInfoSectionList[count]->setTaskIt(currentIt);
        this->renderObjects[count]->setBounds(SkRect::MakeXYWH(this->bounds.x(), yPos, this->bounds.width(), this->renderObjects[count]->getBounds().height()));
        this->renderObjects[count]->Render(renderInfo);
        yPos += this->renderObjects[count]->getBounds().height() + this->viewSetting.paddingY;
        currentIt++;
        count ++;
    }
    this->srcBounds.setWH(0,yPos-this->viewSetting.paddingY+this->yOffset-this->bounds.y());

    renderInfo.canvas->restore();
}

bool TM_TaskInfoView::PollEvents(TM_EventInput eventInput)
{
    bool select = false;
    if(this->bounds.contains(eventInput.mouseX, eventInput.mouseY))
    {
        std::pair<TM_TaskItIt,TM_TaskItIt> itRange;
        TM_SubtaskIt taskIt;
        TM_TaskItIt currentIt;
        int length=0;

        if(subtaskList)
        {
            if(this->taskManPtr->getCurrentTask())
            {
                taskIt = this->taskManPtr->getCurrentTask()->getSubtaskList().begin();
                length = this->taskManPtr->getCurrentTask()->getSubtaskCount();
            }
            else
                return false;
        }
        else if(this->getItFunc != NULL)
        {
            itRange = getItFunc(this->taskManPtr);
            currentIt = itRange.first;
        }

        SkScalar yPos = this->bounds.y()-this->yOffset;
        int count = 0;

        while((!subtaskList && currentIt != itRange.second && **currentIt != NULL) || (subtaskList && count < length))
        {
            if(this->subtaskList)
                currentIt = *(taskIt++);

            if(count>=this->taskInfoSectionList.size())
                this->addTaskInfoObject(currentIt);
            this->taskInfoSectionList[count]->setTaskIt(currentIt);

            this->renderObjects[count]->setBounds(SkRect::MakeXYWH(this->bounds.x(), yPos, this->bounds.width(), this->renderObjects[count]->getBounds().height()));

            if(!this->subtaskList)
                currentIt++;

            if(this->renderObjects[count]->PollEvents(eventInput))
            {
                select = true;
                break;
            }

            yPos += this->renderObjects[count]->getBounds().height() + this->viewSetting.paddingY;
            
            count++;
        }
        if(eventInput.scrollY != 0)
		{
			SkScalar newY = this->yOffset+eventInput.scrollY;
			this->yOffset = fmin(fmax(0, newY),fmax(0, this->srcBounds.height()-this->bounds.height()));
		}
    }
    return select;
}

TM_ImportTaskInfoView::TM_ImportTaskInfoView(SkRect bounds, TM_TaskManager* importTaskManPtr, TM_TaskManager* mainTaskManPtr, std::pair<TM_TaskItIt,TM_TaskItIt> (*getItFunc)(TM_TaskManager* taskManPtr),  TM_ViewSetting viewSetting) : TM_TaskInfoView(bounds, importTaskManPtr, getItFunc, viewSetting)
{
    this->mainTaskManPtr = mainTaskManPtr;
}

void TM_ImportTaskInfoView::addTaskInfoObject(TM_TaskItIt taskIt)
{
    this->taskInfoSectionList.push_back(new TM_ImportTaskInfoSection(SkRect::MakeWH(0,100), taskManPtr, mainTaskManPtr));
    this->addRenderObject(this->taskInfoSectionList[this->taskInfoSectionList.size()-1]);
}