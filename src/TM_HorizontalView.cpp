#include <TM_HorizontalView.hpp>

TM_HorizontalView::TM_HorizontalView(SkRect bounds, std::vector<TM_RenderObject*> objects, std::vector<SkScalar> proportionTable, TM_ViewSetting viewSetting) : TM_View(bounds, proportionTable, objects, viewSetting)
{
}

void TM_HorizontalView::Render(TM_RenderInfo renderInfo) 
{
	SkScalar objectWidth = (this->bounds.width()-this->viewSetting.paddingX * (this->getNumExists()-1))/(SkScalar)this->getNumExists(),
			 objectHeight = this->bounds.height() - 2 * this->viewSetting.paddingY;

	if(this->viewSetting.borderThickness>0)
	{
		SkPaint p;
		p.setStrokeWidth(this->viewSetting.borderThickness-1);
		p.setStyle(SkPaint::kStroke_Style);
		p.setColor(this->viewSetting.borderColor);
		renderInfo.canvas->drawRect(this->bounds, p);
	}
	renderInfo.canvas->save();
	//renderInfo.canvas->clipRect(this->bounds);
	renderInfo.canvas->translate(this->bounds.x(), this->bounds.y());

	SkScalar x = 0, i=0;

	for(TM_RenderObject* renderObject : this->renderObjects)
	{
		if(!renderObject->exists()) continue;
		SkScalar width;
		if(fit && !proportionTable.size())
		{
			width = (this->bounds.width()-this->viewSetting.paddingX * (this->getNumExists()-1))/(SkScalar)this->getNumExists();
		}
		else if(fit)
		{
			width = (this->bounds.width()-this->viewSetting.paddingX * (this->getNumExists()-1)) * proportionTable[i];
		}
		renderObject->setBounds(SkRect::MakeXYWH(x,this->viewSetting.paddingY,width,objectHeight));
		renderObject->Render(renderInfo);
		x+=width+this->viewSetting.paddingX;
		i++;
	}
	renderInfo.canvas->restore();
}

