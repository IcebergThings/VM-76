#==============================================================================
# ■ VMGS - gnu.makefile
#==============================================================================

include ../inc.makefile

TARGET = VMGS

LDLIBS += \
	$(shell pkg-config --libs --static glfw3 glm gl glew portaudio-2.0 vorbisfile)
LDFLAGS += -flto -fPIC ../VMDE/libVMDE.so
CXXFLAGS += -flto -I../lib/SOIL/include -I../VMDE -IGame -I.. \
	$(shell pkg-config --cflags glfw3 glm glew gl portaudio-2.0 vorbisfile)

all: $(TARGET)

%.o %.debug.o: %.cpp
	@echo 编译C++文件$^……
	$(CXX) -c $^ -o $@ $(CXXFLAGS)

$(TARGET): $(OBJECTS)
	@echo 链接最终二进制
	$(CCLD) $^ -o $@ $(LDLIBS) $(LDFLAGS)

.PHONY: all
