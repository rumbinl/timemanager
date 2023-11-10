#pragma once

#include <TM_TextView.hpp>

template <typename ContextType> class TM_TextBox : public TM_TextView
{
    public:
        TM_TextBox(SkRect bounds=SkRect::MakeEmpty(), std::string placeholder="", ContextType* contextPtr=NULL, std::string (*getStringFunc)(ContextType* contextPtr)=NULL, void (*setStringFunc)(ContextType* contextPtr, std::string newString)=NULL, TM_ViewSetting viewSetting ={colorScheme[1],colorScheme[2],colorScheme[3],0,24,5,5});
        void Render(TM_RenderInfo renderInfo) override;
        bool PollEvents(TM_EventInput eventInput) override;
		SkScalar getCharWidth(char a, SkFont* font);
    private:
		void locatePosition(SkScalar mouseX, std::string text, SkFont* font);
        std::string placeholder, content="";
        ContextType* contextPtr;
        std::string (*getStringFunc)(ContextType* contextPtr);
        void (*setStringFunc)(ContextType* contextPtr, std::string newString);
        bool fitted = false;
		SkScalar cursorX=0.0f;
        int cursorIndex=0;
};