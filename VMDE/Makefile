CXX = gcc
CCLD = gcc

OBJ = libVMDE.so
Src = $(shell find . -name "*.cpp")
Objects = $(patsubst %.cpp, %.o, $(Src))

LIBS = $(shell pkg-config --libs glfw3 gl glm glew portaudio-2.0 ogg) -lstdc++ -lSOIL
LDFLAGS = -shared -O4
CXXFLAGS = -I../lib/SOIL/include  -I../ \
	$(shell pkg-config --cflags glfw3 gl glm glew portaudio-2.0) \
	-std=c++14 -Wall -Wextra -O3 -c -fPIC \
	-Ofast -msse4.1 -mtune=core2

all: $(OBJ)

.cpp.o: .cpp
	@echo 编译C++文件$^……
	$(CXX) $(CXXFLAGS) $^ -o $@

libVMDE.so: $(Objects)
	@echo 链接最终二进制
	$(CCLD) $(LIBS) $(LDFLAGS) $^ -o libVMDE.so

clean:
	rm -rf $(OBJ) $(Objects)

.PHONY: all clean
