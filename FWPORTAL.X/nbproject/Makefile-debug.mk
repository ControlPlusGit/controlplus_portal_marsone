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
ifeq "$(wildcard nbproject/Makefile-local-debug.mk)" "nbproject/Makefile-local-debug.mk"
include nbproject/Makefile-local-debug.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=debug
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/FWPORTAL.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/FWPORTAL.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=
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
SOURCEFILES_QUOTED_IF_SPACED=src/ajuste_cap.c src/appl_commands.c src/appl_commands_table.c src/as3993.c src/bootloadable.c src/cancelas.c src/config_i2c3.c src/empilhadeira.c src/eth.c src/exclusao.c src/FSM_DataHora.c src/gen2.c src/global.c src/i2c.c src/logger.c src/main.c src/mem_i2c_24LC256.c src/onewire.c src/perifericos.c src/platform.c src/portal.c src/rtc.c src/setup_usb.c src/spi_driver.c src/stream_dispatcher.c src/system_clock.c src/tags.c src/timer.c src/tuner.c src/uart_driver.c src/usb_descriptors.c src/usb_device.c src/usb_function_hid.c src/usb_hal_pic24.c src/usb_hid_stream_driver.c src/weak_stream_functions.c src/barreiraIR.c ../../../02_empilhadeira/controlplus_empilhadeira_urano/FWEMP.X/src/frequencies.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/src/ajuste_cap.o ${OBJECTDIR}/src/appl_commands.o ${OBJECTDIR}/src/appl_commands_table.o ${OBJECTDIR}/src/as3993.o ${OBJECTDIR}/src/bootloadable.o ${OBJECTDIR}/src/cancelas.o ${OBJECTDIR}/src/config_i2c3.o ${OBJECTDIR}/src/empilhadeira.o ${OBJECTDIR}/src/eth.o ${OBJECTDIR}/src/exclusao.o ${OBJECTDIR}/src/FSM_DataHora.o ${OBJECTDIR}/src/gen2.o ${OBJECTDIR}/src/global.o ${OBJECTDIR}/src/i2c.o ${OBJECTDIR}/src/logger.o ${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/mem_i2c_24LC256.o ${OBJECTDIR}/src/onewire.o ${OBJECTDIR}/src/perifericos.o ${OBJECTDIR}/src/platform.o ${OBJECTDIR}/src/portal.o ${OBJECTDIR}/src/rtc.o ${OBJECTDIR}/src/setup_usb.o ${OBJECTDIR}/src/spi_driver.o ${OBJECTDIR}/src/stream_dispatcher.o ${OBJECTDIR}/src/system_clock.o ${OBJECTDIR}/src/tags.o ${OBJECTDIR}/src/timer.o ${OBJECTDIR}/src/tuner.o ${OBJECTDIR}/src/uart_driver.o ${OBJECTDIR}/src/usb_descriptors.o ${OBJECTDIR}/src/usb_device.o ${OBJECTDIR}/src/usb_function_hid.o ${OBJECTDIR}/src/usb_hal_pic24.o ${OBJECTDIR}/src/usb_hid_stream_driver.o ${OBJECTDIR}/src/weak_stream_functions.o ${OBJECTDIR}/src/barreiraIR.o ${OBJECTDIR}/_ext/713759798/frequencies.o
POSSIBLE_DEPFILES=${OBJECTDIR}/src/ajuste_cap.o.d ${OBJECTDIR}/src/appl_commands.o.d ${OBJECTDIR}/src/appl_commands_table.o.d ${OBJECTDIR}/src/as3993.o.d ${OBJECTDIR}/src/bootloadable.o.d ${OBJECTDIR}/src/cancelas.o.d ${OBJECTDIR}/src/config_i2c3.o.d ${OBJECTDIR}/src/empilhadeira.o.d ${OBJECTDIR}/src/eth.o.d ${OBJECTDIR}/src/exclusao.o.d ${OBJECTDIR}/src/FSM_DataHora.o.d ${OBJECTDIR}/src/gen2.o.d ${OBJECTDIR}/src/global.o.d ${OBJECTDIR}/src/i2c.o.d ${OBJECTDIR}/src/logger.o.d ${OBJECTDIR}/src/main.o.d ${OBJECTDIR}/src/mem_i2c_24LC256.o.d ${OBJECTDIR}/src/onewire.o.d ${OBJECTDIR}/src/perifericos.o.d ${OBJECTDIR}/src/platform.o.d ${OBJECTDIR}/src/portal.o.d ${OBJECTDIR}/src/rtc.o.d ${OBJECTDIR}/src/setup_usb.o.d ${OBJECTDIR}/src/spi_driver.o.d ${OBJECTDIR}/src/stream_dispatcher.o.d ${OBJECTDIR}/src/system_clock.o.d ${OBJECTDIR}/src/tags.o.d ${OBJECTDIR}/src/timer.o.d ${OBJECTDIR}/src/tuner.o.d ${OBJECTDIR}/src/uart_driver.o.d ${OBJECTDIR}/src/usb_descriptors.o.d ${OBJECTDIR}/src/usb_device.o.d ${OBJECTDIR}/src/usb_function_hid.o.d ${OBJECTDIR}/src/usb_hal_pic24.o.d ${OBJECTDIR}/src/usb_hid_stream_driver.o.d ${OBJECTDIR}/src/weak_stream_functions.o.d ${OBJECTDIR}/src/barreiraIR.o.d ${OBJECTDIR}/_ext/713759798/frequencies.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/src/ajuste_cap.o ${OBJECTDIR}/src/appl_commands.o ${OBJECTDIR}/src/appl_commands_table.o ${OBJECTDIR}/src/as3993.o ${OBJECTDIR}/src/bootloadable.o ${OBJECTDIR}/src/cancelas.o ${OBJECTDIR}/src/config_i2c3.o ${OBJECTDIR}/src/empilhadeira.o ${OBJECTDIR}/src/eth.o ${OBJECTDIR}/src/exclusao.o ${OBJECTDIR}/src/FSM_DataHora.o ${OBJECTDIR}/src/gen2.o ${OBJECTDIR}/src/global.o ${OBJECTDIR}/src/i2c.o ${OBJECTDIR}/src/logger.o ${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/mem_i2c_24LC256.o ${OBJECTDIR}/src/onewire.o ${OBJECTDIR}/src/perifericos.o ${OBJECTDIR}/src/platform.o ${OBJECTDIR}/src/portal.o ${OBJECTDIR}/src/rtc.o ${OBJECTDIR}/src/setup_usb.o ${OBJECTDIR}/src/spi_driver.o ${OBJECTDIR}/src/stream_dispatcher.o ${OBJECTDIR}/src/system_clock.o ${OBJECTDIR}/src/tags.o ${OBJECTDIR}/src/timer.o ${OBJECTDIR}/src/tuner.o ${OBJECTDIR}/src/uart_driver.o ${OBJECTDIR}/src/usb_descriptors.o ${OBJECTDIR}/src/usb_device.o ${OBJECTDIR}/src/usb_function_hid.o ${OBJECTDIR}/src/usb_hal_pic24.o ${OBJECTDIR}/src/usb_hid_stream_driver.o ${OBJECTDIR}/src/weak_stream_functions.o ${OBJECTDIR}/src/barreiraIR.o ${OBJECTDIR}/_ext/713759798/frequencies.o

# Source Files
SOURCEFILES=src/ajuste_cap.c src/appl_commands.c src/appl_commands_table.c src/as3993.c src/bootloadable.c src/cancelas.c src/config_i2c3.c src/empilhadeira.c src/eth.c src/exclusao.c src/FSM_DataHora.c src/gen2.c src/global.c src/i2c.c src/logger.c src/main.c src/mem_i2c_24LC256.c src/onewire.c src/perifericos.c src/platform.c src/portal.c src/rtc.c src/setup_usb.c src/spi_driver.c src/stream_dispatcher.c src/system_clock.c src/tags.c src/timer.c src/tuner.c src/uart_driver.c src/usb_descriptors.c src/usb_device.c src/usb_function_hid.c src/usb_hal_pic24.c src/usb_hid_stream_driver.c src/weak_stream_functions.c src/barreiraIR.c ../../../02_empilhadeira/controlplus_empilhadeira_urano/FWEMP.X/src/frequencies.c



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

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-debug.mk ${DISTDIR}/FWPORTAL.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24FJ256DA210
MP_LINKER_FILE_OPTION=,--script="src\p24FJ256DA210.gld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/src/ajuste_cap.o: src/ajuste_cap.c  .generated_files/flags/debug/9bb22c98c7fa9f52c45aff7265bb5225de1b2a4f .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/ajuste_cap.o.d 
	@${RM} ${OBJECTDIR}/src/ajuste_cap.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/ajuste_cap.c  -o ${OBJECTDIR}/src/ajuste_cap.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/ajuste_cap.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/appl_commands.o: src/appl_commands.c  .generated_files/flags/debug/2dd8ece28c45f2e92e1b146053f91b04a92866c5 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/appl_commands.o.d 
	@${RM} ${OBJECTDIR}/src/appl_commands.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/appl_commands.c  -o ${OBJECTDIR}/src/appl_commands.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/appl_commands.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/appl_commands_table.o: src/appl_commands_table.c  .generated_files/flags/debug/487a2d136c933ff112045042892c85373d638723 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/appl_commands_table.o.d 
	@${RM} ${OBJECTDIR}/src/appl_commands_table.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/appl_commands_table.c  -o ${OBJECTDIR}/src/appl_commands_table.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/appl_commands_table.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/as3993.o: src/as3993.c  .generated_files/flags/debug/9c684deafe8152791cae7b1d0fc8942b229b9a0f .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/as3993.o.d 
	@${RM} ${OBJECTDIR}/src/as3993.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/as3993.c  -o ${OBJECTDIR}/src/as3993.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/as3993.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/bootloadable.o: src/bootloadable.c  .generated_files/flags/debug/ba8365990836618328ddd9c038134742afcecf1a .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/bootloadable.o.d 
	@${RM} ${OBJECTDIR}/src/bootloadable.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/bootloadable.c  -o ${OBJECTDIR}/src/bootloadable.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/bootloadable.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/cancelas.o: src/cancelas.c  .generated_files/flags/debug/8c831237e48f06ecc058c59e4682175153efb085 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/cancelas.o.d 
	@${RM} ${OBJECTDIR}/src/cancelas.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/cancelas.c  -o ${OBJECTDIR}/src/cancelas.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/cancelas.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/config_i2c3.o: src/config_i2c3.c  .generated_files/flags/debug/7bea95f14598ce0b4ef37571654a6b1b6da23260 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/config_i2c3.o.d 
	@${RM} ${OBJECTDIR}/src/config_i2c3.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/config_i2c3.c  -o ${OBJECTDIR}/src/config_i2c3.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/config_i2c3.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/empilhadeira.o: src/empilhadeira.c  .generated_files/flags/debug/219f5bb2ff20a105e14a0c4792d45a936dfa98bd .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/empilhadeira.o.d 
	@${RM} ${OBJECTDIR}/src/empilhadeira.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/empilhadeira.c  -o ${OBJECTDIR}/src/empilhadeira.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/empilhadeira.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/eth.o: src/eth.c  .generated_files/flags/debug/251879ba08f84b97a938a4db14c36afa9c9c5acd .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/eth.o.d 
	@${RM} ${OBJECTDIR}/src/eth.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/eth.c  -o ${OBJECTDIR}/src/eth.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/eth.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/exclusao.o: src/exclusao.c  .generated_files/flags/debug/541299c7608ccc86ce26e36f017bda32287dddf9 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/exclusao.o.d 
	@${RM} ${OBJECTDIR}/src/exclusao.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/exclusao.c  -o ${OBJECTDIR}/src/exclusao.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/exclusao.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/FSM_DataHora.o: src/FSM_DataHora.c  .generated_files/flags/debug/25049bab151a6d99a7fee45f91e510f5ca695970 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/FSM_DataHora.o.d 
	@${RM} ${OBJECTDIR}/src/FSM_DataHora.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FSM_DataHora.c  -o ${OBJECTDIR}/src/FSM_DataHora.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/FSM_DataHora.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/gen2.o: src/gen2.c  .generated_files/flags/debug/48d828945b11a54b162a5b528545e7de19281aeb .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/gen2.o.d 
	@${RM} ${OBJECTDIR}/src/gen2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/gen2.c  -o ${OBJECTDIR}/src/gen2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/gen2.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/global.o: src/global.c  .generated_files/flags/debug/ee15d035d333a71633098f9c6aaa6fa0c83f560b .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/global.o.d 
	@${RM} ${OBJECTDIR}/src/global.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/global.c  -o ${OBJECTDIR}/src/global.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/global.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/i2c.o: src/i2c.c  .generated_files/flags/debug/913cd08ea0f440b6c7deabf7f2fa5d6cca2064b4 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/i2c.o.d 
	@${RM} ${OBJECTDIR}/src/i2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/i2c.c  -o ${OBJECTDIR}/src/i2c.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/i2c.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/logger.o: src/logger.c  .generated_files/flags/debug/2b14e2f3ce834f3dc54646268a64376d71f3799e .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/logger.o.d 
	@${RM} ${OBJECTDIR}/src/logger.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/logger.c  -o ${OBJECTDIR}/src/logger.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/logger.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/main.o: src/main.c  .generated_files/flags/debug/814e4c303483aa1efed93125bdcd1eed46198b8a .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/main.o.d 
	@${RM} ${OBJECTDIR}/src/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/main.c  -o ${OBJECTDIR}/src/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/mem_i2c_24LC256.o: src/mem_i2c_24LC256.c  .generated_files/flags/debug/17c8f5909ed3da59f2421fbed057aeb92aedd6ac .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/mem_i2c_24LC256.o.d 
	@${RM} ${OBJECTDIR}/src/mem_i2c_24LC256.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/mem_i2c_24LC256.c  -o ${OBJECTDIR}/src/mem_i2c_24LC256.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/mem_i2c_24LC256.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/onewire.o: src/onewire.c  .generated_files/flags/debug/83b419bb5b495d8db19964aa8f6c4bff9f896a8f .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/onewire.o.d 
	@${RM} ${OBJECTDIR}/src/onewire.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/onewire.c  -o ${OBJECTDIR}/src/onewire.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/onewire.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/perifericos.o: src/perifericos.c  .generated_files/flags/debug/227e772a534e022b0b29b1cd243356e2b79a8fc7 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/perifericos.o.d 
	@${RM} ${OBJECTDIR}/src/perifericos.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/perifericos.c  -o ${OBJECTDIR}/src/perifericos.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/perifericos.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/platform.o: src/platform.c  .generated_files/flags/debug/f3ddea5176c7a131be430671073dfbddb1c4f606 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/platform.o.d 
	@${RM} ${OBJECTDIR}/src/platform.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/platform.c  -o ${OBJECTDIR}/src/platform.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/platform.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/portal.o: src/portal.c  .generated_files/flags/debug/c97be302d5c3dbd9094a36d60ff4c74dd8de5cd5 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/portal.o.d 
	@${RM} ${OBJECTDIR}/src/portal.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/portal.c  -o ${OBJECTDIR}/src/portal.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/portal.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/rtc.o: src/rtc.c  .generated_files/flags/debug/9c041d21d26df909adc635f2db2d68fdf3956c02 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/rtc.o.d 
	@${RM} ${OBJECTDIR}/src/rtc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/rtc.c  -o ${OBJECTDIR}/src/rtc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/rtc.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/setup_usb.o: src/setup_usb.c  .generated_files/flags/debug/361e8f18728709394ad2d298950bb2e0f2f774a2 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/setup_usb.o.d 
	@${RM} ${OBJECTDIR}/src/setup_usb.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/setup_usb.c  -o ${OBJECTDIR}/src/setup_usb.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/setup_usb.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/spi_driver.o: src/spi_driver.c  .generated_files/flags/debug/5d0f422ae8f97767c0af88ae908f21ad0319bba1 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/spi_driver.o.d 
	@${RM} ${OBJECTDIR}/src/spi_driver.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/spi_driver.c  -o ${OBJECTDIR}/src/spi_driver.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/spi_driver.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/stream_dispatcher.o: src/stream_dispatcher.c  .generated_files/flags/debug/91d9110809051c317383316950d89bfb6c6e1af1 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/stream_dispatcher.o.d 
	@${RM} ${OBJECTDIR}/src/stream_dispatcher.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/stream_dispatcher.c  -o ${OBJECTDIR}/src/stream_dispatcher.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/stream_dispatcher.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/system_clock.o: src/system_clock.c  .generated_files/flags/debug/781b914e1fc0125dc90b242098fda23222e7e583 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/system_clock.o.d 
	@${RM} ${OBJECTDIR}/src/system_clock.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/system_clock.c  -o ${OBJECTDIR}/src/system_clock.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/system_clock.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/tags.o: src/tags.c  .generated_files/flags/debug/b1780f340c7e89a6a93c97ce750bdc923bc2627c .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/tags.o.d 
	@${RM} ${OBJECTDIR}/src/tags.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/tags.c  -o ${OBJECTDIR}/src/tags.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/tags.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/timer.o: src/timer.c  .generated_files/flags/debug/1548ad1c32796400de69a1c89aacbafd58c0e42f .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/timer.o.d 
	@${RM} ${OBJECTDIR}/src/timer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/timer.c  -o ${OBJECTDIR}/src/timer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/timer.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/tuner.o: src/tuner.c  .generated_files/flags/debug/21e4496a0ba6c631de1a877f126e9fb061d69f15 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/tuner.o.d 
	@${RM} ${OBJECTDIR}/src/tuner.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/tuner.c  -o ${OBJECTDIR}/src/tuner.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/tuner.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/uart_driver.o: src/uart_driver.c  .generated_files/flags/debug/fcda45a152a1f9b68315f57e7173ab7ee3ff4f91 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/uart_driver.o.d 
	@${RM} ${OBJECTDIR}/src/uart_driver.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/uart_driver.c  -o ${OBJECTDIR}/src/uart_driver.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/uart_driver.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/usb_descriptors.o: src/usb_descriptors.c  .generated_files/flags/debug/90f1ecea792257e2428741bfe2057dcb560370aa .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/src/usb_descriptors.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/usb_descriptors.c  -o ${OBJECTDIR}/src/usb_descriptors.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/usb_descriptors.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/usb_device.o: src/usb_device.c  .generated_files/flags/debug/882e50351163726db8abc728c6f513a950a76b3d .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/usb_device.o.d 
	@${RM} ${OBJECTDIR}/src/usb_device.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/usb_device.c  -o ${OBJECTDIR}/src/usb_device.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/usb_device.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/usb_function_hid.o: src/usb_function_hid.c  .generated_files/flags/debug/89b8149e37fe1d400ebf7f30752cbff2c6b239b5 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/usb_function_hid.o.d 
	@${RM} ${OBJECTDIR}/src/usb_function_hid.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/usb_function_hid.c  -o ${OBJECTDIR}/src/usb_function_hid.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/usb_function_hid.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/usb_hal_pic24.o: src/usb_hal_pic24.c  .generated_files/flags/debug/6201b56380f21081a33faa50773bc13898b5c9d .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/usb_hal_pic24.o.d 
	@${RM} ${OBJECTDIR}/src/usb_hal_pic24.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/usb_hal_pic24.c  -o ${OBJECTDIR}/src/usb_hal_pic24.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/usb_hal_pic24.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/usb_hid_stream_driver.o: src/usb_hid_stream_driver.c  .generated_files/flags/debug/4c15ec33cf2f789984e52c18edc3840ed7a5bef1 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/usb_hid_stream_driver.o.d 
	@${RM} ${OBJECTDIR}/src/usb_hid_stream_driver.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/usb_hid_stream_driver.c  -o ${OBJECTDIR}/src/usb_hid_stream_driver.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/usb_hid_stream_driver.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/weak_stream_functions.o: src/weak_stream_functions.c  .generated_files/flags/debug/414203d8ca6e7a8a599d84f8ec703e9fb355a6c1 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/weak_stream_functions.o.d 
	@${RM} ${OBJECTDIR}/src/weak_stream_functions.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/weak_stream_functions.c  -o ${OBJECTDIR}/src/weak_stream_functions.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/weak_stream_functions.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/barreiraIR.o: src/barreiraIR.c  .generated_files/flags/debug/44493daec3af545ecc114916206df01d2f2b1279 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/barreiraIR.o.d 
	@${RM} ${OBJECTDIR}/src/barreiraIR.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/barreiraIR.c  -o ${OBJECTDIR}/src/barreiraIR.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/barreiraIR.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/713759798/frequencies.o: ../../../02_empilhadeira/controlplus_empilhadeira_urano/FWEMP.X/src/frequencies.c  .generated_files/flags/debug/7c12523f07149a3cb12913867456dad5d2a8fbb4 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/_ext/713759798" 
	@${RM} ${OBJECTDIR}/_ext/713759798/frequencies.o.d 
	@${RM} ${OBJECTDIR}/_ext/713759798/frequencies.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../02_empilhadeira/controlplus_empilhadeira_urano/FWEMP.X/src/frequencies.c  -o ${OBJECTDIR}/_ext/713759798/frequencies.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/713759798/frequencies.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
else
${OBJECTDIR}/src/ajuste_cap.o: src/ajuste_cap.c  .generated_files/flags/debug/e3707f11f2fdea3f371af2e67798587d108a92e8 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/ajuste_cap.o.d 
	@${RM} ${OBJECTDIR}/src/ajuste_cap.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/ajuste_cap.c  -o ${OBJECTDIR}/src/ajuste_cap.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/ajuste_cap.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/appl_commands.o: src/appl_commands.c  .generated_files/flags/debug/6b306bc9ea562c79caffb95bfce03f1e35a61070 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/appl_commands.o.d 
	@${RM} ${OBJECTDIR}/src/appl_commands.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/appl_commands.c  -o ${OBJECTDIR}/src/appl_commands.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/appl_commands.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/appl_commands_table.o: src/appl_commands_table.c  .generated_files/flags/debug/72e4e5c71bee2117b94a6079f411db7f178274a5 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/appl_commands_table.o.d 
	@${RM} ${OBJECTDIR}/src/appl_commands_table.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/appl_commands_table.c  -o ${OBJECTDIR}/src/appl_commands_table.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/appl_commands_table.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/as3993.o: src/as3993.c  .generated_files/flags/debug/cc5a60ca0599bb8a43340a61f5e8fc6633ccf765 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/as3993.o.d 
	@${RM} ${OBJECTDIR}/src/as3993.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/as3993.c  -o ${OBJECTDIR}/src/as3993.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/as3993.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/bootloadable.o: src/bootloadable.c  .generated_files/flags/debug/84df60ef28309e5122f98a385856d7c10887b380 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/bootloadable.o.d 
	@${RM} ${OBJECTDIR}/src/bootloadable.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/bootloadable.c  -o ${OBJECTDIR}/src/bootloadable.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/bootloadable.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/cancelas.o: src/cancelas.c  .generated_files/flags/debug/50e46bdf179d625b01b749fa84a0757eecb4f9d9 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/cancelas.o.d 
	@${RM} ${OBJECTDIR}/src/cancelas.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/cancelas.c  -o ${OBJECTDIR}/src/cancelas.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/cancelas.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/config_i2c3.o: src/config_i2c3.c  .generated_files/flags/debug/140a27880c0ce7e0aea13387b3bbfc7029756577 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/config_i2c3.o.d 
	@${RM} ${OBJECTDIR}/src/config_i2c3.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/config_i2c3.c  -o ${OBJECTDIR}/src/config_i2c3.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/config_i2c3.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/empilhadeira.o: src/empilhadeira.c  .generated_files/flags/debug/40e4fade15955725f89d24c3381d124150cb7fec .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/empilhadeira.o.d 
	@${RM} ${OBJECTDIR}/src/empilhadeira.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/empilhadeira.c  -o ${OBJECTDIR}/src/empilhadeira.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/empilhadeira.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/eth.o: src/eth.c  .generated_files/flags/debug/fd27d29ef7c35978ac9ae0c2c5b5175d73138998 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/eth.o.d 
	@${RM} ${OBJECTDIR}/src/eth.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/eth.c  -o ${OBJECTDIR}/src/eth.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/eth.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/exclusao.o: src/exclusao.c  .generated_files/flags/debug/8c3c08a3c322dbbbce4fc2e52659bb4e031e75ba .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/exclusao.o.d 
	@${RM} ${OBJECTDIR}/src/exclusao.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/exclusao.c  -o ${OBJECTDIR}/src/exclusao.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/exclusao.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/FSM_DataHora.o: src/FSM_DataHora.c  .generated_files/flags/debug/368a8b3595c2da79130400e40d103bd764c7aa21 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/FSM_DataHora.o.d 
	@${RM} ${OBJECTDIR}/src/FSM_DataHora.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FSM_DataHora.c  -o ${OBJECTDIR}/src/FSM_DataHora.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/FSM_DataHora.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/gen2.o: src/gen2.c  .generated_files/flags/debug/3644cb3b73ad8318332edb1a94d63c92dbab96f3 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/gen2.o.d 
	@${RM} ${OBJECTDIR}/src/gen2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/gen2.c  -o ${OBJECTDIR}/src/gen2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/gen2.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/global.o: src/global.c  .generated_files/flags/debug/58c6becd24fd099722e1da4109547b62d37424f9 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/global.o.d 
	@${RM} ${OBJECTDIR}/src/global.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/global.c  -o ${OBJECTDIR}/src/global.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/global.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/i2c.o: src/i2c.c  .generated_files/flags/debug/7e63864ccad5348beb40ab5060f8363e0ceda83e .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/i2c.o.d 
	@${RM} ${OBJECTDIR}/src/i2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/i2c.c  -o ${OBJECTDIR}/src/i2c.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/i2c.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/logger.o: src/logger.c  .generated_files/flags/debug/96207521c7a1f49f0ff8e542775a2c98ad58f0 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/logger.o.d 
	@${RM} ${OBJECTDIR}/src/logger.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/logger.c  -o ${OBJECTDIR}/src/logger.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/logger.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/main.o: src/main.c  .generated_files/flags/debug/4bbd08f7b6494457d9e1537d94635e99cd60fca5 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/main.o.d 
	@${RM} ${OBJECTDIR}/src/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/main.c  -o ${OBJECTDIR}/src/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/main.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/mem_i2c_24LC256.o: src/mem_i2c_24LC256.c  .generated_files/flags/debug/287cf3c1602d98d947cceafede533379b517a17e .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/mem_i2c_24LC256.o.d 
	@${RM} ${OBJECTDIR}/src/mem_i2c_24LC256.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/mem_i2c_24LC256.c  -o ${OBJECTDIR}/src/mem_i2c_24LC256.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/mem_i2c_24LC256.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/onewire.o: src/onewire.c  .generated_files/flags/debug/7c0254e109ac96b78b7a41be28190965001263a1 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/onewire.o.d 
	@${RM} ${OBJECTDIR}/src/onewire.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/onewire.c  -o ${OBJECTDIR}/src/onewire.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/onewire.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/perifericos.o: src/perifericos.c  .generated_files/flags/debug/36b63263b58b8390a2b78974b35560ba82e9203 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/perifericos.o.d 
	@${RM} ${OBJECTDIR}/src/perifericos.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/perifericos.c  -o ${OBJECTDIR}/src/perifericos.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/perifericos.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/platform.o: src/platform.c  .generated_files/flags/debug/e366576e4f57e822d8ac371724038df7b306551d .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/platform.o.d 
	@${RM} ${OBJECTDIR}/src/platform.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/platform.c  -o ${OBJECTDIR}/src/platform.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/platform.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/portal.o: src/portal.c  .generated_files/flags/debug/eb0945bf752153071d50f95064ade6925dc34966 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/portal.o.d 
	@${RM} ${OBJECTDIR}/src/portal.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/portal.c  -o ${OBJECTDIR}/src/portal.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/portal.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/rtc.o: src/rtc.c  .generated_files/flags/debug/df9bebb30d0f958ec660b0f92f3e7440dba9e680 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/rtc.o.d 
	@${RM} ${OBJECTDIR}/src/rtc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/rtc.c  -o ${OBJECTDIR}/src/rtc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/rtc.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/setup_usb.o: src/setup_usb.c  .generated_files/flags/debug/43a438c379ddf644bb64e8a5ad21f46a947405a7 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/setup_usb.o.d 
	@${RM} ${OBJECTDIR}/src/setup_usb.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/setup_usb.c  -o ${OBJECTDIR}/src/setup_usb.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/setup_usb.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/spi_driver.o: src/spi_driver.c  .generated_files/flags/debug/5e7d0be6641aa52af7d564c06456077fcfc2a31 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/spi_driver.o.d 
	@${RM} ${OBJECTDIR}/src/spi_driver.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/spi_driver.c  -o ${OBJECTDIR}/src/spi_driver.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/spi_driver.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/stream_dispatcher.o: src/stream_dispatcher.c  .generated_files/flags/debug/42ddcbcb71710d5e2511aad429ebde07589ab63c .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/stream_dispatcher.o.d 
	@${RM} ${OBJECTDIR}/src/stream_dispatcher.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/stream_dispatcher.c  -o ${OBJECTDIR}/src/stream_dispatcher.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/stream_dispatcher.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/system_clock.o: src/system_clock.c  .generated_files/flags/debug/c8c09ad9348b5cd6d2226d70cb55c40c22680f29 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/system_clock.o.d 
	@${RM} ${OBJECTDIR}/src/system_clock.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/system_clock.c  -o ${OBJECTDIR}/src/system_clock.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/system_clock.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/tags.o: src/tags.c  .generated_files/flags/debug/28d1483851136d20a06419055a46d7b6bc26cdc2 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/tags.o.d 
	@${RM} ${OBJECTDIR}/src/tags.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/tags.c  -o ${OBJECTDIR}/src/tags.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/tags.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/timer.o: src/timer.c  .generated_files/flags/debug/9d06246dafc17e75a60f891785b50958a5f6b550 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/timer.o.d 
	@${RM} ${OBJECTDIR}/src/timer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/timer.c  -o ${OBJECTDIR}/src/timer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/timer.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/tuner.o: src/tuner.c  .generated_files/flags/debug/4e23535fd4dcd6efabc83b2269d72c54bc3f0cbf .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/tuner.o.d 
	@${RM} ${OBJECTDIR}/src/tuner.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/tuner.c  -o ${OBJECTDIR}/src/tuner.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/tuner.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/uart_driver.o: src/uart_driver.c  .generated_files/flags/debug/87c10a5038774694f112be7df4996b86db0c1dd1 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/uart_driver.o.d 
	@${RM} ${OBJECTDIR}/src/uart_driver.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/uart_driver.c  -o ${OBJECTDIR}/src/uart_driver.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/uart_driver.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/usb_descriptors.o: src/usb_descriptors.c  .generated_files/flags/debug/4966444ca31d8acce4f6981048196b25f8f706d3 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/src/usb_descriptors.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/usb_descriptors.c  -o ${OBJECTDIR}/src/usb_descriptors.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/usb_descriptors.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/usb_device.o: src/usb_device.c  .generated_files/flags/debug/f31ff86d40882949237b4228fcb1bb4a1a3ba768 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/usb_device.o.d 
	@${RM} ${OBJECTDIR}/src/usb_device.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/usb_device.c  -o ${OBJECTDIR}/src/usb_device.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/usb_device.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/usb_function_hid.o: src/usb_function_hid.c  .generated_files/flags/debug/e3af08fac17547cf3b0aab3172679733cefcfc30 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/usb_function_hid.o.d 
	@${RM} ${OBJECTDIR}/src/usb_function_hid.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/usb_function_hid.c  -o ${OBJECTDIR}/src/usb_function_hid.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/usb_function_hid.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/usb_hal_pic24.o: src/usb_hal_pic24.c  .generated_files/flags/debug/bd22e6eb75336bcbd531de5f61c3bbfd7389c35d .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/usb_hal_pic24.o.d 
	@${RM} ${OBJECTDIR}/src/usb_hal_pic24.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/usb_hal_pic24.c  -o ${OBJECTDIR}/src/usb_hal_pic24.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/usb_hal_pic24.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/usb_hid_stream_driver.o: src/usb_hid_stream_driver.c  .generated_files/flags/debug/44f2e9fbb6e23b324aa914a8aa82b41fd0d040f .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/usb_hid_stream_driver.o.d 
	@${RM} ${OBJECTDIR}/src/usb_hid_stream_driver.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/usb_hid_stream_driver.c  -o ${OBJECTDIR}/src/usb_hid_stream_driver.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/usb_hid_stream_driver.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/weak_stream_functions.o: src/weak_stream_functions.c  .generated_files/flags/debug/d68119c03e6ac04e8b2ced2e37f0e5892b1857a0 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/weak_stream_functions.o.d 
	@${RM} ${OBJECTDIR}/src/weak_stream_functions.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/weak_stream_functions.c  -o ${OBJECTDIR}/src/weak_stream_functions.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/weak_stream_functions.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/barreiraIR.o: src/barreiraIR.c  .generated_files/flags/debug/36114d1241f89f22468f24ed756f9695195444c0 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/barreiraIR.o.d 
	@${RM} ${OBJECTDIR}/src/barreiraIR.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/barreiraIR.c  -o ${OBJECTDIR}/src/barreiraIR.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/barreiraIR.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/713759798/frequencies.o: ../../../02_empilhadeira/controlplus_empilhadeira_urano/FWEMP.X/src/frequencies.c  .generated_files/flags/debug/96c06d696ed2fd2d1a197414538022f80239941 .generated_files/flags/debug/8881d020ac2e456b5250ba3b697007f725b33ad9
	@${MKDIR} "${OBJECTDIR}/_ext/713759798" 
	@${RM} ${OBJECTDIR}/_ext/713759798/frequencies.o.d 
	@${RM} ${OBJECTDIR}/_ext/713759798/frequencies.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../02_empilhadeira/controlplus_empilhadeira_urano/FWEMP.X/src/frequencies.c  -o ${OBJECTDIR}/_ext/713759798/frequencies.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/713759798/frequencies.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/FWPORTAL.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    src/p24FJ256DA210.gld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/FWPORTAL.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)   -mreserve=data@0x800:0x81B -mreserve=data@0x81C:0x81D -mreserve=data@0x81E:0x81F -mreserve=data@0x820:0x821 -mreserve=data@0x822:0x823 -mreserve=data@0x824:0x827 -mreserve=data@0x82A:0x84F   -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,--defsym=__MPLAB_DEBUGGER_PK3=1,$(MP_LINKER_FILE_OPTION),--heap=1950,--no-check-sections,--data-init,--pack-data,--handles,--isr,--gc-sections,--fill-upper=0,--stackguard=16,--library-path="..",--no-force-link,--smart-io,-Map="$(BINDIR_)$(TARGETBASE).map",--report-mem$(MP_EXTRA_LD_POST)  
	
else
${DISTDIR}/FWPORTAL.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   src/p24FJ256DA210.gld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/FWPORTAL.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--heap=1950,--no-check-sections,--data-init,--pack-data,--handles,--isr,--gc-sections,--fill-upper=0,--stackguard=16,--library-path="..",--no-force-link,--smart-io,-Map="$(BINDIR_)$(TARGETBASE).map",--report-mem$(MP_EXTRA_LD_POST)  
	${MP_CC_DIR}\\xc16-bin2hex ${DISTDIR}/FWPORTAL.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf   
	
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
