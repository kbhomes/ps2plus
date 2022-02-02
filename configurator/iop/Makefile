IOP_BIN = ps2plupd.irx
IOP_OBJS = ps2plus_updater.o imports.o exports.o ../common/primitive_data.o
IOP_INCS += -I../include

all: $(IOP_BIN)

clean:
	rm -f -r $(IOP_OBJS) $(IOP_OBJS_DIR) $(IOP_BIN)

include $(PS2SDK)/Defs.make
include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.iopglobal