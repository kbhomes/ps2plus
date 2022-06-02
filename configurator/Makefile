# Set this to 1 to enable debug mode
DEBUG = 1

BIN2C=bin2c
RM=rm -f

include $(PS2SDK)/Defs.make

BUILDDIR=$(ROOTDIR)/build/$(PROJECTDIR)
BUILDDIR_EE=$(BUILDDIR)/ee
BUILDDIR_IOP=$(BUILDDIR)/iop

EE_OUT=$(BUILDDIR_EE)/ps2plman.elf
EE_DIST=$(ROOTDIR)/$(DISTDIR)/ps2plus-configurator-$(VERSION).elf

IRXFILES=iomanX.irx fileXio.irx freepad.irx freesio2.irx
EE_IRX_OBJS = $(addprefix $(BUILDDIR_EE)/, $(addsuffix _irx.o, $(basename $(IRXFILES))))
EE_IRX_OBJS += ps2plman_irx.o

# Where to find the IRX files
vpath %.irx $(PS2SDK)/iop/irx/

VARIABLES=DEBUG=$(DEBUG)
TARGETS = iop builtins ee

all: $(TARGETS)
	mkdir -p $(ROOTDIR)/$(DISTDIR);
	ps2-packer $(EE_OUT) $(EE_DIST);

ee:
	mkdir -p $(BUILDDIR_EE)
	$(VARIABLES) $(MAKE) -C ee ROOTDIR=$(ROOTDIR) PROJECTDIR=$(PROJECTDIR)/ee

iop:
	mkdir -p $(BUILDDIR_IOP)
	$(VARIABLES) $(MAKE) -C iop ROOTDIR=$(ROOTDIR) PROJECTDIR=$(PROJECTDIR)/iop

clean:
	$(MAKE) -C ee clean ROOTDIR=$(ROOTDIR) PROJECTDIR=$(PROJECTDIR)/ee
	$(MAKE) -C iop clean ROOTDIR=$(ROOTDIR) PROJECTDIR=$(PROJECTDIR)/iop
	rm -rf $(BUILDDIR)

rebuild: clean all

builtins: $(EE_IRX_OBJS)

$(BUILDDIR_EE)/%_irx.o: %.irx
	mkdir -p $(BUILDDIR_EE)
	$(BIN2C) $< $(BUILDDIR_IOP)/$*_irx.c $*_irx
	$(EE_CC) -c $(BUILDDIR_IOP)/$*_irx.c -o $(BUILDDIR_EE)/$*_irx.o
	rm $(BUILDDIR_IOP)/$*_irx.c

ps2plman_irx.o: iop
	mkdir -p $(BUILDDIR_EE)
	$(BIN2C) $(BUILDDIR_IOP)/ps2plman.irx $(BUILDDIR_IOP)/ps2plman_irx.c ps2plman_irx
	$(EE_CC) -c $(BUILDDIR_IOP)/ps2plman_irx.c -o $(BUILDDIR_EE)/ps2plman_irx.o
	rm $(BUILDDIR_IOP)/ps2plman_irx.c

.PHONY: iop ee