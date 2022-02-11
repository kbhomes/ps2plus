.DEFAULT_GOAL := all

DEBUG = 1

BUILDDIR = $(ROOTDIR)/build/$(PROJECTDIR)
PROJECTDIR = $(ROOTDIR)/$(PROJECTDIR)

SRCDIRS = ./ ../common ../../shared
SRCFILES = $(foreach dir, $(SRCDIRS), $(shell find $(dir) -name '*.c' -o -name '*.cpp'))

EE_BIN = ps2plman.elf
EE_INCS += -I. -Ilib -I../common -I../..
EE_LIBS += -lpatches -lpad
EE_OBJS = $(addprefix $(BUILDDIR)/, $(patsubst %.c, %.o, $(patsubst %.cpp, %.o, $(SRCFILES))))

# Add all required IRX files into the compilation
EE_LDFLAGS += ps2plman_irx.o
EE_CXXFLAGS += -std=gnu++11

# Add gsKit references
EE_INCS += -I$(GSKIT)/include
EE_LIBS += -lgskit_toolkit -lgskit -ldmakit
EE_LDFLAGS += -L$(GSKIT)/lib

# Add imgui references

ifeq ($(DEBUG),1)
EE_CFLAGS += -DDEBUG -g
EE_CXXFLAGS += -DDEBUG -g
EE_LDFLAGS += -g
else
EE_LDFLAGS += -s
endif

# Use C++ compiler for all files in this compilation
$(BUILDDIR)/%.o: %.c
	mkdir -p $(BUILDDIR)/$(dir $<)
	$(EE_CXX_COMPILE) -c $< -o $@

$(BUILDDIR)/%.o: %.cpp
	mkdir -p $(BUILDDIR)/$(dir $<)
	$(EE_CXX_COMPILE) -c $< -o $@

all: $(EE_BIN)

clean:
	rm -f $(EE_BIN) $(EE_OBJS)
	rm -rf $(BUILDDIR)

reset:
	ps2client reset

include $(PS2SDK)/Defs.make
include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal_cpp
