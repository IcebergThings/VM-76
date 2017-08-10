#==============================================================================
# ■ makefile
#==============================================================================

include inc.makefile

PROJECTS = ASM76 VMDE VMGS

all: $(PROJECTS)

$(PROJECTS):
	cd $@ && $(MAKE) -f build/$(PLATFORM).makefile

clean:
ifeq "$(PLATFORM)" "msw"
	del /s *.o *.o-????????
	del VMGS\output.log
else
	find . -name "*.o" -print -delete
	$(RM) VMGS/output.log
endif

run:
	$(MAKE) $(PROJECTS) MODE=test
ifeq "$(PLATFORM)" "msw"
	call windows_path.bat && cd VMGS && VMGS.exe
else
	cd VMGS && ./VMGS
endif

debug:
	$(MAKE) $(PROJECTS) MODE=debug
ifeq "$(PLATFORM)" "msw"
	call windows_path.bat && cd VMGS && gdb VMGS.exe
else
	cd VMGS && gdb VMGS
endif

platform:
	@echo $(PLATFORM)

.PHONY: all clean run debug $(PROJECTS) platform
