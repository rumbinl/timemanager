#include <TM_View.hpp>

TM_View::TM_View(SkRect bounds, std::vector<TM_RenderObject*> objects, bool scroll, TM_ViewSetting viewSetting) : TM_RenderObject(bounds, viewSetting)
{
    this->renderObjects = objects;
	for(TM_RenderObject* renderObject : this->renderObjects)
		this->numExists += renderObject->exists();
    this->viewSetting = viewSetting;
	this->scroll = scroll;
	this->scrollView = new TM_ScrollView(SkRect::MakeWH(20,bounds.height()), (void*)this, 
		[](void* viewPtr) -> std::vector<SkScalar> {
			TM_View* view = (TM_View*)viewPtr;
			return {view->getYOffset()/(view->getSrcBounds().height()-view->getBounds().height()), view->getBounds().height()/view->getSrcBounds().height()};
		}, 
		[](void* viewPtr, SkScalar scroll) {
			TM_View* view = (TM_View*)viewPtr;
			SkScalar scrollOffset = scroll * view->getSrcBounds().height();
			SkScalar newY = scrollOffset + view->getYOffset();
			view->setYOffset(fmin(fmax(0, newY),fmax(0, view->getSrcBounds().height()-view->getBounds().height())));
		}
	);
}

TM_View::TM_View(SkRect bounds, std::vector<SkScalar> proportionTable, std::vector<TM_RenderObject*> objects, TM_ViewSetting viewSetting) : TM_View(bounds, objects, false, viewSetting)
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

    //renderInfo.canvas->clipRect(SkRect::MakeXYWH(this->bounds.x(),this->bounds.y(),this->bounds.width(),this->bounds.height()));

	renderInfo.canvas->translate(this->bounds.x(), this->bounds.y()-this->yOffset);

    SkScalar y = this->viewSetting.paddingY;
	
	SkScalar height = this->bounds.height() - this->viewSetting.paddingY*(this->renderObjects.size());
	SkScalar contentWidth = scroll?this->bounds.width()-this->scrollView->getBounds().width():this->bounds.width();

    for(int i=0;i<renderObjects.size();i++)
    {
        if(!this->renderObjects[i]->exists())
            continue; 

		SkScalar width = this->renderObjects[i]->getMaxBounds().width() == 0.0f ? contentWidth : std::fmin(contentWidth, this->renderObjects[i]->getMaxBounds().width()); 
		SkRect tempBounds = this->renderObjects[i]->getBounds();

		if(this->fit)
		{
			if(!this->proportionTable.size())
				this->renderObjects[i]->setBounds(
					SkRect::MakeXYWH(
						std::fabs(contentWidth-width)/2.0f, 
						y, 	
						width,
						height/(SkScalar)getNumExists()
					)
				);
			else
				this->renderObjects[i]->setBounds(
					SkRect::MakeXYWH(
						std::fabs(contentWidth-width)/2.0f,
						y, 	
						width,
						this->proportionTable[i] * height 
					)
				);
		}
		else
			this->renderObjects[i]->setBounds(SkRect::MakeXYWH(std::fabs(contentWidth-width)/2.0f, y, width, renderObjects[i]->getBounds().height()));

		this->renderObjects[i]->Render(renderInfo);

		y += this->renderObjects[i]->getBounds().height() + this->viewSetting.paddingY;
    }

    this->srcBounds.setWH(this->bounds.width(), y - this->viewSetting.paddingY);

    renderInfo.canvas->restoreToCount(restore);

	if(scroll)
	{
		scrollView->setBounds(SkRect::MakeXYWH(this->bounds.x()+this->bounds.width() - scrollView->getBounds().width(), this->bounds.y(), this->scrollView->getBounds().width(), this->bounds.height()));
		scrollView->Render(renderInfo);
	}
}


bool TM_View::PollEvents(TM_EventInput eventInput)
{
	bool ret= false;
    if(this->bounds.contains(eventInput.mouseX,eventInput.mouseY))
    {
		this->select = true;

		if(this->scroll)
			ret += this->scrollView->PollEvents(eventInput);
        eventInput.mouseX -= this->bounds.x();
        eventInput.mouseY -= this->bounds.y() - this->yOffset;
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
			ret += true;
		}
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
		ret = true;
	}
    return ret;
}

SkScalar TM_View::getYOffset()
{
	return this->yOffset;
}

void TM_View::setYOffset(SkScalar newYOffset)
{
	this->yOffset = newYOffset;
}