#include <TM_Button.hpp>

template <typename DataType> TM_Button<DataType>::TM_Button(std::string text, SkRect bounds, DataType data, void* context, void (*actionFunc)(void* context, DataType data), TM_ViewSetting viewSetting, bool centered) : TM_TextView(text, bounds, viewSetting, centered)
{
    this->actionFunc = actionFunc;
    this->context = context;
    this->data = data;
}

template <typename DataType> bool TM_Button<DataType>::PollEvents(TM_EventInput eventInput)
{
    if(this->bounds.contains(eventInput.mouseX, eventInput.mouseY))
    {
        if(eventInput.mousePressed)
            if(actionFunc != NULL)
                (*actionFunc)(this->context, this->data);
        this->select=true;
        return true;
    }
    if(this->select)
    {
        this->select = false;
        return true;
    }
    
    return false;
}

template <typename DataType> void TM_Button<DataType>::setData(DataType data)
{
    this->data = data;
}

#include <TM_TaskManager.hpp>
#include <TM_Task.hpp>

template class TM_Button<int>;
template class TM_Button<TM_Task*>;
template class TM_Button<TM_TaskIt>;
template class TM_Button<TM_TaskItIt>;