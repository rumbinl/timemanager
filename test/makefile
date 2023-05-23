CXX=/usr/bin/g++
CMP=-std=c++20
SRC=*.cpp *.c
HDD=-I/usr/local/include/gl3w -I/usr/local/include/skia
LIB=-lskia -lglfw3 -framework Cocoa -framework IOkit
FLAG=-Wnonnull -Wdeprecated-dependencies
RUN=timeman

build:
	$(CXX) $(CMP) $(SRC) $(HDD) $(FLAG) $(LIB) -o $(RUN)

run:
	./$(RUN)
