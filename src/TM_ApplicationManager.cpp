#include <TM_ApplicationManager.hpp>
#include <locale>
#include <codecvt>

TM_ApplicationManager::TM_ApplicationManager()
{
	if(!this->window_ptr.Was_init_success())
	{
		std::cout<<"Window failed to initialize! Aborting..."<<std::endl;
		this->should_terminate = true;
		return;
	}

	this->skia_canvas = this->window_ptr.Get_skia_canvas_ptr();
	if(!this->skia_canvas)
	{
		std::cout<<"Failed to create Skia canvas!"<<std::endl;
	}

	this->should_render_update = true;
	this->skia_canvas_clear_color = colorScheme[BACKGROUND_COLOR_INDEX];

	this->storageManPtr = new TM_StorageManager("./TM_Stor.db");
	this->taskManPtr = new TM_TaskManager(&this->taskViewPtr, &this->storageManPtr);
	this->storageManPtr->LoadTasks(this->taskManPtr);
	this->importTaskManPtr = new TM_TaskManager(&this->taskViewPtr, NULL);
	this->importTaskInfoViewPtr = new TM_ImportTaskInfoView(SkRect::MakeEmpty(), this->importTaskManPtr, this->taskManPtr, [](TM_TaskManager* taskManager) -> std::pair<TM_TaskItIt,TM_TaskItIt> {
		return {taskManager->getStartIt(), taskManager->getEndIt()};
	});
	this->taskViewPtr = new TM_TaskView(SkRect::MakeEmpty(), this->taskManPtr);
	this->mainView = new TM_View(SkRect::MakeXYWH(0,0,this->window_ptr.getWindowWidth(),this->window_ptr.getWindowHeight()), {0, 1.00}, {
		new TM_HorizontalView(SkRect::MakeWH(0,TM_NormalHeight), {
				new TM_Button<int>("\uefe8", SkRect::MakeEmpty(), (int)0, &this->mainView, [](void* context, int data) {
					TM_RenderObject* subView = (*((TM_RenderObject**)context))->getRenderObject(1)->getRenderObject(0);
					(subView)->setRenderObjectExistence(0, true);
					(subView)->setRenderObjectExistence(1, false);
					(subView)->setRenderObjectExistence(2, false);
				}, {colorScheme[1],colorScheme[2],colorScheme[3],0,12,5,5,true}),
				new TM_Button<int>("\ue1b2", SkRect::MakeEmpty(), (int)0, &this->mainView, [](void* context, int data) {
					TM_RenderObject* subView = (*((TM_RenderObject**)context))->getRenderObject(1)->getRenderObject(0);
					(subView)->setRenderObjectExistence(0, false);
					(subView)->setRenderObjectExistence(1, true);
					(subView)->setRenderObjectExistence(2, false);
				}, {colorScheme[1],colorScheme[2],colorScheme[3],0,12,5,5,true}),
				new TM_Button<int>("\ue9fc", SkRect::MakeEmpty(), (int)0, &this->mainView, [](void* context, int data) {
					TM_RenderObject* subView = (*((TM_RenderObject**)context))->getRenderObject(1)->getRenderObject(0);
					(subView)->setRenderObjectExistence(0, false);
					(subView)->setRenderObjectExistence(1, false);
					(subView)->setRenderObjectExistence(2, true);
				}, {colorScheme[1],colorScheme[2],colorScheme[3],0,12,5,0,true})
			}),

		new TM_HorizontalView(SkRect::MakeEmpty(), {
			new TM_View(SkRect::MakeEmpty(), {}, {

				this->calendarViewPtr = new TM_CalendarView(SkRect::MakeEmpty(), this->taskManPtr),

				new TM_TaskInfoView(SkRect::MakeEmpty(), this->taskManPtr, [](TM_TaskManager* taskManPtr) -> std::pair<TM_TaskItIt,TM_TaskItIt> {
					return {taskManPtr->getStartIt(), taskManPtr->getEndIt()};
				}),

				new TM_View(SkRect::MakeEmpty(), {1.00, 0.00}, {
					this->importTaskInfoViewPtr,
					new TM_FileDrop("Place file here.", SkRect::MakeWH(0,75), this->importTaskManPtr, [](void* importTaskManPtr, std::string filePath){
						((TM_TaskManager*)importTaskManPtr)->openDocXFile(filePath);
					})
				}, {colorScheme[0],colorScheme[2],colorScheme[3],0,12,10,10})

			}, {colorScheme[0],colorScheme[2],colorScheme[3],0,12,0,0}),
			this->taskViewPtr
		})
	}, {colorScheme[1],colorScheme[2],colorScheme[3],0,12,0,0});
	TM_RenderObject* subView = this->mainView->getRenderObject(1)->getRenderObject(0);
	(subView)->setRenderObjectExistence(0, true);
	(subView)->setRenderObjectExistence(1, false);
	(subView)->setRenderObjectExistence(2, false);
}

void TM_ApplicationManager::Run()
{
    while(!this->should_terminate)
    {
        if(this->should_render_update)
        {
            this->Render();
            this->should_render_update = false;
        }
        this->PollEvents();
    }
}

void TM_ApplicationManager::Render()
{
	this->skia_canvas->resetMatrix();
	this->skia_canvas->clear(this->skia_canvas_clear_color);

	TM_RenderInfo renderInfo = {this->skia_canvas, this->skia_fontList[0], this->skia_fontList[1], this->window_ptr.getDPI()};
	this->mainView->setBounds(SkRect::MakeXYWH(0,0,(SkScalar)this->window_ptr.getWindowWidth(), this->window_ptr.getWindowHeight()));
	this->mainView->Render(renderInfo);

	this->skia_canvas->flush();
	this->window_ptr.Swap_buffers();
}

void TM_ApplicationManager::PollEvents()
{
    if(SDL_WaitEvent(&this->SDL_event_ptr))
    {
		if(this->SDL_event_ptr.type==SDL_EVENT_QUIT)
			this->should_terminate=true;
		else if(this->SDL_event_ptr.type == SDL_EVENT_WINDOW_RESIZED)
		{
			this->window_ptr.Handle_resize(&this->SDL_event_ptr);
			this->skia_canvas = this->window_ptr.Get_skia_canvas_ptr();
		}
		else
		{
			float mouseX,mouseY,scrollY=0.0f,scrollX=0.0f;

			bool pressed = SDL_event_ptr.type==SDL_EVENT_MOUSE_BUTTON_DOWN;

			std::string inputText = "";

			if(SDL_event_ptr.type == SDL_EVENT_TEXT_INPUT)
				inputText = SDL_event_ptr.text.text;

			SDL_PumpEvents(); 

			bool held = (SDL_GetMouseState(&mouseX,&mouseY)&1)>0; 

			if(this->SDL_event_ptr.type == SDL_EVENT_MOUSE_WHEEL)
				scrollY = this->SDL_event_ptr.wheel.y, scrollX = this->SDL_event_ptr.wheel.x;

			TM_EventInput eventInput = {
					mouseX*this->window_ptr.getDPI(), 
					mouseY*this->window_ptr.getDPI(), 
					scrollX*scrollSensFactor*(this->SDL_event_ptr.wheel.direction==SDL_MOUSEWHEEL_FLIPPED?-1:1),
					scrollY*scrollSensFactor*(this->SDL_event_ptr.wheel.direction==SDL_MOUSEWHEEL_FLIPPED?-1:1),
					this->window_ptr.getDPI(),
					pressed,held,
					SDL_event_ptr.type == SDL_EVENT_KEY_DOWN, 
					SDL_event_ptr.type == SDL_EVENT_DROP_FILE,
					inputText,(SDL_event_ptr.type == SDL_EVENT_DROP_FILE?SDL_event_ptr.drop.file:""),this->skia_fontList[this->defaultFont],SDL_event_ptr.key.keysym.scancode
				};

			this->mainView->PollEvents(eventInput);

			if(SDL_event_ptr.type == SDL_EVENT_DROP_FILE)
				SDL_free(SDL_event_ptr.drop.file);
		}
		this->should_render_update = true;
    }
}

void TM_ApplicationManager::LoadFont(std::string fontPath)
{
	sk_sp<SkFontMgr> fontManager(SkFontMgr::RefDefault());
	sk_sp<SkTypeface> typeface = fontManager->makeFromFile(fontPath.c_str());
//	if(typeface==NULL)
//		std::cout<<"Failed to load font from path: "<<fontPath<<std::endl;
    this->skia_fontList.push_back(new SkFont(typeface)); 
}

void TM_ApplicationManager::setDefaultFont(int newDefaultFont)
{
    if(newDefaultFont >= this->skia_fontList.size())
        this->defaultFont = 0;
    else
        this->defaultFont = newDefaultFont;
}

int TM_ApplicationManager::getDefaultFont()
{
    return this->defaultFont;
}

TM_ApplicationManager::~TM_ApplicationManager()
{
	delete this->taskManPtr;
	delete this->taskViewPtr;
	delete this->storageManPtr;
}
