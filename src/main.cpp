#include <TM_ApplicationManager.hpp>

int main()
{
	TM_ApplicationManager application_manager;
	application_manager.LoadFont("fonts/Roboto/Roboto-Regular.ttf");
	application_manager.LoadFont("fonts/Material/Material-Regular.woff2");
	application_manager.setDefaultFont(0);
	application_manager.Run();
	return 0;
}
