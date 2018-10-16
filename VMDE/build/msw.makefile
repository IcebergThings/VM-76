#==============================================================================
# ■ VMDE - msw.makefile
#------------------------------------------------------------------------------
#   Windoge
#==============================================================================

TARGET = VMDE.dll

include ../inc.makefile
include ../routine.makefile

LDLIBS += -L../ASM76 -lASM76 $(shell type build\windows_libs.txt)
LDFLAGS += -shared -Wl,--export-all-symbols
CXXFLAGS += -I../lib/lodepng -I.. -I../lib/ASM76 \
	$(shell type build\windows_flags.txt)
