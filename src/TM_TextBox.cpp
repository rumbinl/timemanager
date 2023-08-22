#include <TM_Core.hpp>
#include <TM_UI.hpp>

TM_TextBox::TM_TextBox(SkScalar x, SkScalar y, SkScalar width, SkScalar height, std::string placeholder, TM_ViewSetting viewSetting) : TM_TextView("", width, height, x, y, viewSetting, false)
{
}