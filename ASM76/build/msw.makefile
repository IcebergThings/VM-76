#==============================================================================
# ■ ASM76 - msw.makefile
#==============================================================================

include ../inc.makefile

TARGET = ASM76.dll

LDLIBS += $(shell type build\windows_libs.txt)
LDFLAGS += -shared -Wl,--export-all-symbols
CXXFLAGS += -I.. $(shell type build\windows_flags.txt)

all: $(TARGET)

%.o %.debug.o: %.cpp
	$(CXX) -c $^ -o $@ $(CXXFLAGS)

$(TARGET): $(OBJECTS)
	$(CCLD) $^ -o $@ $(LDLIBS) $(LDFLAGS)

VMtest: VMtest.cxx $(TARGET)
	$(CXX) $^ -o $@ $(CXXFLAGS) $(TARGET)

.PHONY: all
