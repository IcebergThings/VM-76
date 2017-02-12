#==============================================================================
# ■ VMGS - msw.makefile
#==============================================================================

include ../inc.makefile

TARGET = VMGS.exe
OBJECTS += VMDE.dll

LDLIBS += $(shell type build\windows_libs.txt)
LDFLAGS += VMDE.dll
CXXFLAGS += -I../lib/SOIL/include -I../VMDE -I./Game -I.. \
	$(shell type build\windows_flags.txt)

all:: $(TARGET)

$(TARGET):  $(OBJECTS)
	$(CCLD) $^ -o $@ $(LDLIBS) $(LDFLAGS)

VMDE.dll: ../VMDE/VMDE.dll
	copy ..\VMDE\VMDE.dll $@
