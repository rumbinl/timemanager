#pragma once

#include <TM_TextView.hpp>

class TM_TextBox : public TM_TextView
{
    public:
        TM_TextBox(SkRect bounds, std::string placeholder, std::string* srcString=NULL, TM_ViewSetting viewSetting ={colorScheme[1],colorScheme[2],colorScheme[3],1,24,5,5});
        void Render(TM_RenderInfo renderInfo) override;
        bool PollEvents(TM_EventInput eventInput) override;
        void setSrcString(std::string* srcString);
		SkScalar getCharWidth(char a, SkFont* font);
    private:
		void locatePosition(SkScalar mouseX, std::string text, SkFont* font);
        std::string placeholder, content="", *srcString;
        bool fitted = false;
		SkScalar cursorX=0.0f;
        int cursorIndex=0;
};