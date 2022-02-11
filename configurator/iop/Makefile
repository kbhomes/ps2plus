BUILDDIR = $(ROOTDIR)/build/$(PROJECTDIR)
PROJECTDIR = $(ROOTDIR)/$(PROJECTDIR)

SRCDIRS = ./ ../common ../../shared
SRCFILES = $(foreach dir, $(SRCDIRS), $(shell find $(dir) -name '*.c'))

IOP_OBJS_DIR = $(BUILDDIR)/obj/
IOP_BIN = $(BUILDDIR)/ps2plman.irx
IOP_INCS += -I./ -I../common -I../..

IOP_OBJS = $(patsubst %.c, %.o, $(SRCFILES))
IOP_OBJS += imports.o exports.o

all: $(IOP_BIN)

clean:
	rm -f -r $(IOP_OBJS) $(IOP_OBJS_DIR) $(IOP_BIN)
	rm -rf $(BUILDDIR)

include $(PS2SDK)/Defs.make
include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.iopglobal