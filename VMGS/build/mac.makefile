#==============================================================================
# ■ VMGS - mac.makefile
#------------------------------------------------------------------------------
#   make.rb is not working on Mac
#==============================================================================

include ../inc.makefile

OBJ = VMGS
Src = $(shell find . -name "*.cpp")
Objects = $(patsubst %.cpp, %.o, $(Src))

LDLIBS += $(shell pkg-config --libs --static glfw3 glm glew portaudio-2.0 vorbisfile)
LDFLAGS += ../VMDE/libVMDE.dylib -framework OpenGL
CXXFLAGS += -I../lib/SOIL/include -I../VMDE -I./Game -I.. -c \
	$(shell pkg-config --cflags glfw3 glm glew portaudio-2.0 vorbisfile)

all: $(OBJ)
run: all
	@echo 运行
	./$(OBJ)

.cpp.o: .cpp
	@echo 编译C++文件$^……
	$(CXX) $(CXXFLAGS) $^ -o $@

# mac_build.sh存在的唯一意义被融化进了这个Makefile：
libVMDE.dylib: ../VMDE/libVMDE.dylib
	@echo 更新动态库
	cp ../VMDE/libVMDE.dylib .

VMGS: $(Objects) libVMDE.dylib
	@echo 链接最终二进制
	$(CCLD) $(LDLIBS) $(LDFLAGS) $^ -o VMGS

clean:
	rm -rf $(OBJ) $(Objects)mcpu

.PHONY: all run clean
