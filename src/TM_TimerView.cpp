#include <TM_TimerView.hpp>

TM_TimerView::TM_TimerView(SkRect bounds, TM_ApplicationManager* appManPtr, SkScalar dialThickness, void* contextPtr, std::pair<TM_Time,TM_Time> (*getProgress)(void* contextPtr), void (*setProgress)(void* contextPtr, TM_Time time), TM_ViewSetting viewSetting) : TM_RenderObject(bounds, viewSetting)
{
    this->contextPtr = contextPtr;
    this->getProgress = getProgress;
    this->setProgress = setProgress;
    this->dialThickness = dialThickness;
    this->progressText = new TM_TextView("00.00.00", SkRect::MakeEmpty());
    this->appManPtr = appManPtr;
}

void TM_TimerView::Render(TM_RenderInfo renderInfo)
{
    renderInfo.canvas->save();
    renderInfo.canvas->clipRect(this->bounds);
	renderInfo.canvas->translate(this->bounds.x(), this->bounds.y());

    SkScalar radius = std::fmin(this->bounds.width(), this->bounds.height())/2.0f - this->dialThickness/2.0f;

    std::pair<TM_Time,TM_Time> result = {{0,0,1},{0,0,1}}; 

    if(this->getProgress != NULL)
        result = this->getProgress(this->contextPtr);
    
    SkScalar proportion = result.second.getSeconds() != 0 ? (SkScalar)result.first.getSeconds()/(SkScalar)result.second.getSeconds() : 0;

    SkScalar cx = this->bounds.width()/2.0f,cy = this->bounds.height()/2.0f; ;

    SkPaint paint;
    paint.setAntiAlias(true);
    paint.setStyle(SkPaint::kStroke_Style);
    paint.setStrokeWidth(this->dialThickness);
    paint.setColor(this->viewSetting.backgroundColor);

    renderInfo.canvas->drawCircle(cx,cy, radius, paint);

    paint.setColor(this->viewSetting.textColor);

    SkRect arcTemplate = SkRect::MakeXYWH(cx - radius, cy - radius, 2*radius, 2*radius);
    renderInfo.canvas->drawArc(arcTemplate, -90, 360.0f * proportion, false, paint);

    std::string timeLabel = TM_GetTimeString(result.first);

    paint.setStyle(SkPaint::kFill_Style);
    paint.setColor(this->viewSetting.backgroundColor);

    SkRect textBounds;
    SkFontMetrics fontMetrics;

    renderInfo.textFont->setSize(this->viewSetting.fontSize*renderInfo.dpi);
    renderInfo.textFont->getMetrics(&fontMetrics);
    renderInfo.textFont->measureText(timeLabel.c_str(), timeLabel.length() * sizeof(char), SkTextEncoding::kUTF8, &textBounds);

    renderInfo.canvas->drawSimpleText(timeLabel.c_str(), sizeof(char)*timeLabel.length(), SkTextEncoding::kUTF8, cx-textBounds.width()/2, fontMetrics.fDescent+cy, *renderInfo.textFont, paint);

    renderInfo.canvas->restore();
}

void TM_TimerView::startCountdown()
{
    this->countingDown = true;
    this->countdownThread = std::thread([this]() {
        while(this->countingDown)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            if(this->setProgress!=NULL && this->countingDown)
            {
                this->setProgress(this->contextPtr, this->getProgress(this->contextPtr).first + TM_Time{0,0,1});
                this->appManPtr->setShouldRenderUpdate();
            }
        }
    });
}

void TM_TimerView::stopCountdown()
{
    this->countingDown = false;
    this->countdownThread.detach();
}

bool TM_TimerView::isCountingDown()
{
    return this->countingDown;
}