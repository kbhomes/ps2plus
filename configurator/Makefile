# Set this to 1 to enable debug mode
DEBUG = 1

BIN2C=bin2c
RM=rm -f

include $(PS2SDK)/Defs.make

BUILDDIR=$(ROOTDIR)/build/$(PROJECTDIR)
BUILDDIR_EE=$(BUILDDIR)/ee
BUILDDIR_IOP=$(BUILDDIR)/iop

EEFILES=$(BUILDDIR_EE)/ps2plman.elf
IRXFILES=ps2plman.irx iomanX.irx fileXio.irx freepad.irx freesio2.irx
EE_IRX_OBJS = $(addprefix $(BUILDDIR_EE)/, $(addsuffix _irx.o, $(basename $(IRXFILES))))

# Where to find the IRX files
vpath %.irx $(PS2SDK)/iop/irx/
vpath %.irx $(BUILDDIR_IOP)

# Rule to generate them
$(BUILDDIR_EE)/%_irx.o: %.irx
	mkdir -p $(BUILDDIR_EE)
	$(BIN2C) $< $(BUILDDIR_IOP)/$*_irx.c $*_irx
	$(EE_CC) -c $(BUILDDIR_IOP)/$*_irx.c -o $(BUILDDIR_EE)/$*_irx.o
	rm $(BUILDDIR_IOP)/$*_irx.c

VARIABLES=DEBUG=$(DEBUG)
TARGETS = iop builtins ee

all: $(TARGETS)
	mkdir -p $(ROOTDIR)/build/dist;
	@for file in $(EEFILES); do \
		new=`echo $${file/*\//}|tr "[:lower:]" "[:upper:]"`; \
		ps2-packer $$file $(ROOTDIR)/build/dist/$$new; \
	done;

ee:
	$(VARIABLES) $(MAKE) -C ee ROOTDIR=$(ROOTDIR) PROJECTDIR=$(PROJECTDIR)/ee

iop:
	$(VARIABLES) $(MAKE) -C iop ROOTDIR=$(ROOTDIR) PROJECTDIR=$(PROJECTDIR)/iop

clean:
	$(MAKE) -C ee clean ROOTDIR=$(ROOTDIR) PROJECTDIR=$(PROJECTDIR)/ee
	$(MAKE) -C iop clean ROOTDIR=$(ROOTDIR) PROJECTDIR=$(PROJECTDIR)/iop
	rm -rf $(BUILDDIR)

rebuild: clean all

builtins: $(EE_IRX_OBJS)

.PHONY: iop ee