#==============================================================================
# ■ ASM76 - gnu.makefile
#==============================================================================

TARGET = libASM76.so

include ../inc.makefile
include ../routine.makefile

LDLIBS += $(shell pkg-config --libs --static glm)
LDFLAGS += -shared
CXXFLAGS += -I.. $(shell pkg-config --cflags glm) -fPIC

all: $(TARGET) asm76util

test: VMtest.cxx $(TARGET)
	$(CXX) -c VMtest.cxx -o VMtest.o $(CXXFLAGS)
	$(CCLD) VMtest.o ./libASM76.so -lstdc++ -flto -lm -o VMtest
	./VMtest

asm76util: asm76util.cxx $(TARGET)
	$(CXX) -c asm76util.cxx -o asm76util.o $(CXXFLAGS)
	$(CCLD) asm76util.o ./libASM76.so -lstdc++ -flto -lm -o $@

clean:
	rm -f $(OBJECTS) $(TARGET) VMtest

.PHONY: test
