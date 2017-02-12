#==============================================================================
# ■ VMGS - msw.makefile
#==============================================================================

include ../inc.makefile

TARGET = ASM76.dll

LDLIBS += $(shell type build\windows_libs.txt)
CXXFLAGS += -I.. $(shell type build\windows_flags.txt)

all:: $(TARGET)

VMtest: VMtest.cpp $(TARGET)
	$(CXX) $^ -o $@ $(CXXFLAGS) $(TARGET)

.PHONY: all
