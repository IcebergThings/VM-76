#==============================================================================
# ■ ASM76 - msw.makefile
#==============================================================================

include ../inc.makefile

TARGET = ASM76.dll

LDLIBS += $(shell type build\windows_libs.txt)
LDFLAGS += -flto -shared -Wl,--export-all-symbols
CXXFLAGS += -flto -I.. $(shell type build\windows_flags.txt)

all: $(TARGET)

%.o %.debug.o: %.cpp
	$(CXX) -c $^ -o $@ $(CXXFLAGS)

$(TARGET): $(OBJECTS)
	$(CCLD) $^ -o $@ $(LDLIBS) $(LDFLAGS)

test: test.exe
	test.exe

test.exe: VMtest.cxx $(TARGET)
	$(CXX) $^ -o $@ $(CXXFLAGS) $(TARGET)

.PHONY: all test
