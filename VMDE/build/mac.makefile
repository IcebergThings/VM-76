#==============================================================================
# ■ VMDE - mac.makefile
#------------------------------------------------------------------------------
#   Mac does not support that make.rb, in a strange way...
#==============================================================================

include ../inc.makefile

TARGET = libVMDE.dylib

LDLIBS += /usr/local/lib/libSOIL.a \
	$(shell pkg-config --libs glfw3 glm glew portaudio-2.0 ogg vorbisfile)
LDFLAGS += -dynamiclib -framework OpenGL
CXXFLAGS += -I../lib/SOIL/include -I.. -I../ASM76 -c -fPIC \
	$(shell pkg-config --cflags glfw3 glm glew portaudio-2.0 vorbisfile)

all: $(TARGET)

%.o %.debug.o: %.cpp
	@echo -e "\033[33m 编译C++文件\033[47;30m $^…… \033[0m"
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TARGET): $(OBJECTS)
	@echo -e "\033[33m 链接最终二进制 \033[0m"
	$(CCLD) $(LDLIBS) $(LDFLAGS) $^ -o $@

clean:
	rm -rf $(TARGET) $(OBJECTS)

.PHONY: all clean
