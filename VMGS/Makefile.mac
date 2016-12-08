# make.rb is not working on Mac

CXX = gcc
CCLD = gcc

OBJ = VMGS
Src = $(shell find . -name "*.cpp")
Objects = $(patsubst %.cpp, %.o, $(Src))

LIBS = $(shell pkg-config --libs --static glfw3 glm glew portaudio-2.0 vorbisfile)  -lstdc++
LDFLAGS = -O4 ../VMDE/libVMDE.dylib -framework OpenGL
CXXFLAGS = -I../lib/SOIL/include -I../VMDE \
	$(shell pkg-config --cflags glfw3 glm glew portaudio-2.0 vorbisfile) \
	-I../ -std=c++14 -Wall -Wextra -O3 -c \
	-Ofast -msse4.1 -mtune=core2

all: $(OBJ)
run: all
	@echo 运行
	./$(OBJ)

.cpp.o: .cpp
	@echo 编译C++文件$^……
	$(CXX) $(CXXFLAGS) $^ -o $@

VMGS: $(Objects)
	@echo 链接最终二进制
	$(CCLD) $(LIBS) $(LDFLAGS) $^ -o VMGS

clean:
	rm -rf $(OBJ) $(Objects)mcpu

.PHONY: all run clean
