#include <TM_SelectButton.hpp>

template <typename DataType> TM_SelectButton<DataType>::TM_SelectButton(std::string text, SkRect bounds, DataType data, void* context, void (*actionFunc)(void* context, DataType data, TM_Button<DataType>* self), bool (*isSelectedFunc)(void* context, DataType data), TM_ViewSetting viewSetting):TM_Button<DataType>(text, bounds, data, context, actionFunc, viewSetting)
{
    this->isSelectedFunc = isSelectedFunc;  
}

template <typename DataType> void TM_SelectButton<DataType>::Render(TM_RenderInfo renderInfo)
{
    this->select = isSelectedFunc(this->context, this->data);
    TM_TextView::Render(renderInfo);
}

template class TM_SelectButton<int>;
template class TM_SelectButton<bool>;