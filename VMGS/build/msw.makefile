#==============================================================================
# ■ VMGS - msw.makefile
#==============================================================================

include ../inc.makefile

TARGET = VMGS.exe

LDLIBS += $(shell type build\windows_libs.txt)
LDFLAGS += VMDE.dll
CXXFLAGS += -I../lib/SOIL/include -I../VMDE -I./Game -I.. \
	$(shell type build\windows_flags.txt)

all: $(TARGET)

%.o %.debug.o: %.cpp
	@echo -e "\033[33m 编译C++文件\033[47;30m $^…… \033[0m"
	$(CXX) -c $^ -o $@ $(CXXFLAGS)

%.o: %.rc
$(TARGET): VMDE.dll $(OBJECTS) windows.rc.o
	$(CCLD) $^ -o $@ $(LDLIBS) $(LDFLAGS)

VMDE.dll: ../VMDE/VMDE.dll
	@echo -e "\033[33m 更新动态库 \033[0m"
	copy ..\VMDE\VMDE.dll $@

windows.rc.o: windows.rc
	windres -i windows.rc -o windows.rc.o

.PHONY: all
