EE_BIN = ps2plus_updater.elf
EE_OBJS = ps2plus_updater.o
EE_INCS += -I../include
EE_LIBS += -ldebug -lc -lpatches

# Add all required IRX files into the compilation
EE_LDFLAGS += ps2plupd_irx.o sio2man_irx.o

ifeq ($(DEBUG),1)
EE_CFLAGS += -DDEBUG -g
EE_LDFLAGS += -g
else
EE_LDFLAGS += -s
endif

all: $(EE_BIN)

clean:
	rm -f $(EE_BIN) $(EE_OBJS)

run: $(EE_BIN)
	ps2client execee host:$(EE_BIN)

emulate: $(EE_BIN)
	"/mnt/c/Program Files (x86)/PCSX2/pcsx2.exe" --nodisc --console --elf="./$(EE_BIN)" 

reset:
	ps2client reset

include $(PS2SDK)/Defs.make
include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal
