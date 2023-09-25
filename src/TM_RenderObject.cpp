#include <TM_RenderObject.hpp>

TM_RenderObject::TM_RenderObject(SkRect bounds, TM_ViewSetting viewSetting)
{
    this->bounds = bounds;
    this->viewSetting = viewSetting;
	this->numExists = 0;
}

void TM_RenderObject::Render(TM_RenderInfo renderInfo)
{

}

bool TM_RenderObject::PollEvents(TM_EventInput eventInput)
{
    return this->bounds.contains(eventInput.mouseX,eventInput.mouseY);
}

SkRect TM_RenderObject::getBounds()
{
    return this->bounds;
}

SkRect TM_RenderObject::getSrcBounds()
{
    return this->srcBounds;
}

void TM_RenderObject::setBounds(SkRect newBounds)
{
    this->bounds = newBounds;
}

bool TM_RenderObject::exists()
{
    return this->existence;
}

void TM_RenderObject::setExistence(bool existence)
{
    this->existence = existence;
}

int TM_RenderObject::getNumExists()
{
	return numExists;
}

void TM_RenderObject::addRenderObject(TM_RenderObject* renderObject)
{
	this->numExists += renderObject->exists();
	this->renderObjects.push_back(renderObject);
}

void TM_RenderObject::setRenderObjectExistence(int index, bool existence)
{
    if(index < this->renderObjects.size() && index>=0)
	{
		this->numExists += (int)existence-(int)this->renderObjects[index]->exists();
        this->renderObjects[index]->setExistence(existence);
	}
}

TM_RenderObject* TM_RenderObject::getRenderObject(int index)
{
	return this->renderObjects[index];
}
