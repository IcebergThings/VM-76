#==============================================================================
# ■ inc.makefile
#------------------------------------------------------------------------------
#   This makefile is intended to be included in every makefile in this project.
#==============================================================================

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

# Global project settings.
CXX = clang++
CCLD = clang
CFLAGS = -Wall -Wextra -Wno-unused-parameter -msse4.1 -mtune=core2
CXXFLAGS = $(CFLAGS) -std=c++14 -DGLM_FORCE_SSE41
LDFLAGS =
LDLIBS = -lstdc++
ifdef DEBUG
	CXXFLAGS += -O0 -g -DDEBUG
	LDFLAGS += -O0
else
	CXXFLAGS += -O3 -Ofast
	LDFLAGS += -O4
endif

ifeq "$(PLATFORM)" "msw"
	ifdef GCC
		CXX = g++
	else
		CFLAGS += --target=i686-pc-mingw32
	endif
	CCLD = gcc
else ifeq "$(PLATFORM)" "gnu"
else ifeq "$(PLATFORM)" "mac"
	CXX = clang
endif

# Useful variables.
ifeq "$(PLATFORM)" "msw"
	SOURCES = $(shell dir /b /s *.cpp *.rc)
else ifeq "$(PLATFORM)" "gnu"
	SOURCES = $(shell find . -name "*.cpp")
else ifeq "$(PLATFORM)" "mac"
	SOURCES = $(shell find . -name "*.cpp")
endif

ifdef DEBUG
	OBJECTS = $(SOURCES:%.cpp=%.debug.o)
else
	OBJECTS := $(SOURCES:%.cpp=%.o)
	OBJECTS := $(OBJECTS:%.rc=%.o)
endif

# General recipes.
# They are copied over and over until they are here.
all::
.PHONY: all

%.o %.debug.o: %.cpp
	$(CXX) -c $^ -o $@ $(CXXFLAGS)

%.o: %.rc
	windres -i $^ -o $@

$(TARGET): $(OBJECTS)
	$(CCLD) $^ -o $@ $(LDLIBS) $(LDFLAGS)
