CXX=/usr/bin/g++
VERSION=-std=c++20
SRC=src/*.cpp src/*.c
HEADERS=-I/usr/local/include/gl3w -I/usr/local/include/skia -I./include
LIB=-lskia -lSDL3 -framework Cocoa
RUN=timeman
CFLAGS = $(VERSION) $(HEADERS)
OBJECTS = TM_ApplicationManager.o TM_CalendarView.o TM_CalendarWeekView.o TM_DateTime.o TM_HorizontalView.o TM_RenderObject.o TM_Task.o TM_TaskManager.o TM_TaskView.o TM_TextBox.o TM_TextView.o TM_View.o TM_Window.o

TM_Task.o : src/TM_Task.cpp include/TM_Task.hpp include/TM_Core.hpp
	$(CXX) $(CFLAGS) src/TM_Task.cpp -c -o TM_Task.o

TM_Window.o : src/TM_Window.cpp include/TM_Window.hpp include/TM_Core.hpp
	$(CXX) $(CFLAGS) src/TM_Window.cpp -c -o TM_Window.o

TM_View.o : src/TM_View.cpp TM_RenderObject.o include/TM_ViewSetting.hpp include/TM_RuntimeInfo.hpp include/TM_RenderObject.hpp include/TM_Core.hpp
	$(CXX) $(CFLAGS) src/TM_View.cpp TM_RenderObject.o -c -o TM_View.o

TM_RenderObject.o : src/TM_RenderObject.cpp include/TM_RenderObject.hpp include/TM_Core.hpp include/TM_ViewSetting.hpp include/TM_RuntimeInfo.hpp
	$(CXX) $(CFLAGS) src/TM_RenderObject.cpp -c -o TM_RenderObject.o

TM_TextView.o : src/TM_TextView.cpp TM_RenderObject.o include/TM_TextView.hpp include/TM_Core.hpp include/TM_RuntimeInfo.hpp
	$(CXX) $(CFLAGS) src/TM_TextView.cpp TM_RenderObject.o -c -o TM_TextView.o

TM_TextBox.o : src/TM_TextBox.cpp TM_TextView.o
	$(CXX) $(CFLAGS) src/TM_TextBox.cpp TM_TextView.o -c -o TM_TextBox.o

TM_TaskView.o : src/TM_TextBox.cpp TM_TextView.o
	$(CXX) $(CFLAGS) src/TM_TextBox.cpp TM_TextView.o -c -o TM_TextBox.o

run:
	./$(RUN)
