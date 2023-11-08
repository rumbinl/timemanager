#pragma once

#include <TM_Core.hpp>
#include <TM_TextView.hpp>

template <typename ContextType, typename DataType> class TM_Button : public TM_TextView 
{
    public:
        TM_Button(std::string text="", SkRect bounds=SkRect::MakeEmpty(), DataType data=0, ContextType* context=NULL, void (*actionFunc)(ContextType* context, DataType data)=NULL, TM_ViewSetting viewSetting={colorScheme[1],colorScheme[2],colorScheme[3],0,24,5,5});

        bool PollEvents(TM_EventInput eventInput) override;

        void setData(DataType data);
    protected:
        ContextType* context;
        DataType data;
        void (*actionFunc)(ContextType* context, DataType data);
};