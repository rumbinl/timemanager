#pragma once

#include <TM_Core.hpp>
#include <TM_TextView.hpp>

template<class T> class TM_Button : public TM_TextView 
{
    public:
        TM_Button(std::string text, SkRect bounds, void (*actionFunc)(T* context)=NULL, T* context=NULL) : TM_TextView(text, bounds)
        {
            this->actionFunc = actionFunc;
            this->context = context;
        }

        bool PollEvents(TM_EventInput eventInput) override
        {
            if(this->bounds.contains(eventInput.mouseX, eventInput.mouseY))
            {
				if(eventInput.mousePressed)
					if(actionFunc != NULL)
						(*actionFunc)(this->context);
				this->select=true;
				return true;
            }
			if(this->select)
			{
				this->select = false;
				return true;
			}
			
            return false;
        }
    private:
        T* context;
        void (*actionFunc)(T* context);
};