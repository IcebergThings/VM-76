#==============================================================================
# ■ VMGS - gnu.makefile
#==============================================================================

include ../inc.makefile

TARGET = VMGS

LDLIBS += \
	$(shell pkg-config --libs --static glfw3 glm gl glew portaudio-2.0 vorbisfile)
LDFLAGS += -fPIC ../VMDE/libVMDE.so
CXXFLAGS += -I../lib/SOIL/include -I../VMDE -IGame -I.. \
	$(shell pkg-config --cflags glfw3 glm glew gl portaudio-2.0 vorbisfile)

all:: $(TARGET)
