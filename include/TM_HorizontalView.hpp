#pragma once

#include <TM_Core.hpp>
#include <TM_ViewSetting.hpp>
#include <TM_RuntimeInfo.hpp>
#include <TM_View.hpp>

class TM_HorizontalView : public TM_View 
{
	public:
        TM_HorizontalView(SkRect bounds=SkRect::MakeEmpty(), std::vector<TM_RenderObject*> objects={}, std::vector<SkScalar> proportionTable={}, TM_ViewSetting viewSetting={colorScheme[0],colorScheme[2],colorScheme[3],0,24,5,0});
        void Render(TM_RenderInfo renderInfo) override;
	private:
};