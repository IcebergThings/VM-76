#==============================================================================
# ■ VMDE - mac.makefile
#------------------------------------------------------------------------------
#   Mac does not support that make.rb, in a strange way...
#==============================================================================

include ../inc.makefile

OBJ = libVMDE.dylib
Src = $(shell find . -name "*.cpp")
Objects = $(patsubst %.cpp, %.o, $(Src))

LDLIBS += /usr/local/lib/libSOIL.a \
	$(shell pkg-config --libs glfw3 glm glew portaudio-2.0 ogg vorbisfile)
LDFLAGS += -dynamiclib -framework OpenGL
CXXFLAGS += -I../lib/SOIL/include -I.. -c -fPIC \
	$(shell pkg-config --cflags glfw3 glm glew portaudio-2.0 vorbisfile)

all: $(OBJ)

.cpp.o: .cpp
	@echo 编译C++文件$^……
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJ): $(Objects)
	@echo 链接最终二进制
	$(CCLD) $(LDLIBS) $(LDFLAGS) $^ -o $(OBJ)

clean:
	rm -rf $(OBJ) $(Objects)

.PHONY: all clean
