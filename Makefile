#==============================================================================
# ■ makefile
#==============================================================================

include inc.makefile

PROJECTS = VMDE VMGS

all: $(PROJECTS)

$(PROJECTS):
	cd $@ && $(MAKE) -f build/$(PLATFORM).makefile

clean:
ifeq "$(PLATFORM)" "msw"
	del /s *.o
else
	find . -name "*.o" -print -delete
endif

run: all
	cd VMGS && ./VMGS

platform:
	@echo $(PLATFORM)

.PHONY: all clean run $(PROJECTS) platform
