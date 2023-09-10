#include <TM_UI.hpp>

TM_View::TM_View(SkRect bounds, std::vector<TM_RenderObject*> objects, TM_ViewSetting viewSetting) : TM_RenderObject(bounds, viewSetting)
{
    this->renderObjects = objects;
	for(TM_RenderObject* renderObject : this->renderObjects)
		this->numExists += renderObject->exists();
    this->viewSetting = viewSetting;
}

void TM_View::Render(SkCanvas* skia_canvas, SkFont* font)
{
    SkPaint paint;
    paint.setColor(this->viewSetting.backgroundColor);
    paint.setStyle(SkPaint::kFill_Style);
    skia_canvas->drawRect(this->bounds, paint);
    paint.setColor(this->viewSetting.borderColor);
    paint.setStyle(SkPaint::kStroke_Style);
    skia_canvas->drawRect(this->bounds, paint);

    int restore = skia_canvas->save();
    skia_canvas->clipRect(this->bounds);
	skia_canvas->setMatrix(SkMatrix::Translate(this->bounds.x(), this->bounds.y()-this->yOffset));

    SkScalar y = this->viewSetting.padding;

    for(TM_RenderObject* renderObject : this->renderObjects)
    {
        if(!renderObject->exists())
            continue; 

        renderObject->setBounds(SkRect::MakeXYWH(this->viewSetting.padding, y, this->bounds.width()-this->viewSetting.padding*2, renderObject->getBounds().height()));
        renderObject->Render(skia_canvas, font);

        y += renderObject->getBounds().height() + this->viewSetting.padding;
    }

    this->srcBounds.setWH(this->bounds.width(), y);

    skia_canvas->restoreToCount(restore);
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
        for(int i=0;i<this->renderObjects.size();i++)
            if(existTable[i])
				ret += this->renderObjects[i]->PollEvents(eventInput);

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

int TM_View::getNumExists()
{
	return numExists;
}

void TM_View::addRenderObject(TM_RenderObject* renderObject)
{
	this->numExists += renderObject->exists();
	this->renderObjects.push_back(renderObject);
}

void TM_View::setRenderObjectExistence(int index, bool existence)
{
    if(index < this->renderObjects.size() && index>=0)
	{
		this->numExists += (int)existence-(int)this->renderObjects[index]->exists();
        this->renderObjects[index]->setExistence(existence);
	}
}

TM_View::~TM_View()
{

}
