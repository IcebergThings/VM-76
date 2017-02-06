#==============================================================================
# ■ VMGS - gnu.makefile
#==============================================================================

include ../inc.makefile

OBJ = VMGS
Src = $(shell find . -name "*.cpp")
Objects = $(patsubst %.cpp, %.o, $(Src))

LDLIBS += \
	$(shell pkg-config --libs --static glfw3 glm gl glew portaudio-2.0 vorbisfile)
LDFLAGS += -fPIC ../VMDE/libVMDE.so
CXXFLAGS += -I../lib/SOIL/include -I../VMDE -IGame -I.. \
	$(shell pkg-config --cflags glfw3 glm glew gl portaudio-2.0 vorbisfile)

all: $(OBJ)

%.o: %.cpp
	@echo 编译C++文件$^……
	$(CXX) -c $^ -o $@ $(CXXFLAGS)

$(OBJ): $(Objects)
	@echo 链接最终二进制
	$(CCLD) $^ -o $@ $(LDLIBS) $(LDFLAGS)

.PHONY: all
