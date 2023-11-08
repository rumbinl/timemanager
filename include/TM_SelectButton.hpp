#pragma once

#include <TM_Button.hpp>

template <typename ContextType, typename DataType> class TM_SelectButton : public TM_Button <ContextType, DataType>
{
    public: 
        TM_SelectButton(std::string text, SkRect bounds, DataType data, ContextType* context=NULL, void (*actionFunc)(ContextType* context, DataType data)=NULL, bool (*isSelectedFunc)(ContextType* context, DataType data)=NULL, TM_ViewSetting viewSetting={colorScheme[1],colorScheme[2],colorScheme[3],0,24,5,5});

    private:
        bool (*isSelectedFunc)(ContextType* context, DataType data);

};