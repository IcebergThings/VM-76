#==============================================================================
# ■ VMDE - msw.makefile
#------------------------------------------------------------------------------
#   Windoge
#==============================================================================

include ../inc.makefile

TARGET = VMDE.dll

LDLIBS += -L../ASM76 -lASM76 $(shell type build\windows_libs.txt)
LDFLAGS += -shared -Wl,--export-all-symbols
CXXFLAGS += -I../lib/SOIL/include -I.. -I../ASM76 \
	$(shell type build\windows_flags.txt)

all: $(TARGET)

%.o %.debug.o: %.cpp
	$(CXX) -c $^ -o $@ $(CXXFLAGS)

$(TARGET): $(OBJECTS)
	$(CCLD) $^ -o $@ $(LDLIBS) $(LDFLAGS)

.PHONY: all
