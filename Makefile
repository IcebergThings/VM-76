#==============================================================================
# ■ makefile
#==============================================================================

PROJECTS = VMDE VMGS

# Determine the PLATFORM.
# PLATFORM can be one of these: msw, gnu, mac, unknown
ifeq "$(SHELL)" "/bin/sh"
	UNAME_S = $(shell uname -s)
	ifeq "$(UNAME_S)" "Linux"
		PLATFORM = gnu
	else ifeq "$(UNAME_S)" "Darwin"
		PLATFORM = mac
	else
		PLATFORM = unknown
	endif
else ifeq "$(OS)" "Windows_NT"
	PLATFORM = msw
else
	PLATFORM = unknown
endif

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
