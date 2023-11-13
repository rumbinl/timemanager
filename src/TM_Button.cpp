#include <TM_Button.hpp>

template <typename ContextType, typename DataType> TM_Button<ContextType,DataType>::TM_Button(std::string text, SkRect bounds, DataType data, ContextType* context, void (*actionFunc)(ContextType* context, DataType data), TM_ViewSetting viewSetting) : TM_TextView(text, bounds, viewSetting)
{
    this->actionFunc = actionFunc;
    this->context = context;
    this->data = data;
}

template <typename ContextType, typename DataType> bool TM_Button<ContextType,DataType>::PollEvents(TM_EventInput eventInput)
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

template <typename ContextType, typename DataType> void TM_Button<ContextType,DataType>::setData(DataType data)
{
    this->data = data;
}

#include <TM_View.hpp>
#include <TM_CalendarWeekView.hpp>
#include <TM_CalendarView.hpp>
#include <TM_MonthView.hpp>
#include <TM_TaskManager.hpp>
#include <TM_TaskView.hpp>
#include <TM_FileDrop.hpp>

template class TM_Button<TM_View, int>;
template class TM_Button<TM_View*, int>;
template class TM_Button<TM_FileDrop, int>;
template class TM_Button<TM_TaskView, int>; 
template class TM_Button<TM_CalendarView, int>;
template class TM_Button<TM_CalendarWeekView, int>;
template class TM_Button<TM_TaskManager, TM_TaskManIt>;
template class TM_Button<TM_MonthView<TM_TaskManager>, int>;
template class TM_Button<TM_MonthView<TM_CalendarView>, int>;