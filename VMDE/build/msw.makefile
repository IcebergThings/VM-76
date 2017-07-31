#==============================================================================
# ■ VMDE - msw.makefile
#------------------------------------------------------------------------------
#   Windoge
#==============================================================================

include ../inc.makefile

TARGET = VMDE.dll

LDLIBS += $(shell type build\windows_libs.txt)
LDFLAGS += -flto -shared -Wl,--export-all-symbols
CXXFLAGS += -flto -I../lib/SOIL/include -I.. $(shell type build\windows_flags.txt)

all: $(TARGET)

%.o %.debug.o: %.cpp
	@echo -e "\033[33m 编译C++文件\033[47;30m $^…… \033[0m"
	$(CXX) -c $^ -o $@ $(CXXFLAGS)

$(TARGET): $(OBJECTS)
	@echo -e "\033[33m 链接最终二进制 \033[0m"
	$(CCLD) $^ -o $@ $(LDLIBS) $(LDFLAGS)

.PHONY: all
