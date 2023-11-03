#include <TM_View.hpp>

TM_View::TM_View(SkRect bounds, std::vector<TM_RenderObject*> objects, TM_ViewSetting viewSetting) : TM_RenderObject(bounds, viewSetting)
{
    this->renderObjects = objects;
	for(TM_RenderObject* renderObject : this->renderObjects)
		this->numExists += renderObject->exists();
    this->viewSetting = viewSetting;
}

TM_View::TM_View(SkRect bounds, std::vector<SkScalar> proportionTable, std::vector<TM_RenderObject*> objects, TM_ViewSetting viewSetting) : TM_View(bounds, objects, viewSetting)
{
	this->fit = true;
	this->proportionTable = proportionTable;	
}

void TM_View::Render(TM_RenderInfo renderInfo)
{
    SkPaint paint;
    paint.setColor(this->viewSetting.backgroundColor);
    paint.setStyle(SkPaint::kFill_Style);

    renderInfo.canvas->drawRect(this->bounds, paint);

	if(this->viewSetting.borderThickness)
	{
		paint.setColor(this->viewSetting.borderColor);
		paint.setStyle(SkPaint::kStroke_Style);
		renderInfo.canvas->drawRect(this->bounds, paint);
	}

    int restore = renderInfo.canvas->save();

    //renderInfo.canvas->clipRect(SkRect::MakeXYWH(this->bounds.x()-1,this->bounds.y()-1,this->bounds.width()+1,this->bounds.height()+1));

	renderInfo.canvas->translate(this->bounds.x(), this->bounds.y()-this->yOffset);

    SkScalar y = 0;
	
	SkScalar height = this->bounds.height() - this->viewSetting.paddingY*(this->renderObjects.size()-1);

    for(int i=0;i<renderObjects.size();i++)
    {
        if(!this->renderObjects[i]->exists())
            continue; 
		
		if(this->fit)
		{
			if(!this->proportionTable.size())
				this->renderObjects[i]->setBounds(
					SkRect::MakeXYWH(
						0, 
						y, 	
						this->bounds.width(),
						height/(SkScalar)getNumExists()
					)
				);
			else
				this->renderObjects[i]->setBounds(
					SkRect::MakeXYWH(
						0, 
						y, 	
						this->bounds.width(),
						this->proportionTable[i] * height 
					)
				);
		}
		else
			this->renderObjects[i]->setBounds(SkRect::MakeXYWH(0, y, this->bounds.width(), renderObjects[i]->getBounds().height()));

		this->renderObjects[i]->Render(renderInfo);

		y += this->renderObjects[i]->getBounds().height() + this->viewSetting.paddingY;
    }

    this->srcBounds.setWH(this->bounds.width(), y - this->viewSetting.paddingY);

    renderInfo.canvas->restoreToCount(restore);
}


bool TM_View::PollEvents(TM_EventInput eventInput)
{
    if(this->bounds.contains(eventInput.mouseX,eventInput.mouseY))
    {
		this->select = true;
        eventInput.mouseX -= this->bounds.x();
        eventInput.mouseY -= this->bounds.y() - this->yOffset;

		bool ret = false;
		std::vector<bool> existTable(this->renderObjects.size());
		for(int i=0;i<this->renderObjects.size();i++) existTable[i] = this->renderObjects[i]->exists();
		SkScalar y = this->viewSetting.paddingY;
        for(int i=0;i<this->renderObjects.size();i++)
            if(existTable[i])
				ret += (int)this->renderObjects[i]->PollEvents(eventInput);

		if(eventInput.scrollY != 0)
		{
			SkScalar newY = this->yOffset+eventInput.scrollY;
			this->yOffset = fmin(fmax(0, newY),fmax(0, this->srcBounds.height()-this->bounds.height()));
		}
		return ret;
    }
	else if(this->select)
	{
		eventInput.mouseX -= this->bounds.x();
        eventInput.mouseY -= this->bounds.y() - this->yOffset;
		std::vector<bool> existTable(this->renderObjects.size());
		for(int i=0;i<this->renderObjects.size();i++) existTable[i] = this->renderObjects[i]->exists();
        for(int i=0;i<this->renderObjects.size();i++)
            if(existTable[i])
				this->renderObjects[i]->PollEvents(eventInput);
		this->select = false;
		return true;
	}
    return false;
}
