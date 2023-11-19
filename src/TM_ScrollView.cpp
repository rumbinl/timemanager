#include <TM_ScrollView.hpp>

TM_ScrollView::TM_ScrollView(SkRect bounds, void* scrollContext, std::vector<SkScalar> (*getScrollFunc)(void* scrollContext), void (*setScrollFunc)(void* scrollContext, SkScalar scroll), TM_ViewSetting viewSetting) : TM_RenderObject(bounds, viewSetting)
{
    this->scrollContext = scrollContext;
    this->setScrollFunc = setScrollFunc;
    this->getScrollFunc = getScrollFunc;
}

void TM_ScrollView::Render(TM_RenderInfo renderInfo)
{
    SkPaint paint;
    paint.setColor(this->viewSetting.backgroundColor);
    renderInfo.canvas->drawRect(this->bounds, paint);
    paint.setColor(this->viewSetting.textColor);
    std::vector<SkScalar> scrollData = {0,0};
    if(this->getScrollFunc != NULL)
    {
        scrollData = this->getScrollFunc(this->scrollContext);
    }
    renderInfo.canvas->drawRect(SkRect::MakeXYWH(this->bounds.x(), this->bounds.y() + scrollData[0]*(this->bounds.height()*(1-scrollData[1])), this->bounds.width(), this->bounds.height() * scrollData[1]),paint);
}

bool TM_ScrollView::PollEvents(TM_EventInput eventInput)
{
    bool shouldUpdate = false;
    SkScalar scroll = this->getScrollFunc?this->getScrollFunc(this->scrollContext)[0]:0;
    if(this->bounds.contains(eventInput.mouseX, eventInput.mouseY))
    {
        if(eventInput.mousePressed || eventInput.mouseHeld)
        {
            if(lastMouseY < 0)
            {
                lastMouseY = eventInput.mouseY;
                this->select = true;
            }
            this->setScrollFunc(this->scrollContext, (eventInput.mouseY - lastMouseY)/this->bounds.height());
            lastMouseY = eventInput.mouseY;
        }
        else
        {
            select = false;
            this->lastMouseY = -1;
        }
        shouldUpdate = true;
    }
    else
    {
        this->select = false;
        this->lastMouseY = -1;
    }
    return shouldUpdate;
}