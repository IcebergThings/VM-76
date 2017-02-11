#==============================================================================
# ■ VMGS - gnu.makefile
#==============================================================================

include ../inc.makefile

TARGET = libASM76.so

LDLIBS += $(shell pkg-config --libs --static glm)
LDFLAGS += -shared
CXXFLAGS += -I.. $(shell pkg-config --cflags glm) -fPIC

all: $(TARGET)

%.o %.debug.o: %.cpp
	@echo 编译C++文件$^……
	$(CXX) -c $^ -o $@ $(CXXFLAGS)

$(TARGET): $(OBJECTS)
	@echo 链接最终二进制
	$(CCLD) $^ -o $@ $(LDLIBS) $(LDFLAGS)

VMtest: VMtest.cxx $(TARGET)
	$(CXX) -c VMtest.cxx -o VMtest.o $(CXXFLAGS)
	$(CCLD) VMtest.o ./libASM76.so -o VMtest
	./VMtest

clean:
	rm -f $(OBJECTS) $(TARGET) VMtest

.PHONY: all
