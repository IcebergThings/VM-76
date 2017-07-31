#==============================================================================
# ■ VMGS - msw.makefile
#==============================================================================

include ../inc.makefile

TARGET = VMGS.exe

LDLIBS += $(shell type build\windows_libs.txt)
LDFLAGS += -flto VMDE.dll
CXXFLAGS += -flto -I../lib/SOIL/include -I../VMDE -I./Game -I.. \
	$(shell type build\windows_flags.txt)

all: $(TARGET)

%.o %.debug.o: %.cpp
	@echo -e "\033[33m 编译C++文件\033[47;30m $^…… \033[0m"
	$(CXX) -c $^ -o $@ $(CXXFLAGS)

%.o: %.rc
	@echo -e "\033[33m 生成描述文件 \033[0m"
	windres -i $^ -o $@

$(TARGET): VMDE.dll $(OBJECTS)
	@echo -e "\033[33m 链接最终二进制 \033[0m"

VMDE.dll: ../VMDE/VMDE.dll
	@echo -e "\033[33m 更新动态库 \033[0m"
	copy ..\VMDE\VMDE.dll $@

.PHONY: all
