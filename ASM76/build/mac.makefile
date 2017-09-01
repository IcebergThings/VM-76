#==============================================================================
# ■ ASM76 - mac.makefile
#==============================================================================

TARGET = libASM76.dylib

include ../inc.makefile
include ../routine.makefile

LDLIBS += $(shell pkg-config --libs --static glm)
LDFLAGS += -shared
CXXFLAGS += -I.. $(shell pkg-config --cflags glm) -fPIC

all: $(TARGET) asm76util

test: VMtest.cxx $(TARGET)
	$(CXX) -c VMtest.cxx -o VMtest.o $(CXXFLAGS)
	$(CCLD) -flto VMtest.o ./libASM76.dylib -lstdc++ -m -o VMtest
	./VMtest

asm76util: asm76util.cxx $(TARGET)
	$(CXX) -c asm76util.cxx -o asm76util.o $(CXXFLAGS)
	$(CCLD) -flto asm76util.o ./libASM76.dylib -lstdc++ -m -o $@

clean:
	rm -f $(OBJECTS) $(TARGET) VMtest asm76util

.PHONY: test
