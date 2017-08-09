#==============================================================================
# ■ inc.makefile
#------------------------------------------------------------------------------
#   This makefile is intended to be included in every makefile in this project.
#==============================================================================

ifndef _INCLUDE_INC_MAKEFILE
	# Makefile include guard.
	_INCLUDE_INC_MAKEFILE := 1

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

	# Determine the build MODE.
	# MODE can be one of the following:
	#     debug - for use with GDB, with optimizations turned off
	#     test - for normal development testing
	#     release - for releasing, maybe I'll add automatic packing sometime
	ifndef MODE
		MODE = test
	endif
	ifeq "$(MODE)" "debug"
		BUILD_MODE_MACRO = BUILD_MODE_DEBUG
	else ifeq "$(MODE)" "test"
		BUILD_MODE_MACRO = BUILD_MODE_TEST
	else ifeq "$(MODE)" "release"
		BUILD_MODE_MACRO = BUILD_MODE_RELEASE
	endif

	# Global project settings.
	ifdef GCC
		CXX = g++
		CCLD = gcc
	else
		CXX = clang++
		CCLD = clang
	endif
	CFLAGS = -Wall -Wextra \
		-Wno-unused-parameter \
		-msse4.1 -mtune=core2 \
		-DBUILD_MODE=$(BUILD_MODE_MACRO)
	ifeq "$(CXX)" "g++"
		# GCC doesn't support this.
		CFLAGS += -Wimplicit-fallthrough
	endif
	CXXFLAGS = $(CFLAGS) -std=c++14 -DGLM_FORCE_SSE41
	LDFLAGS =
	LDLIBS = -lstdc++
	ifneq "$(PLATFORM)" "msw"
		CFLAGS += -flto
		LDFLAGS += -flto
	endif
	ifeq "$(MODE)" "debug"
		CXXFLAGS += -O0 -g -DDEBUG
		LDFLAGS += -O0
	else ifeq "$(MODE)" "test"
		CXXFLAGS += -O3 -Ofast
		LDFLAGS += -O3
	else ifeq "$(MODE)" "release"
		CXXFLAGS += -O3 -Ofast
		LDFLAGS += -O4
	endif

	ifeq "$(PLATFORM)" "msw"
		ifeq "$(CXX)" "clang++"
			CFLAGS += --target=i686-w64-mingw32
		endif
		ifeq "$(CCLD)" "clang"
			CCLD = gcc
		endif
	else ifeq "$(PLATFORM)" "gnu"
	else ifeq "$(PLATFORM)" "mac"
	endif

	ifeq "$(PLATFORM)" "msw"
		ECHO_BANNER = @echo ● $(1)
		ECHO_BANNER_BOLD = @echo ● $(1) $(2)
	else
		ECHO_BANNER = @echo -e "\033[33m $(1) \033[0m"
		ECHO_BANNER_BOLD = @echo -e "\033[33m $(1)\033[47;30m $(2) \033[0m"
	endif

first: all
.PHONY: first
endif
