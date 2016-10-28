#==============================================================================
# ■ Makefile for Windows × Linux
#------------------------------------------------------------------------------
#   双坑合并！
#==============================================================================

# 用GCC链接。嗯，没什么问题。
LD = gcc

ifeq "$(OS)" "Windows_NT"
	/ = \\
	CC = gcc
	CXX = g++
	CD = chdir
	RM = del
	delete = -del $(1)
	ls = dir $(1) /b
else
	/ = /
	CD = cd
	delete = $(RM) $(1)
	ls = ls -1 $(1)
	CFLAGS = -I../lib/SOIL/include -I. -I..
	CFLAGS += $(shell pkg-config --cflags glfw3 gl glm glew portaudio-2.0)
	LDLIBS = -lstdc++ -lSOIL
	LDLIBS += $(shell pkg-config --libs --static glfw3 gl glm glew portaudio-2.0 ogg)
endif

CFLAGS = -std=c++14 -Wall -Wextra -O0
CXXFLAGS = $(CFLAGS)
LDFLAGS = -O4

all: $(TARGET)

%.o: %.cpp
	$(CXX) -c $^ -o $@ $(CXXFLAGS)

$(TARGET): $(OBJECTS)
	$(LD) $(LDFLAGS) $(LDLIBS) $^ -o $(TARGET)

clean:
	$(call delete, $(TARGET) $(OBJECTS))

VMDE:
	$(CD) VMDE
	$(eval TARGET = libVMDE.so)
	$(eval SOURCES = $(shell $(call ls, "*.cpp")))
	$(eval OBJECTS = $(patsubst %.cpp, %.o, $(SOURCES)))
	$(CD) ..

VMGS:
	$(CD) VMGS
	$(CD) ..

.PHONY: all clean VMDE VMGS
