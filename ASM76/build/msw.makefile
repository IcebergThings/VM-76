#==============================================================================
# ■ ASM76 - msw.makefile
#==============================================================================

include ../inc.makefile

TARGET = ASM76.dll

LDLIBS += $(shell type build\windows_libs.txt)
CXXFLAGS += -I.. $(shell type build\windows_flags.txt)

all: $(TARGET)

%.o %.debug.o: %.cpp
	@echo 编译C++文件$^……
	$(CXX) -c $^ -o $@ $(CXXFLAGS)

$(TARGET): $(OBJECTS)
	@echo 链接最终二进制
	$(CCLD) $^ -o $@ $(LDLIBS) $(LDFLAGS)

VMtest: VMtest.cxx $(TARGET)
	$(CXX) $^ -o $@ $(CXXFLAGS) $(TARGET)

.PHONY: all
