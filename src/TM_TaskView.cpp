#include <TM_TaskView.hpp>

TM_TaskView::TM_TaskView(SkRect bounds) : TM_View(bounds, {})
{
    this->textBox = new TM_TextBox(SkRect::MakeWH(0,40),"Untitled");
    this->taskList = new TM_View(SkRect::MakeWH(0,200), {});
    this->addSubtaskButton = new TM_Button<TM_TaskView>("Add Subtask", SkRect::MakeWH(0,50));
    this->monthView = new TM_CalendarMonthView<TM_TaskView>(SkRect::MakeWH(0,480));

    this->renderObjects.push_back(this->textBox);
    this->renderObjects.push_back(this->taskList);
    this->renderObjects.push_back(this->addSubtaskButton);
    this->renderObjects.push_back(this->monthView);
}