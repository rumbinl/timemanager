#include <TM_HorizontalView.hpp>

TM_HorizontalView::TM_HorizontalView(SkRect bounds, std::vector<TM_RenderObject*> objects, TM_ViewSetting viewSetting) : TM_View(bounds, objects, viewSetting)
{
}

void TM_HorizontalView::Render(TM_RenderInfo renderInfo) 
{
	SkScalar objectWidth = (this->bounds.width()-this->viewSetting.paddingX * (this->getNumExists()-1))/(SkScalar)this->getNumExists(),
			 objectHeight = this->bounds.height() - 2 * this->viewSetting.paddingY;

	renderInfo.canvas->save();
	//renderInfo.canvas->clipRect(this->bounds);
	renderInfo.canvas->translate(this->bounds.x(), this->bounds.y());

	SkScalar x = 0;

	for(TM_RenderObject* renderObject : this->renderObjects)
	{
		if(!renderObject->exists()) continue;
		renderObject->setBounds(SkRect::MakeXYWH(x,this->viewSetting.paddingY,objectWidth,objectHeight));
		renderObject->Render(renderInfo);
		x+=objectWidth+this->viewSetting.paddingX;
	}
	renderInfo.canvas->restore();
}

