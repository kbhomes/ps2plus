.DEFAULT_GOAL := all

DEBUG = 1

EE_BIN = ps2plman.elf
EE_INCS += -I../common -I../lib
EE_LIBS += -lpatches -lpad

EE_OBJS = main.o libps2plman.o \
	ui/gfx.o ui/pad.o \
	ui/drawing/drawing.o ui/drawing/drawing_controller.o \
	ui/widgets/widget_cursor.o ui/widgets/widget_gamepad.o ui/widgets/widget_icon.o ui/widgets/widget_misc.o

# Add common files
EE_OBJS += ../common/primitive_data.o

# Add all required IRX files into the compilation
EE_LDFLAGS += ps2plman_irx.o

# Add gsKit references
EE_INCS += -I$(GSKIT)/include
EE_LIBS += -lgskit_toolkit -lgskit -ldmakit
EE_LDFLAGS += -L$(GSKIT)/lib

# Add imgui references
EE_OBJS += ../lib/imgui/imgui.o ../lib/imgui/imgui_demo.o ../lib/imgui/imgui_draw.o ../lib/imgui/imgui_tables.o ../lib/imgui/imgui_widgets.o
EE_OBJS += ../lib/imgui/backends/imgui_impl_ps2sdk.o ../lib/imgui/backends/imgui_impl_ps2gskit.o
EE_CXXFLAGS += -std=gnu++11

ifeq ($(DEBUG),1)
EE_CFLAGS += -DDEBUG -g
EE_CXXFLAGS += -DDEBUG -g
EE_LDFLAGS += -g
else
EE_LDFLAGS += -s
endif

# Use C++ compiler for all files in this compilation
%.o: %.c
	$(EE_CXX_COMPILE) -c $< -o $@

all: $(EE_BIN)

clean:
	rm -f $(EE_BIN) $(EE_OBJS)

reset:
	ps2client reset

include $(PS2SDK)/Defs.make
include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal_cpp
