#==============================================================================
# ■ VMGS - msw.makefile
#==============================================================================

TARGET = VMGS.exe

include ../inc.makefile
include ../routine.makefile

OBJECTS += ASM76.dll VMDE.dll lodepng.dll windows.rc.o

LDLIBS += $(shell type build\windows_libs.txt)
LDFLAGS += VMDE.dll ASM76.dll lodepng.dll
CXXFLAGS += -I../lib/lodepng -I../VMDE -I../lib/ASM76 -I./Game -I.. \
	$(shell type build\windows_flags.txt)

VMDE.dll: ../VMDE/VMDE.dll
	copy ..\VMDE\VMDE.dll $@

ASM76.dll: ../ASM76/ASM76.dll
	copy ..\lib\ASM76\ASM76.dll $@

lodepng.dll: ../ASM76/ASM76.dll
	copy ..\lib\lodepng\lodepng.dll $@

windows.rc.o: windows.rc
	windres -i windows.rc -o windows.rc.o
