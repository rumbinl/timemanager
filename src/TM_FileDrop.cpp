#include <TM_FileDrop.hpp>

TM_FileDrop::TM_FileDrop(std::string text, SkRect bounds, void* outContextPtr, void (*outputFunc)(void* outContextPtr, std::string filePath), TM_ViewSetting viewSetting) : TM_TextView(text, bounds, viewSetting)
{

}

bool TM_FileDrop::PollEvents(TM_EventInput eventInput)
{
    if(this->bounds.contains(eventInput.mouseX, eventInput.mouseY))
    {
        if(eventInput.fileDropped)
        {
            this->setText(eventInput.fileName);
            return true;
        }
    }
    return false;
}
