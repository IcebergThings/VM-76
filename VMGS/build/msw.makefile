#==============================================================================
# ■ VMGS - msw.makefile
#==============================================================================

include ../inc.makefile

TARGET = VMGS.exe

LDLIBS += $(shell type build\windows_libs.txt)
LDFLAGS += VMDE.dll ASM76.dll
CXXFLAGS += -I../lib/SOIL/include -I../VMDE -I../ASM76 -I./Game -I.. \
	$(shell type build\windows_flags.txt)

all: $(TARGET)

%.o %.debug.o: %.cpp
	$(CXX) -c $^ -o $@ $(CXXFLAGS)

%.o: %.rc
$(TARGET): VMDE.dll $(OBJECTS) windows.rc.o
	$(CCLD) $^ -o $@ $(LDLIBS) $(LDFLAGS)

VMDE.dll: ../VMDE/VMDE.dll
	copy ..\VMDE\VMDE.dll $@

ASM76.dll: ../ASM76/ASM76.dll
	copy ..\ASM76\ASM76.dll $@

windows.rc.o: windows.rc
	windres -i windows.rc -o windows.rc.o

.PHONY: all
