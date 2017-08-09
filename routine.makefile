#==============================================================================
# ■ routine.makefile
#------------------------------------------------------------------------------
#   Sometimes it's just too amazing.
#==============================================================================

# 又被include的cwd坑了一把

# Useful variables.
ifeq "$(PLATFORM)" "msw"
	SOURCES = $(subst $(shell cd)\,,$(shell dir /b /s *.cpp))
else ifeq "$(PLATFORM)" "gnu"
	SOURCES = $(shell find . -name "*.cpp")
else ifeq "$(PLATFORM)" "mac"
	SOURCES = $(shell find . -name "*.cpp")
endif

ifdef DEBUG
	OBJECTS = $(SOURCES:%.cpp=%.debug.o)
else
	OBJECTS = $(SOURCES:%.cpp=%.o)
endif

ONE_FILE = one.cxx

all: $(TARGET)

one:
ifeq "$(PLATFORM)" "msw"
	type nul > $(ONE_FILE) \
	& for %%i in ($(SOURCES)) do echo #include "%%i" >> $(ONE_FILE)
else
	echo $(SOURCES) | sed 's/ /\n/g' | sed 's/^/#include "/;s/$/"/' > $(ONE_FILE)
endif
	$(CXX) $(ONE_FILE) -o VMDE.one.dll $(CXXFLAGS) $(LDLIBS) $(LDFLAGS)

%.o %.debug.o: %.cpp
	$(call ECHO_BANNER_BOLD,编译C++文件,$^)
	$(CXX) -c $^ -o $@ $(CXXFLAGS)

$(TARGET): $(OBJECTS)
	$(call ECHO_BANNER,链接最终二进制)
	$(CCLD) $^ -o $@ $(LDLIBS) $(LDFLAGS)

clean:
ifeq "$(PLATFORM)" "msw"
else
	$(RM) $(TARGET) $(OBJECTS)mcpu
endif

run: all
	$(call ECHO_BANNER,运行)
ifeq "$(PLATFORM)" "msw"
	$(TARGET)
else
	./$(TARGET)
endif

.PHONY: all one clean
