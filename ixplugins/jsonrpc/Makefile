#
#	Makefile for jsonrpc ixplugins
#

CROSS       := 

CC          := $(CROSS)gcc
CXX         := $(CROSS)g++
LD          := $(CROSS)ld
STRIP       := $(CROSS)strip --strip-debug

SILENT      := @

###################################################################################
# Dependencies
###################################################################################
CFLAGS		+= -I./include/ -I./modules/

# libjannson
LIBJANSSON_HEADER_PATH  :=
CFLAGS		+= -I$(LIBJANSSON_HEADER_PATH)

###################################################################################
# Compile Options
###################################################################################
CFLAGS		+= -O3 -Wall -fomit-frame-pointer -MMD -Wno-literal-suffix
CFLAGS		+= -std=c++0x
CXXFLAGS	+= $(CFLAGS)  -fPIC -Wno-literal-suffix

TARGETS		:= jsonrpc.so
SOURCES     := $(shell find $(SOURCE_DIRS) -maxdepth 5 -name "*.cpp")
CXX_SRC		:= $(SOURCES)

OBJS		= $(C_SRC:.c=.o) $(CXX_SRC:.cpp=.o)
DEPFILES	= $(OBJS:.o=.d)
CLEANLIST	= $(TARGETS) $(OBJS) $(DEPFILES)

# implicit rules
%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(TARGETS): $(OBJS)
	$(SILENT)$(CXX) -shared $(OBJS) -o $(TARGETS) $(CXXFLAGS)

## targets ----------
all: $(TARGETS)

.PHONY: clean
clean:
	rm -rf $(CLEANLIST)
