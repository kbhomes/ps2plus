EE_BIN = ps2plman.elf
EE_OBJS = ps2plman.o ../common/primitive_data.o
EE_INCS += -I../common
EE_LIBS += -ldebug -lc -lpatches

# Add all required IRX files into the compilation
EE_LDFLAGS += ps2plman_irx.o sio2man_irx.o

ifeq ($(DEBUG),1)
EE_CFLAGS += -DDEBUG -g
EE_LDFLAGS += -g
else
EE_LDFLAGS += -s
endif

all: $(EE_BIN)

clean:
	rm -f $(EE_BIN) $(EE_OBJS)

reset:
	ps2client reset

include $(PS2SDK)/Defs.make
include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal
