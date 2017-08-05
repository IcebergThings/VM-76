#==============================================================================
# ■ routine.makefile
#------------------------------------------------------------------------------
#   Sometimes it's just too amazing.
#==============================================================================

# 又被include的cwd坑了一把

one:
ifeq "$(PLATFORM)" "msw"
	type nul > $(ONE_FILE) \
	& for %%i in ($(SOURCES)) do echo #include "%%i" >> $(ONE_FILE)
else
	echo $(SOURCES) | sed 's/ /\n/g' | sed 's/^/#include "/;s/$/"/' > $(ONE_FILE)
endif
	$(CXX) $(ONE_FILE) -o VMDE.one.dll $(CXXFLAGS) $(LDLIBS) $(LDFLAGS)
