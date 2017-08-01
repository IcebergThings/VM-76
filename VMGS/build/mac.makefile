#==============================================================================
# ■ VMGS - mac.makefile
#------------------------------------------------------------------------------
#   make.rb is not working on Mac
#==============================================================================

include ../inc.makefile

TARGET = VMGS

LDLIBS += $(shell pkg-config --libs --static glfw3 glm glew portaudio-2.0 vorbisfile)
LDFLAGS += ../VMDE/libVMDE.dylib -framework OpenGL
CXXFLAGS += -I../lib/SOIL/include -I../VMDE -I./Game -I.. -c \
	$(shell pkg-config --cflags glfw3 glm glew portaudio-2.0 vorbisfile)

all: $(TARGET)
run: all
	@echo 运行
	./$(TARGET)

%.o %.debug.o: %.cpp
	@echo -e "\033[33m 编译C++文件\033[47;30m $^…… \033[0m"
	$(CXX) $(CXXFLAGS) $^ -o $@

libVMDE.dylib: ../VMDE/libVMDE.dylib
	@echo -e "\033[33m 更新动态库 \033[0m"
	cp ../VMDE/libVMDE.dylib .

$(TARGET): $(OBJECTS) libVMDE.dylib
	@echo -e "\033[33m 链接最终二进制 \033[0m"
	$(CCLD) $(LDLIBS) $(LDFLAGS) $^ -o $@

clean:
	rm -rf $(TARGET) $(OBJECTS)mcpu

.PHONY: all run clean
