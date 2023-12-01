#pragma once

#include <TM_Button.hpp>

template <typename DataType> class TM_SelectButton : public TM_Button <DataType>
{
    public: 
        TM_SelectButton(std::string text, SkRect bounds, DataType data, void* context, void (*actionFunc)(void* context, DataType data)=NULL, bool (*isSelectedFunc)(void* context, DataType data)=NULL, TM_ViewSetting viewSetting={colorScheme[1],colorScheme[2],colorScheme[3],0,12,5,5});

        void Render(TM_RenderInfo renderInfo) override;
    private:
        bool (*isSelectedFunc)(void* context, DataType data);

};