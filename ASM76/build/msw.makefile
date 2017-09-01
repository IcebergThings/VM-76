#==============================================================================
# ■ ASM76 - msw.makefile
#==============================================================================

TARGET = ASM76.dll

include ../inc.makefile
include ../routine.makefile

LDLIBS += $(shell type build\windows_libs.txt)
LDFLAGS += -shared -Wl,--export-all-symbols -pthread
CXXFLAGS += -I.. $(shell type build\windows_flags.txt)

all: $(TARGET) asm76util.exe

test: VMtest.cxx $(TARGET)
	$(CXX) $^ -o VMtest.exe $(CXXFLAGS)
	VMtest.exe

asm76util.exe: asm76util.cxx $(TARGET)
	$(CXX) $^ -o $@ $(CXXFLAGS)

.PHONY: test
