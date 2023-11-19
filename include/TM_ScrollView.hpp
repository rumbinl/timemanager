#pragma once

#include <TM_RenderObject.hpp>
#include <TM_ViewSetting.hpp>
#include <TM_RuntimeInfo.hpp>
#include <vector>

class TM_ScrollView : public TM_RenderObject
{
    public:
        TM_ScrollView(SkRect bounds, void* scrollContext=NULL, std::vector<SkScalar> (*getScrollFunc)(void* scrollContext)=NULL, void (*setScrollFunc)(void* scrollContext, SkScalar scroll)=NULL, TM_ViewSetting viewSetting={colorScheme[2],colorScheme[2],colorScheme[3],0,0,0,0});
        void Render(TM_RenderInfo renderInfo) override;
        bool PollEvents(TM_EventInput eventInput) override;
    private:
        void* scrollContext; 
        std::vector<SkScalar> (*getScrollFunc)(void* scrollContext); 
        SkScalar lastMouseY=-1;
        void (*setScrollFunc)(void* scrollContext, SkScalar scroll);
};