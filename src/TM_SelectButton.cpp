#include <TM_SelectButton.hpp>

template <typename ContextType, typename DataType> TM_SelectButton<ContextType, DataType>::TM_SelectButton(std::string text, SkRect bounds, DataType data, ContextType* context, void (*actionFunc)(ContextType* context, DataType data), bool (*isSelectedFunc)(ContextType* context, DataType data), TM_ViewSetting viewSetting):TM_Button<ContextType,DataType>(text, bounds, data, context, actionFunc, viewSetting)
{
    this->isSelectedFunc = isSelectedFunc;  
}

template <typename ContextType, typename DataType> void TM_SelectButton<ContextType, DataType>::Render(TM_RenderInfo renderInfo)
{
    this->select = isSelectedFunc(this->context, this->data);
    TM_TextView::Render(renderInfo);
}

#include <TM_TaskManager.hpp>
#include <TM_MonthView.hpp>
#include <TM_CalendarView.hpp>

template class TM_SelectButton<TM_MonthView<TM_TaskManager>, int>;
template class TM_SelectButton<TM_MonthView<TM_CalendarView>, int>;