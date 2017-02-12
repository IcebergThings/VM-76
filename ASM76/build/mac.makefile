#==============================================================================
# ■ VMGS - mac.makefile
#------------------------------------------------------------------------------
#   make.rb is not working on Mac
#==============================================================================

include ../inc.makefile

TARGET = libASM76.so

LDLIBS += $(shell pkg-config --libs --static glm)
LDFLAGS += -shared
CXXFLAGS += -I.. $(shell pkg-config --cflags glm)

all:: $(TARGET)

VMtest: VMtest.cpp $(TARGET)
	$(CXX) $^ -o $@ $(CXXFLAGS) $(TARGET)

.PHONY: all
