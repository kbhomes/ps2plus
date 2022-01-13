IOP_BIN = ps2plus_updater.irx
IOP_OBJS = ps2plus_updater.o imports.o exports.o

all: $(IOP_BIN)

clean:
	rm -f -r $(IOP_OBJS) $(IOP_OBJS_DIR) $(IOP_BIN)

include $(PS2SDK)/Defs.make
include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.iopglobal