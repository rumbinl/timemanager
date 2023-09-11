#include <TM_UI.hpp>

TM_HorizontalView::TM_HorizontalView(SkRect bounds, std::vector<TM_RenderObject*> objects, bool fit, TM_ViewSetting viewSetting) : TM_View(bounds, objects, viewSetting)
{
	this->fit = fit;	
}

void TM_HorizontalView::Render(TM_RenderInfo renderInfo) 
{
	SkScalar objectWidth = this->bounds.width()/(SkScalar)this->getNumExists();
	renderInfo.canvas->save();
	renderInfo.canvas->clipRect(this->bounds);
	renderInfo.canvas->setMatrix(SkMatrix::Translate(this->bounds.x(), this->bounds.y()));
	SkScalar x = 0;
	for(TM_RenderObject* renderObject : this->renderObjects)
	{
		if(!renderObject->exists()) continue;
		renderObject->setBounds(SkRect::MakeXYWH(x,0,objectWidth,this->bounds.height()));
		renderObject->Render(renderInfo);
		x+=objectWidth;
	}
	renderInfo.canvas->restore();
}

