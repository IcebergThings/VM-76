#==============================================================================
# ■ VMGS - mac.makefile
#------------------------------------------------------------------------------
#   make.rb is not working on Mac
#==============================================================================

include ../inc.makefile

TARGET = VMGS
OBJECTS += libVMDE.dylib

LDLIBS += $(shell pkg-config --libs --static glfw3 glm glew portaudio-2.0 vorbisfile)
LDFLAGS += ../VMDE/libVMDE.dylib -framework OpenGL
CXXFLAGS += -I../lib/SOIL/include -I../VMDE -I./Game -I.. \
	$(shell pkg-config --cflags glfw3 glm glew portaudio-2.0 vorbisfile)

all:: $(TARGET)
run: all
	@echo 运行
	./$(TARGET)

libVMDE.dylib: ../VMDE/libVMDE.dylib
	@echo 更新动态库
	cp ../VMDE/libVMDE.dylib .

clean:
	rm -rf $(TARGET) $(OBJECTS)mcpu

.PHONY: run clean
