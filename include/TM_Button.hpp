#pragma once

#include <TM_Core.hpp>
#include <TM_TextView.hpp>

template<typename DataType> class TM_Button : public TM_TextView 
{
    public:
        TM_Button(std::string text="", SkRect bounds=SkRect::MakeEmpty(), DataType data=0, void* context=NULL, void (*actionFunc)(void* context, DataType data)=NULL, TM_ViewSetting viewSetting={colorScheme[1],colorScheme[2],colorScheme[3],0,24,5,5}, bool centered = true);

        bool PollEvents(TM_EventInput eventInput) override;

        void setData(DataType data);
    protected:
        void* context;

        DataType data;

        void (*actionFunc)(void* context, DataType data);
};