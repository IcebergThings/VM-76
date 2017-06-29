#==============================================================================
# ■ VMGS - mac.makefile
#------------------------------------------------------------------------------
#   make.rb is not working on Mac
#==============================================================================

include ../inc.makefile

TARGET = VMGS

LDLIBS += $(shell pkg-config --libs --static glfw3 glm glew portaudio-2.0 vorbisfile)
LDFLAGS += -flto ../VMDE/libVMDE.dylib -framework OpenGL
CXXFLAGS += -flto -I../lib/SOIL/include -I../VMDE -I./Game -I.. -c \
	$(shell pkg-config --cflags glfw3 glm glew portaudio-2.0 vorbisfile)

all: $(TARGET)
run: all
	@echo 运行
	./$(TARGET)

%.o %.debug.o: %.cpp
	@echo 编译C++文件$^……
	$(CXX) $(CXXFLAGS) $^ -o $@

libVMDE.dylib: ../VMDE/libVMDE.dylib
	@echo 更新动态库
	cp ../VMDE/libVMDE.dylib .

$(TARGET): $(OBJECTS) libVMDE.dylib
	@echo 链接最终二进制
	$(CCLD) $(LDLIBS) $(LDFLAGS) $^ -o $@

clean:
	rm -rf $(TARGET) $(OBJECTS)mcpu

.PHONY: all run clean
