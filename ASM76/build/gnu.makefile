#==============================================================================
# ■ ASM76 - gnu.makefile
#==============================================================================

TARGET = libASM76.so

include ../inc.makefile
include ../routine.makefile

LDLIBS += $(shell pkg-config --libs --static glm)
LDFLAGS += -shared
CXXFLAGS += -I.. $(shell pkg-config --cflags glm) -fPIC

test: VMtest.cxx $(TARGET)
	$(CXX) -c VMtest.cxx -o VMtest.o $(CXXFLAGS)
	$(CCLD) VMtest.o ./libASM76.so -lstdc++ -flto -lm -o VMtest
	./VMtest

clean:
	rm -f $(OBJECTS) $(TARGET) VMtest

.PHONY: test
