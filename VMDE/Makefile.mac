# Mac does not support that make.rb, in a strange way..

CXX = gcc
CCLD = gcc

OBJ = libVMDE.dylib
Src = $(shell find . -name "*.cpp")
Objects = $(patsubst %.cpp, %.o, $(Src))

LIBS = $(shell pkg-config --libs glfw3 glm glew portaudio-2.0 ogg vorbisfile) -lstdc++ /usr/local/lib/libSOIL.a
LDFLAGS = -dynamiclib -O4 -framework OpenGL
CXXFLAGS = -I../lib/SOIL/include  -I../ \
	$(shell pkg-config --cflags glfw3 glm glew portaudio-2.0 vorbisfile) \
	-std=c++14 -Wall -Wextra -O3 -c -fPIC \
	-Ofast -msse4.1 -mtune=core2

all: $(OBJ)

.cpp.o: .cpp
	@echo 编译C++文件$^……
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJ): $(Objects)
	@echo 链接最终二进制
	$(CCLD) $(LIBS) $(LDFLAGS) $^ -o $(OBJ)

clean:
	rm -rf $(OBJ) $(Objects)

.PHONY: all clean
