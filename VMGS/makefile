#==============================================================================
# ■ VMGS/makefile
#------------------------------------------------------------------------------
#   A placeholder for make
#==============================================================================

all:
	../make.rb \
	-DGLM_FORCE_SSE41 \
	-I../lib/SOIL/include -I../VMDE \
	-O3 -Ofast -msse4.1 -mtune=core2 \
	$(shell pkg-config --cflags --libs glfw3 gl glm glew portaudio-2.0 vorbisfile) \
	-lstdc++ -lSOIL
clean:
	../make.rb clean
.PHONY: all clean
