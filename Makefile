#==============================================================================
# ■ makefile
#==============================================================================

include inc.makefile

PROJECTS = VMDE VMGS

all: $(PROJECTS)

$(PROJECTS):
	cd $@ && $(MAKE) -f build/$(PLATFORM).makefile

clean:
ifeq "$(OS)" "msw"
	-del *.o
	# TODO: I shall test this on Windows.
	#for /f %f in ('dir *.o /b /s') do del %f
else
	find . -name "*.o" -print -delete
endif

run: all
	cd VMGS && ./VMGS

platform:
	@echo $(PLATFORM)

.PHONY: all clean run $(PROJECTS) platform
