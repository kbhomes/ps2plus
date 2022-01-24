#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/Arduino_Blink.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/Arduino_Blink.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=lib/PS2Plus/commands/button_inclusions_command.c lib/PS2Plus/commands/command.c lib/PS2Plus/commands/configure_analog_response_command.c lib/PS2Plus/commands/device_descriptor_46_command.c lib/PS2Plus/commands/device_descriptor_47_command.c lib/PS2Plus/commands/device_descriptor_4c_command.c lib/PS2Plus/commands/initialize_pressure_sensor_command.c lib/PS2Plus/commands/main_polling_command.c lib/PS2Plus/commands/map_motors_command.c lib/PS2Plus/commands/packet.c lib/PS2Plus/commands/status_info_command.c lib/PS2Plus/commands/switch_digital_analog_mode_command.c lib/PS2Plus/controller/input.c lib/PS2Plus/controller/state.c lib/PS2Plus/platforms/arduino/arduino_controller.c lib/PS2Plus/platforms/arduino/arduino_memory.c lib/PS2Plus/platforms/arduino/arduino_platform.c lib/PS2Plus/platforms/arduino/arduino_spi_playstation.c lib/PS2Plus/platforms/arduino/arduino_timing.c lib/PS2Plus/platforms/arduino/arduino_uart_serial.c lib/PS2Plus/platforms/native/native.c lib/PS2Plus/platforms/pic16f/pic16f_platform.c lib/PS2Plus/platforms/pic16f/pic16f_controller.c lib/PS2Plus/platforms/pic16f/pic16f_memory.c lib/PS2Plus/platforms/pic16f/pic16f_spi_playstation.c lib/PS2Plus/platforms/pic16f/pic16f_timing.c lib/PS2Plus/platforms/pic16f/pic16f_uart_serial.c lib/PS2Plus/utilities/debounced_bool.c lib/PS2Plus/utilities/latching_bool.c src/main.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/lib/PS2Plus/commands/button_inclusions_command.p1 ${OBJECTDIR}/lib/PS2Plus/commands/command.p1 ${OBJECTDIR}/lib/PS2Plus/commands/configure_analog_response_command.p1 ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_46_command.p1 ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_47_command.p1 ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_4c_command.p1 ${OBJECTDIR}/lib/PS2Plus/commands/initialize_pressure_sensor_command.p1 ${OBJECTDIR}/lib/PS2Plus/commands/main_polling_command.p1 ${OBJECTDIR}/lib/PS2Plus/commands/map_motors_command.p1 ${OBJECTDIR}/lib/PS2Plus/commands/packet.p1 ${OBJECTDIR}/lib/PS2Plus/commands/status_info_command.p1 ${OBJECTDIR}/lib/PS2Plus/commands/switch_digital_analog_mode_command.p1 ${OBJECTDIR}/lib/PS2Plus/controller/input.p1 ${OBJECTDIR}/lib/PS2Plus/controller/state.p1 ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_controller.p1 ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_memory.p1 ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_platform.p1 ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_spi_playstation.p1 ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_timing.p1 ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_uart_serial.p1 ${OBJECTDIR}/lib/PS2Plus/platforms/native/native.p1 ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_platform.p1 ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_controller.p1 ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_memory.p1 ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_spi_playstation.p1 ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_timing.p1 ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_uart_serial.p1 ${OBJECTDIR}/lib/PS2Plus/utilities/debounced_bool.p1 ${OBJECTDIR}/lib/PS2Plus/utilities/latching_bool.p1 ${OBJECTDIR}/src/main.p1
POSSIBLE_DEPFILES=${OBJECTDIR}/lib/PS2Plus/commands/button_inclusions_command.p1.d ${OBJECTDIR}/lib/PS2Plus/commands/command.p1.d ${OBJECTDIR}/lib/PS2Plus/commands/configure_analog_response_command.p1.d ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_46_command.p1.d ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_47_command.p1.d ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_4c_command.p1.d ${OBJECTDIR}/lib/PS2Plus/commands/initialize_pressure_sensor_command.p1.d ${OBJECTDIR}/lib/PS2Plus/commands/main_polling_command.p1.d ${OBJECTDIR}/lib/PS2Plus/commands/map_motors_command.p1.d ${OBJECTDIR}/lib/PS2Plus/commands/packet.p1.d ${OBJECTDIR}/lib/PS2Plus/commands/status_info_command.p1.d ${OBJECTDIR}/lib/PS2Plus/commands/switch_digital_analog_mode_command.p1.d ${OBJECTDIR}/lib/PS2Plus/controller/input.p1.d ${OBJECTDIR}/lib/PS2Plus/controller/state.p1.d ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_controller.p1.d ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_memory.p1.d ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_platform.p1.d ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_spi_playstation.p1.d ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_timing.p1.d ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_uart_serial.p1.d ${OBJECTDIR}/lib/PS2Plus/platforms/native/native.p1.d ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_platform.p1.d ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_controller.p1.d ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_memory.p1.d ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_spi_playstation.p1.d ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_timing.p1.d ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_uart_serial.p1.d ${OBJECTDIR}/lib/PS2Plus/utilities/debounced_bool.p1.d ${OBJECTDIR}/lib/PS2Plus/utilities/latching_bool.p1.d ${OBJECTDIR}/src/main.p1.d

# Object Files
OBJECTFILES=${OBJECTDIR}/lib/PS2Plus/commands/button_inclusions_command.p1 ${OBJECTDIR}/lib/PS2Plus/commands/command.p1 ${OBJECTDIR}/lib/PS2Plus/commands/configure_analog_response_command.p1 ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_46_command.p1 ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_47_command.p1 ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_4c_command.p1 ${OBJECTDIR}/lib/PS2Plus/commands/initialize_pressure_sensor_command.p1 ${OBJECTDIR}/lib/PS2Plus/commands/main_polling_command.p1 ${OBJECTDIR}/lib/PS2Plus/commands/map_motors_command.p1 ${OBJECTDIR}/lib/PS2Plus/commands/packet.p1 ${OBJECTDIR}/lib/PS2Plus/commands/status_info_command.p1 ${OBJECTDIR}/lib/PS2Plus/commands/switch_digital_analog_mode_command.p1 ${OBJECTDIR}/lib/PS2Plus/controller/input.p1 ${OBJECTDIR}/lib/PS2Plus/controller/state.p1 ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_controller.p1 ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_memory.p1 ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_platform.p1 ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_spi_playstation.p1 ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_timing.p1 ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_uart_serial.p1 ${OBJECTDIR}/lib/PS2Plus/platforms/native/native.p1 ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_platform.p1 ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_controller.p1 ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_memory.p1 ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_spi_playstation.p1 ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_timing.p1 ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_uart_serial.p1 ${OBJECTDIR}/lib/PS2Plus/utilities/debounced_bool.p1 ${OBJECTDIR}/lib/PS2Plus/utilities/latching_bool.p1 ${OBJECTDIR}/src/main.p1

# Source Files
SOURCEFILES=lib/PS2Plus/commands/button_inclusions_command.c lib/PS2Plus/commands/command.c lib/PS2Plus/commands/configure_analog_response_command.c lib/PS2Plus/commands/device_descriptor_46_command.c lib/PS2Plus/commands/device_descriptor_47_command.c lib/PS2Plus/commands/device_descriptor_4c_command.c lib/PS2Plus/commands/initialize_pressure_sensor_command.c lib/PS2Plus/commands/main_polling_command.c lib/PS2Plus/commands/map_motors_command.c lib/PS2Plus/commands/packet.c lib/PS2Plus/commands/status_info_command.c lib/PS2Plus/commands/switch_digital_analog_mode_command.c lib/PS2Plus/controller/input.c lib/PS2Plus/controller/state.c lib/PS2Plus/platforms/arduino/arduino_controller.c lib/PS2Plus/platforms/arduino/arduino_memory.c lib/PS2Plus/platforms/arduino/arduino_platform.c lib/PS2Plus/platforms/arduino/arduino_spi_playstation.c lib/PS2Plus/platforms/arduino/arduino_timing.c lib/PS2Plus/platforms/arduino/arduino_uart_serial.c lib/PS2Plus/platforms/native/native.c lib/PS2Plus/platforms/pic16f/pic16f_platform.c lib/PS2Plus/platforms/pic16f/pic16f_controller.c lib/PS2Plus/platforms/pic16f/pic16f_memory.c lib/PS2Plus/platforms/pic16f/pic16f_spi_playstation.c lib/PS2Plus/platforms/pic16f/pic16f_timing.c lib/PS2Plus/platforms/pic16f/pic16f_uart_serial.c lib/PS2Plus/utilities/debounced_bool.c lib/PS2Plus/utilities/latching_bool.c src/main.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

# The following macros may be used in the pre and post step lines
_/_=\\
ShExtension=.bat
Device=PIC18F46K42
ProjectDir="C:\Users\Sajid\Documents\PlatformIO\Projects\Arduino Blink"
ProjectName=PS2Plus
ConfName=default
ImagePath="${DISTDIR}\Arduino_Blink.${IMAGE_TYPE}.${OUTPUT_SUFFIX}"
ImageDir="${DISTDIR}"
ImageName="Arduino_Blink.${IMAGE_TYPE}.${OUTPUT_SUFFIX}"
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IsDebug="true"
else
IsDebug="false"
endif

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/Arduino_Blink.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
	@echo "--------------------------------------"
	@echo "User defined post-build step: [copy ${ImagePath} "D:\"]"
	@copy ${ImagePath} "D:\"
	@echo "--------------------------------------"

MP_PROCESSOR_OPTION=18F46K42
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/lib/PS2Plus/commands/button_inclusions_command.p1: lib/PS2Plus/commands/button_inclusions_command.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/commands" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/button_inclusions_command.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/button_inclusions_command.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/commands/button_inclusions_command.p1 lib/PS2Plus/commands/button_inclusions_command.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/commands/button_inclusions_command.d ${OBJECTDIR}/lib/PS2Plus/commands/button_inclusions_command.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/commands/button_inclusions_command.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/commands/command.p1: lib/PS2Plus/commands/command.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/commands" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/command.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/command.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/commands/command.p1 lib/PS2Plus/commands/command.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/commands/command.d ${OBJECTDIR}/lib/PS2Plus/commands/command.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/commands/command.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/commands/configure_analog_response_command.p1: lib/PS2Plus/commands/configure_analog_response_command.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/commands" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/configure_analog_response_command.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/configure_analog_response_command.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/commands/configure_analog_response_command.p1 lib/PS2Plus/commands/configure_analog_response_command.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/commands/configure_analog_response_command.d ${OBJECTDIR}/lib/PS2Plus/commands/configure_analog_response_command.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/commands/configure_analog_response_command.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_46_command.p1: lib/PS2Plus/commands/device_descriptor_46_command.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/commands" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_46_command.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_46_command.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_46_command.p1 lib/PS2Plus/commands/device_descriptor_46_command.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_46_command.d ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_46_command.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_46_command.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_47_command.p1: lib/PS2Plus/commands/device_descriptor_47_command.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/commands" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_47_command.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_47_command.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_47_command.p1 lib/PS2Plus/commands/device_descriptor_47_command.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_47_command.d ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_47_command.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_47_command.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_4c_command.p1: lib/PS2Plus/commands/device_descriptor_4c_command.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/commands" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_4c_command.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_4c_command.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_4c_command.p1 lib/PS2Plus/commands/device_descriptor_4c_command.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_4c_command.d ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_4c_command.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_4c_command.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/commands/initialize_pressure_sensor_command.p1: lib/PS2Plus/commands/initialize_pressure_sensor_command.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/commands" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/initialize_pressure_sensor_command.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/initialize_pressure_sensor_command.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/commands/initialize_pressure_sensor_command.p1 lib/PS2Plus/commands/initialize_pressure_sensor_command.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/commands/initialize_pressure_sensor_command.d ${OBJECTDIR}/lib/PS2Plus/commands/initialize_pressure_sensor_command.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/commands/initialize_pressure_sensor_command.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/commands/main_polling_command.p1: lib/PS2Plus/commands/main_polling_command.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/commands" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/main_polling_command.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/main_polling_command.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/commands/main_polling_command.p1 lib/PS2Plus/commands/main_polling_command.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/commands/main_polling_command.d ${OBJECTDIR}/lib/PS2Plus/commands/main_polling_command.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/commands/main_polling_command.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/commands/map_motors_command.p1: lib/PS2Plus/commands/map_motors_command.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/commands" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/map_motors_command.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/map_motors_command.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/commands/map_motors_command.p1 lib/PS2Plus/commands/map_motors_command.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/commands/map_motors_command.d ${OBJECTDIR}/lib/PS2Plus/commands/map_motors_command.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/commands/map_motors_command.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/commands/packet.p1: lib/PS2Plus/commands/packet.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/commands" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/packet.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/packet.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/commands/packet.p1 lib/PS2Plus/commands/packet.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/commands/packet.d ${OBJECTDIR}/lib/PS2Plus/commands/packet.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/commands/packet.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/commands/status_info_command.p1: lib/PS2Plus/commands/status_info_command.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/commands" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/status_info_command.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/status_info_command.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/commands/status_info_command.p1 lib/PS2Plus/commands/status_info_command.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/commands/status_info_command.d ${OBJECTDIR}/lib/PS2Plus/commands/status_info_command.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/commands/status_info_command.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/commands/switch_digital_analog_mode_command.p1: lib/PS2Plus/commands/switch_digital_analog_mode_command.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/commands" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/switch_digital_analog_mode_command.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/switch_digital_analog_mode_command.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/commands/switch_digital_analog_mode_command.p1 lib/PS2Plus/commands/switch_digital_analog_mode_command.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/commands/switch_digital_analog_mode_command.d ${OBJECTDIR}/lib/PS2Plus/commands/switch_digital_analog_mode_command.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/commands/switch_digital_analog_mode_command.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/controller/input.p1: lib/PS2Plus/controller/input.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/controller" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/controller/input.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/controller/input.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/controller/input.p1 lib/PS2Plus/controller/input.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/controller/input.d ${OBJECTDIR}/lib/PS2Plus/controller/input.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/controller/input.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/controller/state.p1: lib/PS2Plus/controller/state.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/controller" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/controller/state.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/controller/state.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/controller/state.p1 lib/PS2Plus/controller/state.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/controller/state.d ${OBJECTDIR}/lib/PS2Plus/controller/state.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/controller/state.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_controller.p1: lib/PS2Plus/platforms/arduino/arduino_controller.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/platforms/arduino" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_controller.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_controller.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_controller.p1 lib/PS2Plus/platforms/arduino/arduino_controller.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_controller.d ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_controller.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_controller.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_memory.p1: lib/PS2Plus/platforms/arduino/arduino_memory.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/platforms/arduino" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_memory.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_memory.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_memory.p1 lib/PS2Plus/platforms/arduino/arduino_memory.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_memory.d ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_memory.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_memory.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_platform.p1: lib/PS2Plus/platforms/arduino/arduino_platform.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/platforms/arduino" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_platform.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_platform.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_platform.p1 lib/PS2Plus/platforms/arduino/arduino_platform.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_platform.d ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_platform.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_platform.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_spi_playstation.p1: lib/PS2Plus/platforms/arduino/arduino_spi_playstation.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/platforms/arduino" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_spi_playstation.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_spi_playstation.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_spi_playstation.p1 lib/PS2Plus/platforms/arduino/arduino_spi_playstation.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_spi_playstation.d ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_spi_playstation.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_spi_playstation.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_timing.p1: lib/PS2Plus/platforms/arduino/arduino_timing.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/platforms/arduino" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_timing.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_timing.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_timing.p1 lib/PS2Plus/platforms/arduino/arduino_timing.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_timing.d ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_timing.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_timing.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_uart_serial.p1: lib/PS2Plus/platforms/arduino/arduino_uart_serial.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/platforms/arduino" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_uart_serial.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_uart_serial.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_uart_serial.p1 lib/PS2Plus/platforms/arduino/arduino_uart_serial.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_uart_serial.d ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_uart_serial.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_uart_serial.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/platforms/native/native.p1: lib/PS2Plus/platforms/native/native.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/platforms/native" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/native/native.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/native/native.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/platforms/native/native.p1 lib/PS2Plus/platforms/native/native.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/platforms/native/native.d ${OBJECTDIR}/lib/PS2Plus/platforms/native/native.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/platforms/native/native.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_platform.p1: lib/PS2Plus/platforms/pic16f/pic16f_platform.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/platforms/pic16f" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_platform.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_platform.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_platform.p1 lib/PS2Plus/platforms/pic16f/pic16f_platform.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_platform.d ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_platform.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_platform.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_controller.p1: lib/PS2Plus/platforms/pic16f/pic16f_controller.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/platforms/pic16f" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_controller.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_controller.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_controller.p1 lib/PS2Plus/platforms/pic16f/pic16f_controller.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_controller.d ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_controller.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_controller.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_memory.p1: lib/PS2Plus/platforms/pic16f/pic16f_memory.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/platforms/pic16f" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_memory.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_memory.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_memory.p1 lib/PS2Plus/platforms/pic16f/pic16f_memory.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_memory.d ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_memory.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_memory.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_spi_playstation.p1: lib/PS2Plus/platforms/pic16f/pic16f_spi_playstation.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/platforms/pic16f" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_spi_playstation.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_spi_playstation.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_spi_playstation.p1 lib/PS2Plus/platforms/pic16f/pic16f_spi_playstation.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_spi_playstation.d ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_spi_playstation.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_spi_playstation.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_timing.p1: lib/PS2Plus/platforms/pic16f/pic16f_timing.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/platforms/pic16f" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_timing.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_timing.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_timing.p1 lib/PS2Plus/platforms/pic16f/pic16f_timing.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_timing.d ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_timing.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_timing.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_uart_serial.p1: lib/PS2Plus/platforms/pic16f/pic16f_uart_serial.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/platforms/pic16f" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_uart_serial.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_uart_serial.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_uart_serial.p1 lib/PS2Plus/platforms/pic16f/pic16f_uart_serial.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_uart_serial.d ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_uart_serial.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_uart_serial.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/utilities/debounced_bool.p1: lib/PS2Plus/utilities/debounced_bool.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/utilities" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/utilities/debounced_bool.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/utilities/debounced_bool.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/utilities/debounced_bool.p1 lib/PS2Plus/utilities/debounced_bool.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/utilities/debounced_bool.d ${OBJECTDIR}/lib/PS2Plus/utilities/debounced_bool.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/utilities/debounced_bool.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/utilities/latching_bool.p1: lib/PS2Plus/utilities/latching_bool.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/utilities" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/utilities/latching_bool.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/utilities/latching_bool.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/utilities/latching_bool.p1 lib/PS2Plus/utilities/latching_bool.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/utilities/latching_bool.d ${OBJECTDIR}/lib/PS2Plus/utilities/latching_bool.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/utilities/latching_bool.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/main.p1: src/main.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/main.p1.d 
	@${RM} ${OBJECTDIR}/src/main.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/main.p1 src/main.c 
	@-${MV} ${OBJECTDIR}/src/main.d ${OBJECTDIR}/src/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/lib/PS2Plus/commands/button_inclusions_command.p1: lib/PS2Plus/commands/button_inclusions_command.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/commands" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/button_inclusions_command.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/button_inclusions_command.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/commands/button_inclusions_command.p1 lib/PS2Plus/commands/button_inclusions_command.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/commands/button_inclusions_command.d ${OBJECTDIR}/lib/PS2Plus/commands/button_inclusions_command.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/commands/button_inclusions_command.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/commands/command.p1: lib/PS2Plus/commands/command.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/commands" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/command.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/command.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/commands/command.p1 lib/PS2Plus/commands/command.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/commands/command.d ${OBJECTDIR}/lib/PS2Plus/commands/command.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/commands/command.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/commands/configure_analog_response_command.p1: lib/PS2Plus/commands/configure_analog_response_command.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/commands" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/configure_analog_response_command.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/configure_analog_response_command.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/commands/configure_analog_response_command.p1 lib/PS2Plus/commands/configure_analog_response_command.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/commands/configure_analog_response_command.d ${OBJECTDIR}/lib/PS2Plus/commands/configure_analog_response_command.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/commands/configure_analog_response_command.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_46_command.p1: lib/PS2Plus/commands/device_descriptor_46_command.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/commands" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_46_command.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_46_command.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_46_command.p1 lib/PS2Plus/commands/device_descriptor_46_command.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_46_command.d ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_46_command.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_46_command.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_47_command.p1: lib/PS2Plus/commands/device_descriptor_47_command.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/commands" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_47_command.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_47_command.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_47_command.p1 lib/PS2Plus/commands/device_descriptor_47_command.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_47_command.d ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_47_command.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_47_command.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_4c_command.p1: lib/PS2Plus/commands/device_descriptor_4c_command.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/commands" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_4c_command.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_4c_command.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_4c_command.p1 lib/PS2Plus/commands/device_descriptor_4c_command.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_4c_command.d ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_4c_command.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/commands/device_descriptor_4c_command.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/commands/initialize_pressure_sensor_command.p1: lib/PS2Plus/commands/initialize_pressure_sensor_command.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/commands" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/initialize_pressure_sensor_command.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/initialize_pressure_sensor_command.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/commands/initialize_pressure_sensor_command.p1 lib/PS2Plus/commands/initialize_pressure_sensor_command.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/commands/initialize_pressure_sensor_command.d ${OBJECTDIR}/lib/PS2Plus/commands/initialize_pressure_sensor_command.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/commands/initialize_pressure_sensor_command.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/commands/main_polling_command.p1: lib/PS2Plus/commands/main_polling_command.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/commands" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/main_polling_command.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/main_polling_command.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/commands/main_polling_command.p1 lib/PS2Plus/commands/main_polling_command.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/commands/main_polling_command.d ${OBJECTDIR}/lib/PS2Plus/commands/main_polling_command.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/commands/main_polling_command.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/commands/map_motors_command.p1: lib/PS2Plus/commands/map_motors_command.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/commands" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/map_motors_command.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/map_motors_command.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/commands/map_motors_command.p1 lib/PS2Plus/commands/map_motors_command.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/commands/map_motors_command.d ${OBJECTDIR}/lib/PS2Plus/commands/map_motors_command.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/commands/map_motors_command.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/commands/packet.p1: lib/PS2Plus/commands/packet.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/commands" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/packet.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/packet.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/commands/packet.p1 lib/PS2Plus/commands/packet.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/commands/packet.d ${OBJECTDIR}/lib/PS2Plus/commands/packet.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/commands/packet.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/commands/status_info_command.p1: lib/PS2Plus/commands/status_info_command.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/commands" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/status_info_command.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/status_info_command.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/commands/status_info_command.p1 lib/PS2Plus/commands/status_info_command.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/commands/status_info_command.d ${OBJECTDIR}/lib/PS2Plus/commands/status_info_command.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/commands/status_info_command.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/commands/switch_digital_analog_mode_command.p1: lib/PS2Plus/commands/switch_digital_analog_mode_command.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/commands" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/switch_digital_analog_mode_command.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/commands/switch_digital_analog_mode_command.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/commands/switch_digital_analog_mode_command.p1 lib/PS2Plus/commands/switch_digital_analog_mode_command.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/commands/switch_digital_analog_mode_command.d ${OBJECTDIR}/lib/PS2Plus/commands/switch_digital_analog_mode_command.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/commands/switch_digital_analog_mode_command.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/controller/input.p1: lib/PS2Plus/controller/input.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/controller" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/controller/input.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/controller/input.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/controller/input.p1 lib/PS2Plus/controller/input.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/controller/input.d ${OBJECTDIR}/lib/PS2Plus/controller/input.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/controller/input.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/controller/state.p1: lib/PS2Plus/controller/state.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/controller" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/controller/state.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/controller/state.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/controller/state.p1 lib/PS2Plus/controller/state.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/controller/state.d ${OBJECTDIR}/lib/PS2Plus/controller/state.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/controller/state.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_controller.p1: lib/PS2Plus/platforms/arduino/arduino_controller.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/platforms/arduino" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_controller.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_controller.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_controller.p1 lib/PS2Plus/platforms/arduino/arduino_controller.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_controller.d ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_controller.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_controller.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_memory.p1: lib/PS2Plus/platforms/arduino/arduino_memory.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/platforms/arduino" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_memory.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_memory.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_memory.p1 lib/PS2Plus/platforms/arduino/arduino_memory.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_memory.d ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_memory.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_memory.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_platform.p1: lib/PS2Plus/platforms/arduino/arduino_platform.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/platforms/arduino" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_platform.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_platform.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_platform.p1 lib/PS2Plus/platforms/arduino/arduino_platform.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_platform.d ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_platform.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_platform.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_spi_playstation.p1: lib/PS2Plus/platforms/arduino/arduino_spi_playstation.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/platforms/arduino" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_spi_playstation.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_spi_playstation.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_spi_playstation.p1 lib/PS2Plus/platforms/arduino/arduino_spi_playstation.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_spi_playstation.d ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_spi_playstation.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_spi_playstation.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_timing.p1: lib/PS2Plus/platforms/arduino/arduino_timing.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/platforms/arduino" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_timing.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_timing.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_timing.p1 lib/PS2Plus/platforms/arduino/arduino_timing.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_timing.d ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_timing.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_timing.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_uart_serial.p1: lib/PS2Plus/platforms/arduino/arduino_uart_serial.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/platforms/arduino" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_uart_serial.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_uart_serial.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_uart_serial.p1 lib/PS2Plus/platforms/arduino/arduino_uart_serial.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_uart_serial.d ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_uart_serial.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/platforms/arduino/arduino_uart_serial.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/platforms/native/native.p1: lib/PS2Plus/platforms/native/native.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/platforms/native" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/native/native.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/native/native.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/platforms/native/native.p1 lib/PS2Plus/platforms/native/native.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/platforms/native/native.d ${OBJECTDIR}/lib/PS2Plus/platforms/native/native.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/platforms/native/native.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_platform.p1: lib/PS2Plus/platforms/pic16f/pic16f_platform.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/platforms/pic16f" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_platform.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_platform.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_platform.p1 lib/PS2Plus/platforms/pic16f/pic16f_platform.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_platform.d ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_platform.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_platform.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_controller.p1: lib/PS2Plus/platforms/pic16f/pic16f_controller.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/platforms/pic16f" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_controller.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_controller.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_controller.p1 lib/PS2Plus/platforms/pic16f/pic16f_controller.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_controller.d ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_controller.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_controller.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_memory.p1: lib/PS2Plus/platforms/pic16f/pic16f_memory.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/platforms/pic16f" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_memory.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_memory.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_memory.p1 lib/PS2Plus/platforms/pic16f/pic16f_memory.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_memory.d ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_memory.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_memory.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_spi_playstation.p1: lib/PS2Plus/platforms/pic16f/pic16f_spi_playstation.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/platforms/pic16f" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_spi_playstation.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_spi_playstation.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_spi_playstation.p1 lib/PS2Plus/platforms/pic16f/pic16f_spi_playstation.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_spi_playstation.d ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_spi_playstation.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_spi_playstation.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_timing.p1: lib/PS2Plus/platforms/pic16f/pic16f_timing.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/platforms/pic16f" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_timing.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_timing.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_timing.p1 lib/PS2Plus/platforms/pic16f/pic16f_timing.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_timing.d ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_timing.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_timing.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_uart_serial.p1: lib/PS2Plus/platforms/pic16f/pic16f_uart_serial.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/platforms/pic16f" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_uart_serial.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_uart_serial.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_uart_serial.p1 lib/PS2Plus/platforms/pic16f/pic16f_uart_serial.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_uart_serial.d ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_uart_serial.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/platforms/pic16f/pic16f_uart_serial.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/utilities/debounced_bool.p1: lib/PS2Plus/utilities/debounced_bool.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/utilities" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/utilities/debounced_bool.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/utilities/debounced_bool.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/utilities/debounced_bool.p1 lib/PS2Plus/utilities/debounced_bool.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/utilities/debounced_bool.d ${OBJECTDIR}/lib/PS2Plus/utilities/debounced_bool.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/utilities/debounced_bool.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lib/PS2Plus/utilities/latching_bool.p1: lib/PS2Plus/utilities/latching_bool.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/lib/PS2Plus/utilities" 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/utilities/latching_bool.p1.d 
	@${RM} ${OBJECTDIR}/lib/PS2Plus/utilities/latching_bool.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/lib/PS2Plus/utilities/latching_bool.p1 lib/PS2Plus/utilities/latching_bool.c 
	@-${MV} ${OBJECTDIR}/lib/PS2Plus/utilities/latching_bool.d ${OBJECTDIR}/lib/PS2Plus/utilities/latching_bool.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lib/PS2Plus/utilities/latching_bool.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/main.p1: src/main.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/main.p1.d 
	@${RM} ${OBJECTDIR}/src/main.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/main.p1 src/main.c 
	@-${MV} ${OBJECTDIR}/src/main.d ${OBJECTDIR}/src/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/Arduino_Blink.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/Arduino_Blink.${IMAGE_TYPE}.map  -D__DEBUG=1  -mdebugger=none  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto        $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/Arduino_Blink.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	@${RM} ${DISTDIR}/Arduino_Blink.${IMAGE_TYPE}.hex 
	
else
${DISTDIR}/Arduino_Blink.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/Arduino_Blink.${IMAGE_TYPE}.map  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DPLATFORM_PIC16F -xassembler-with-cpp -I"lib/PS2Plus" -mwarn=-3 -Wa,-a -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/Arduino_Blink.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
