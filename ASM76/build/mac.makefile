#==============================================================================
# ■ ASM76 - mac.makefile
#==============================================================================

TARGET = libASM76.dylib

include ../inc.makefile
include ../routine.makefile

LDLIBS += $(shell pkg-config --libs --static glm)
LDFLAGS += -shared
CXXFLAGS += -I.. $(shell pkg-config --cflags glm) -fPIC

test: VMtest.cxx $(TARGET)
	$(CXX) -c VMtest.cxx -o VMtest.o $(CXXFLAGS)
	$(CCLD) -flto VMtest.o ./libASM76.dylib -lstdc++ -m -o VMtest
	./VMtest

clean:
	rm -f $(OBJECTS) $(TARGET) VMtest

.PHONY: test
