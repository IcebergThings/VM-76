#==============================================================================
# ■ VMDE/makefile
#------------------------------------------------------------------------------
#   A placeholder for make
#==============================================================================

all:
	../make.rb \
	-DGLM_FORCE_SSE41 \
	-I../lib/SOIL/include -I.. \
	-O3 -Ofast -msse4.1 -mtune=core2 \
	$(shell pkg-config --cflags glfw3 gl glm glew portaudio-2.0 vorbisfile)
clean:
	../make.rb clean
.PHONY: all clean
