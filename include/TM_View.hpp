#pragma once

#include <TM_Core.hpp>
#include <TM_ViewSetting.hpp>
#include <TM_RuntimeInfo.hpp>
#include <TM_RenderObject.hpp>

class TM_View : public TM_RenderObject
{
    public:
        TM_View(SkRect bounds, std::vector<TM_RenderObject*> objects, TM_ViewSetting viewSetting={colorScheme[0],colorScheme[2],colorScheme[3],0,24,10,10});
        TM_View(SkRect bounds, std::vector<SkScalar> proportionTable, std::vector<TM_RenderObject*> objects, TM_ViewSetting viewSetting={colorScheme[0],colorScheme[2],colorScheme[3],0,24,10,10});
        void Render(TM_RenderInfo renderInfo) override;
        bool PollEvents(TM_EventInput eventInput) override;
        
    protected:
		std::vector<SkScalar> proportionTable;
		bool fit = false;
        SkScalar yOffset=0.0f;
};