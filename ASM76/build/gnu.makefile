#==============================================================================
# ■ VMGS - gnu.makefile
#==============================================================================

include ../inc.makefile

TARGET = libASM76.so

LDLIBS += $(shell pkg-config --libs --static glm)
LDFLAGS += -shared
CXXFLAGS += -I.. $(shell pkg-config --cflags glm) -fPIC

all:: $(TARGET)

VMtest: VMtest.cxx $(TARGET)
	$(CXX) -c VMtest.cxx -o VMtest.o $(CXXFLAGS)
	$(CCLD) VMtest.o ./libASM76.so -lstdc++ -o VMtest
	./VMtest

clean:
	rm -f $(OBJECTS) $(TARGET) VMtest

.PHONY: all
