#==============================================================================
# ■ VMGS - gnu.makefile
#==============================================================================

include ../inc.makefile

TARGET = libASM76.so

LDLIBS += $(shell pkg-config --libs --static glm)
LDFLAGS += -shared
CXXFLAGS += -I.. $(shell pkg-config --cflags glm)

all: $(TARGET)

%.o %.debug.o: %.cpp
	@echo 编译C++文件$^……
	$(CXX) -c $^ -o $@ $(CXXFLAGS)

$(TARGET): $(OBJECTS)
	@echo 链接最终二进制
	$(CCLD) $^ -o $@ $(LDLIBS) $(LDFLAGS)

VMtest: VMtest.cxx
	$(CXX) $^ -o $@ $(CXXFLAGS) ./libASM76.so

.PHONY: all
