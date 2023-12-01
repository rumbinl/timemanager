#pragma once

#include <TM_Core.hpp>
#include <TM_ViewSetting.hpp>
#include <TM_RuntimeInfo.hpp>
#include <TM_RenderObject.hpp>
#include <TM_ScrollView.hpp>

class TM_View : public TM_RenderObject
{
    public:
        TM_View(SkRect bounds, std::vector<TM_RenderObject*> objects, bool scroll=false, TM_ViewSetting viewSetting={colorScheme[0],colorScheme[2],colorScheme[3],0,12,0,0});
        TM_View(SkRect bounds, std::vector<SkScalar> proportionTable, std::vector<TM_RenderObject*> objects, TM_ViewSetting viewSetting={colorScheme[0],colorScheme[2],colorScheme[3],0,12,0,0});
        SkScalar getYOffset();
        void setYOffset(SkScalar newYOffset);
        void Render(TM_RenderInfo renderInfo) override;
        bool PollEvents(TM_EventInput eventInput) override;
    protected:
		std::vector<SkScalar> proportionTable;
        TM_ScrollView* scrollView;
		bool fit = false, scroll = false;
        SkScalar yOffset=0.0f;
};