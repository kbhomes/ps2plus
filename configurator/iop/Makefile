IOP_BIN = ps2plman.irx
IOP_OBJS = \
		ps2plman.o rpc.o spi.o imports.o exports.o \
		commands/get_version.o commands/get_configuration.o commands/set_configuration.o \
		commands/disable_enable_configuration.o commands/restore_configuration_defaults.o commands/reboot_controller.o \
		../common/primitive_data.o
IOP_INCS += -I../common

all: $(IOP_BIN)

clean:
	rm -f -r $(IOP_OBJS) $(IOP_OBJS_DIR) $(IOP_BIN)

include $(PS2SDK)/Defs.make
include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.iopglobal