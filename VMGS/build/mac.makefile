#==============================================================================
# ■ VMGS - mac.makefile
#------------------------------------------------------------------------------
#   make.rb is not working on Mac
#==============================================================================

TARGET = VMGS

include ../inc.makefile
include ../routine.makefile

OBJECTS += libVMDE.dylib libASM76.dylib

LDLIBS += $(shell pkg-config --libs --static glfw3 glm glew portaudio-2.0 vorbisfile)
LDFLAGS += ../VMDE/libVMDE.dylib ../ASM76/libASM76.dylib -framework OpenGL
CXXFLAGS += -I../lib/SOIL/include -I../VMDE -I../ASM76 -I.. -c \
	$(shell pkg-config --cflags glfw3 glm glew portaudio-2.0 vorbisfile)

all: libVMDE.dylib libASM76.dylib $(TARGET)

libVMDE.dylib: ../VMDE/libVMDE.dylib
	@echo -e "\033[33m 更新动态库 VMDE \033[0m"
	cp ../VMDE/libVMDE.dylib .

libASM76.dylib: ../ASM76/libASM76.dylib
	@echo -e "\033[33m 更新动态库 ASM76 \033[0m"
	cp ../VMDE/libVMDE.dylib .
