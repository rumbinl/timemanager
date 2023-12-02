#include <TM_HorizontalView.hpp>

TM_HorizontalView::TM_HorizontalView(SkRect bounds, std::vector<TM_RenderObject*> objects, std::vector<SkScalar> proportionTable, TM_ViewSetting viewSetting) : TM_View(bounds, proportionTable, objects, viewSetting)
{
}

void TM_HorizontalView::Render(TM_RenderInfo renderInfo) 
{
	SkScalar contentWidth = (this->bounds.width()-this->viewSetting.paddingX * (this->getNumExists()-1)),
			 objectWidth = contentWidth/(SkScalar)this->getNumExists(),
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
	if(this->viewSetting.cornerRadius>0)
		renderInfo.canvas->clipRRect(SkRRect::MakeRectXY(this->bounds, this->viewSetting.cornerRadius, this->viewSetting.cornerRadius), true);
	renderInfo.canvas->translate(this->bounds.x(), this->bounds.y());

	SkScalar x = 0, i=0;
	int nonFitCount = 0;

	if(this->fit)
	{
		for(int i=0;i<renderObjects.size();i++)
		{
			if(renderObjects[i]->exists())
			{
				if(renderObjects[i]->getMaxBounds().width()>0)
					nonFitCount++;
				contentWidth -= renderObjects[i]->getMaxBounds().width();
			}
		}
	}

	for(TM_RenderObject* renderObject : this->renderObjects)
	{
		if(!renderObject->exists()) { i++; continue;}
		SkScalar width;
		if(fit && !proportionTable.size())
		{
			width = contentWidth / (this->getNumExists() - (SkScalar)nonFitCount);
		}
		else if(fit)
		{
			width = contentWidth * proportionTable[i];
		}
		if(renderObject->getMaxBounds().width() > 0)
			width = renderObject->getMaxBounds().width();
		renderObject->setBounds(SkRect::MakeXYWH(x,this->viewSetting.paddingY,width,objectHeight));
		renderObject->Render(renderInfo);
		x+=width+this->viewSetting.paddingX;
		i++;
	}
	renderInfo.canvas->restore();
}

