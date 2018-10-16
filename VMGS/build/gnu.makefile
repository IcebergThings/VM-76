#==============================================================================
# ■ VMGS - gnu.makefile
#==============================================================================

TARGET = VMGS

include ../inc.makefile
include ../routine.makefile

LDLIBS += \
	$(shell pkg-config --libs --static glfw3 glm gl glew portaudio-2.0 vorbisfile)
LDFLAGS += -fPIC ../VMDE/libVMDE.so ../lib/ASM76/libASM76.so ../lib/lodepng/liblodepng.so
CXXFLAGS += -I../lib/lodepng -I../VMDE -I../lib/ASM76 -IGame -I.. \
	$(shell pkg-config --cflags glfw3 glm glew gl portaudio-2.0 vorbisfile)
