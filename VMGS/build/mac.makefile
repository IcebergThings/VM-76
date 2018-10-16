#==============================================================================
# ■ VMGS - mac.makefile
#------------------------------------------------------------------------------
#   make.rb is not working on Mac
#==============================================================================

TARGET = VMGS

include ../inc.makefile
include ../routine.makefile

OBJECTS += libVMDE.dylib libASM76.dylib liblodepng.dylib

LDLIBS += $(shell pkg-config --libs --static glfw3 glm glew portaudio-2.0 vorbisfile)
LDFLAGS += ../VMDE/libVMDE.dylib ../lib/ASM76/libASM76.dylib ../lib/lodepng/liblodepng.dylib -framework OpenGL
CXXFLAGS += -I../lib/lodepng -I../VMDE -I../lib/ASM76 -I./Game -I.. -c \
	$(shell pkg-config --cflags glfw3 glm glew portaudio-2.0 vorbisfile)

libVMDE.dylib: ../VMDE/libVMDE.dylib
	@echo -e "\033[33m 更新动态库 VMDE \033[0m"
	cp ../VMDE/libVMDE.dylib .

libASM76.dylib: ../ASM76/libASM76.dylib
	@echo -e "\033[33m 更新动态库 ASM76 \033[0m"
	cp ../lib/ASM76/libVMDE.dylib .

liblodepng.dylib: ../ASM76/liblodepng.dylib
	@echo -e "\033[33m 更新动态库 liblodepng \033[0m"
	cp ../lib/lodepng/liblodepng.dylib .