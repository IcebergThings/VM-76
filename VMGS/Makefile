CXX = gcc
CCLD = gcc

OBJ = VM_76
Src = $(shell find . -name "*.cpp")
Objects = $(patsubst %.cpp, %.o, $(Src))

LIBS = $(shell pkg-config --libs --static glfw3 gl glm glew portaudio-2.0)  -lstdc++ -lSOIL
LDFLAGS = -O4 ../VMDE/libVMDE.so
CXXFLAGS = -I../lib/SOIL/include  $(shell pkg-config --cflags glfw3 gl glm glew portaudio-2.0)  -I../ -std=c++14 -Wall -Wextra -O3 -c

all: $(OBJ)
run: all
	@echo 运行
	./$(OBJ)

.cpp.o: .cpp
	@echo 编译C++文件$^……
	$(CXX) $(CXXFLAGS) $^ -o $@

VM_76: $(Objects)
	@echo 链接最终二进制
	$(CCLD) $(LIBS) $(LDFLAGS) $^ -o VM_76

clean:
	rm -rf $(OBJ) $(Objects)

.PHONY: all run clean
