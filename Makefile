CXX=/usr/bin/g++
VERSION=-std=c++20
SRC=src/*.cpp src/*.c
HEADERS=-I/usr/local/include/gl3w -I/usr/local/include/skia -I./include
LIB=-lskia -lSDL3 -framework Cocoa
RUN=timeman
CFLAGS = $(LIB) $(VERSION) $(HEADERS)
OBJECTS = gl3w.o TM_ApplicationManager.o TM_CalendarView.o TM_CalendarWeekView.o TM_DateTime.o TM_HorizontalView.o TM_RenderObject.o TM_Task.o TM_TaskManager.o TM_TaskView.o TM_TextBox.o TM_TextView.o TM_View.o TM_Window.o

link:
	$(CXX) $(VERSION) $(LIB) $(HEADERS) src/main.cpp $(OBJECTS) -o $(RUN)

all: $(OBJECTS) link

TM_ApplicationManager.o : TM_Window.o TM_CalendarView.o TM_TaskView.o
	$(CXX) $(CFLAGS) src/TM_ApplicationManager.cpp TM_Window.o TM_CalendarView.o TM_TaskView.o -c

gl3w.o :
	$(CXX) $(HEADERS) src/gl3w.c -c

TM_CalendarWeekView.o : TM_RenderObject.o TM_Task.o TM_TaskManager.o TM_TextView.o
	$(CXX) $(CFLAGS) src/TM_CalendarWeekView.cpp TM_RenderObject.o TM_Task.o TM_TaskManager.o TM_TextView.o -c

TM_CalendarView.o : TM_View.o TM_Task.o TM_TaskManager.o TM_Button.o TM_HorizontalView.o TM_CalendarWeekView.o 
	$(CXX) $(CFLAGS) src/TM_CalendarView.cpp TM_View.o TM_Task.o TM_TaskManager.o TM_HorizontalView.o TM_CalendarWeekView.o -c

TM_Button.o : TM_TextView.o 
	$(CXX) $(CFLAGS) include/TM_Button.hpp TM_TextView.o -c

TM_DateTime.o : 
	$(CXX) $(CFLAGS) src/TM_DateTime.cpp -c

TM_Task.o :
	$(CXX) $(CFLAGS) src/TM_Task.cpp -c

TM_Window.o :
	$(CXX) $(CFLAGS) src/TM_Window.cpp -c

TM_View.o : TM_RenderObject.o
	$(CXX) $(CFLAGS) src/TM_View.cpp TM_RenderObject.o -c

TM_RenderObject.o :
	$(CXX) $(CFLAGS) src/TM_RenderObject.cpp -c

TM_TextView.o : TM_RenderObject.o
	$(CXX) $(CFLAGS) src/TM_TextView.cpp TM_RenderObject.o -c

TM_TextBox.o : TM_TextView.o
	$(CXX) $(CFLAGS) src/TM_TextBox.cpp TM_TextView.o -c

TM_TaskView.o : TM_TextView.o
	$(CXX) $(CFLAGS) src/TM_TaskView.cpp TM_TextView.o -c

TM_TaskManager.o : TM_Task.o
	$(CXX) $(CFLAGS) src/TM_TaskManager.cpp TM_Task.o -c

TM_HorizontalView.o : TM_View.o 
	$(CXX) $(CFLAGS) src/TM_HorizontalView.cpp TM_View.o -c

run:
	./$(RUN)