#==============================================================================
# ■ VMGS - msw.makefile
#==============================================================================

include ../inc.makefile

TARGET = VMGS.exe
SOURCES = $(shell dir /b /s *.cpp)
OBJECTS = $(patsubst %.cpp, %.o, $(SOURCES))

LDLIBS += $(shell type windows_libs.txt)
LDFLAGS += -Wl,--exclude-all-symbols ../VMDE/VMDE.dll
CXXFLAGS += -I../lib/SOIL/include -I.. $(shell type windows_flags.txt)

all: $(TARGET)

%.o: %.cpp
	$(CXX) -c $^ -o $@ $(CXXFLAGS)

$(TARGET): $(OBJECTS)
	$(CCLD) $^ -o $@ $(LDLIBS) $(LDFLAGS)

.PHONY: all
