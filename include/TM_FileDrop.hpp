#pragma once

#include <string>
#include <TM_TextView.hpp>

class TM_FileDrop : public TM_TextView
{
    public:
        TM_FileDrop(std::string text = "Place file here.", SkRect bounds=SkRect::MakeEmpty(), void* outContextPtr=NULL, void (*outputFunc)(void* outContextPtr, std::string filePath)=NULL, TM_ViewSetting viewSetting = {colorScheme[1],colorScheme[2],colorScheme[3],0,18,0,0,false,20});
        bool PollEvents(TM_EventInput eventInput) override;
    private:
        void* outContextPtr; 
        void (*outputFunc)(void* outContextPtr, std::string filePath);
};