#==============================================================================
# ■ VMDE - gnu.makefile
#==============================================================================

# 这破include居然是根据cwd来的！
include ../inc.makefile
include ../routine.makefile

TARGET = libVMDE.so

LDLIBS += /usr/lib/libSOIL.so \
	$(shell pkg-config --libs glfw3 gl glm glew portaudio-2.0 ogg vorbisfile)
LDFLAGS += -shared
CXXFLAGS += -I../lib/SOIL/include -I../ASM76 -I.. -fPIC \
	$(shell pkg-config --cflags gl glfw3 glm glew portaudio-2.0 vorbisfile)

all: $(TARGET)

%.o %.debug.o: %.cpp
	@echo -e "\033[33m 编译C++文件\033[47;30m $^…… \033[0m"
	$(CXX) -c $^ -o $@ $(CXXFLAGS)

$(TARGET): $(OBJECTS)
	@echo -e "\033[33m 链接最终二进制 \033[0m"
	$(CCLD) $^ -o $@ $(LDLIBS) $(LDFLAGS)

.PHONY: all
