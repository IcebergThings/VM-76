#==============================================================================
# ■ VMDE - msw.makefile
#------------------------------------------------------------------------------
#   Windoge
#==============================================================================

include ../inc.makefile

TARGET = VMDE.dll
SOURCES = $(shell dir /b /s *.cpp)
OBJECTS = $(patsubst %.cpp, %.o, $(SOURCES))

LDLIBS += $(shell type windows_libs.txt)
LDFLAGS += -shared -Wl,--export-all-symbols
CXXFLAGS += -I../lib/SOIL/include -I.. $(shell type windows_flags.txt)

all: $(TARGET)

%.o: %.cpp
	$(CXX) -c $^ -o $@ $(CXXFLAGS)

$(TARGET): $(OBJECTS)
	$(CCLD) $^ -o $@ $(LDLIBS) $(LDFLAGS)

.PHONY: all
