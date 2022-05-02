.DEFAULT_GOAL := all

DEBUG = 1

BUILDDIR = $(ROOTDIR)/build/$(PROJECTDIR)

SRCDIRS = ./ ../common ../../shared
SRCFILES = $(foreach dir, $(SRCDIRS), $(shell find $(dir) -name '*.c' -o -name '*.cpp'))

RESOURCEDIR = ./resources
RESOURCEFILES = $(shell find $(RESOURCEDIR) -type f)

IRX_OBJS = \
	$(ROOTDIR)/build/$(PROJECTDIR)/ps2plman_irx.o \
	$(ROOTDIR)/build/$(PROJECTDIR)/iomanX_irx.o \
	$(ROOTDIR)/build/$(PROJECTDIR)/freepad_irx.o \
	$(ROOTDIR)/build/$(PROJECTDIR)/freesio2_irx.o \
	$(ROOTDIR)/build/$(PROJECTDIR)/fileXio_irx.o 

EE_BIN = $(BUILDDIR)/ps2plman.elf
EE_INCS += -I./ -Ilib -I../common -I../..
EE_LIBS += -lpatches -lpad -lfileXio
EE_CXXFLAGS += -std=gnu++17

EE_OBJS = $(addprefix $(BUILDDIR)/, $(patsubst %.c, %.o, $(patsubst %.cpp, %.o, $(SRCFILES))))

# Add all resource files into the compilation
EE_OBJS += $(addprefix $(BUILDDIR)/, $(addsuffix .o, $(RESOURCEFILES)))

# Add all required IRX files into the compilation
EE_OBJS += $(IRX_OBJS)


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

# Include resources as binary identifiers
$(BUILDDIR)/$(RESOURCEDIR)/%.o: $(RESOURCEDIR)/%
	mkdir -p $(BUILDDIR)/$(dir $<)
	$(EE_LD) --relocatable --format binary -o $@ $<

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
