# Set this to 1 to enable debug mode
DEBUG = 1

BIN2C=bin2c
RM=rm -f

include $(PS2SDK)/Defs.make

EEFILES=ee/ps2plman.elf
IRXFILES=ps2plman.irx
EE_IRX_OBJS = $(addprefix ee/, $(addsuffix _irx.o, $(basename $(IRXFILES))))

# Where to find the IRX files
vpath %.irx $(PS2SDK)/iop/irx/
vpath %.irx iop/

# Rule to generate them
ee/%_irx.o: %.irx
	$(BIN2C) $< $*_irx.c $*_irx
	$(EE_CC) -c $*_irx.c -o ee/$*_irx.o
	rm $*_irx.c

VARIABLES=DEBUG=$(DEBUG)
TARGETS = iop builtins ee

all: $(TARGETS)
	mkdir -p bin;
	@for file in $(EEFILES); do \
		new=`echo $${file/*\//}|tr "[:lower:]" "[:upper:]"`; \
		ps2-packer $$file bin/$$new; \
	done;

ee:
	$(VARIABLES) $(MAKE) -C ee

iop:
	$(VARIABLES) $(MAKE) -C iop

clean:
	$(MAKE) -C ee clean
	$(MAKE) -C iop clean
	@rm -f ee/*_irx.o bin/*.ELF bin/*.IRX

rebuild: clean all

builtins: $(EE_IRX_OBJS)

.PHONY: iop ee