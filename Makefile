CXX=/usr/bin/g++
CMP=-std=c++20
SRC=src/*.cpp src/*.c
HDD=-I/usr/local/include/gl3w -I/usr/local/include/skia -I./include
LIB=-lskia -lSDL3 -framework Cocoa
FLAG=-Wnonnull -Wdeprecated-dependencies
RUN=./timeman

build:
	$(CXX) $(CMP) $(SRC) $(HDD) $(FLAG) $(LIB) -o $(RUN)

run:
	./$(RUN)
