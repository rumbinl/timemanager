#pragma once

#include <TM_Core.hpp>
#include <TM_ViewSetting.hpp>
#include <TM_RuntimeInfo.hpp>
#include <TM_View.hpp>

class TM_HorizontalView : public TM_View 
{
	public:
        TM_HorizontalView(SkRect bounds, std::vector<TM_RenderObject*> objects, TM_ViewSetting viewSetting={colorScheme[0],colorScheme[2],colorScheme[3],1,24,10,0});
        void Render(TM_RenderInfo renderInfo) override;
	private:
		bool fit;
};