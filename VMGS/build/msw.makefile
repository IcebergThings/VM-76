#==============================================================================
# ■ VMGS - msw.makefile
#==============================================================================

TARGET = VMGS.exe

include ../inc.makefile
include ../routine.makefile

OBJECTS += ASM76.dll VMDE.dll windows.rc.o

LDLIBS += $(shell type build\windows_libs.txt)
LDFLAGS += VMDE.dll ASM76.dll
CXXFLAGS += -I../lib/SOIL/include -I../VMDE -I../ASM76 -I.. \
	$(shell type build\windows_flags.txt)

all: VMDE.dll ASM76.dll $(TARGET)

VMDE.dll: ../VMDE/VMDE.dll
	copy ..\VMDE\VMDE.dll $@

ASM76.dll: ../ASM76/ASM76.dll
	copy ..\ASM76\ASM76.dll $@

windows.rc.o: windows.rc
	windres -i windows.rc -o windows.rc.o
