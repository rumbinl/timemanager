#pragma once

#include <TM_Core.hpp>
#include <TM_ViewSetting.hpp>
#include <TM_RuntimeInfo.hpp>

class TM_RenderObject 
{
    public:
        TM_RenderObject(SkRect bounds, TM_ViewSetting viewSetting={colorScheme[0],colorScheme[3],colorScheme[3],0,12,5,5});
        virtual void Render(TM_RenderInfo renderInfo);
        virtual bool PollEvents(TM_EventInput eventInput);
        virtual SkRect getBounds();
        virtual void setBounds(SkRect newBounds);
        SkRect getSrcBounds();
        void setSrcBounds(SkRect srcBounds);
        bool exists();
        void setExistence(bool existence);
		void setRenderObjectExistence(int index, bool existence);
        bool getRenderObjectExistence(int index);
		void addRenderObject(TM_RenderObject* renderObject);
        SkRect getMaxBounds();
        void setMaxBounds(SkRect maxBounds);
		TM_RenderObject* getRenderObject(int index);
		int getNumExists();
    protected:
        SkRect bounds, srcBounds, maxBounds;
        TM_ViewSetting viewSetting;
        std::vector<TM_RenderObject*> renderObjects;
		int numExists=0;
        bool existence=true,select=false;
};
