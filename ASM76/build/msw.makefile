#==============================================================================
# ■ ASM76 - msw.makefile
#==============================================================================

TARGET = ASM76.dll

include ../inc.makefile
include ../routine.makefile

LDLIBS += $(shell type build\windows_libs.txt)
LDFLAGS += -shared -Wl,--export-all-symbols -pthread
CXXFLAGS += -I.. $(shell type build\windows_flags.txt)

all: $(TARGET)

test: VMtest.cxx $(TARGET)
	$(CXX) $^ -o VMtest.exe $(CXXFLAGS) $(TARGET)
	VMtest.exe

.PHONY: test
