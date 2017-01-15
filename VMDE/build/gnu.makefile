#==============================================================================
# ■ VMDE - gnu.makefile
#==============================================================================

# 这破include居然是根据cwd来的！
include ../inc.makefile

OBJ = libVMDE.so
Src = $(shell find . -name "*.cpp")
Objects = $(patsubst %.cpp, %.o, $(Src))

LDLIBS += $(shell pkg-config --libs glfw3 gl glm glew portaudio-2.0 ogg vorbisfile) /usr/lib/libSOIL.so
LDFLAGS += -shared
CXXFLAGS += -I../lib/SOIL/include  -I.. -c -fPIC \
	$(shell pkg-config --cflags gl glfw3 glm glew portaudio-2.0 vorbisfile)

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
