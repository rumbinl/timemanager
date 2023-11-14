#include <TM_FileDrop.hpp>

TM_FileDrop::TM_FileDrop(std::string text, SkRect bounds, void* outContextPtr, void (*outputFunc)(void* outContextPtr, std::string filePath), TM_ViewSetting viewSetting) : TM_TextView(text, bounds, viewSetting)
{
    this->outContextPtr = outContextPtr;
    this->outputFunc = outputFunc;
}

bool TM_FileDrop::PollEvents(TM_EventInput eventInput)
{
    if(this->bounds.contains(eventInput.mouseX, eventInput.mouseY))
    {
        if(eventInput.fileDropped)
        {
            this->setText(eventInput.fileName);
            if(outContextPtr!=NULL && outputFunc != NULL)
                this->outputFunc(outContextPtr, eventInput.fileName);
            return true;
        }
    }
    return false;
}
