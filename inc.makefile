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
CFLAGS = -Wall -Wextra -Wno-unused-parameter -O3 -Ofast
CXXFLAGS = $(CFLAGS) -std=c++14 -msse4.1 -mtune=core2 -DGLM_FORCE_SSE41
LDFLAGS = -O4
LDLIBS = -lstdc++

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
