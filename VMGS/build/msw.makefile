#==============================================================================
# ■ VMGS - msw.makefile
#==============================================================================

include ../inc.makefile

TARGET = VMGS.exe
SOURCES = $(shell dir /b /s *.cpp)
OBJECTS = $(patsubst %.cpp, %.o, $(SOURCES))

LDLIBS += $(shell type windows_libs.txt)
LDFLAGS += VMDE.dll
CXXFLAGS += -I../lib/SOIL/include -I../VMDE -I./Game -I.. \
	$(shell type windows_flags.txt)

all: $(TARGET)

%.o: %.cpp
	$(CXX) -c $^ -o $@ $(CXXFLAGS)

$(TARGET): VMDE.dll $(OBJECTS)
	$(CCLD) $^ -o $@ $(LDLIBS) $(LDFLAGS)

VMDE.dll: ../VMDE/VMDE.dll
	copy ..\VMDE\VMDE.dll $@

.PHONY: all
