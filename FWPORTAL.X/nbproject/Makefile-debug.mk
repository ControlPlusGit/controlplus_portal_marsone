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
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/FWPORTAL.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/FWPORTAL.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
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
SOURCEFILES_QUOTED_IF_SPACED=src/ajuste_cap.c src/appl_commands.c src/appl_commands_table.c src/as3993.c src/bootloadable.c src/cancelas.c src/config_i2c3.c src/empilhadeira.c src/eth.c src/exclusao.c src/FSM_DataHora.c src/gen2.c src/global.c src/i2c.c src/logger.c src/main.c src/mem_i2c_24LC256.c src/onewire.c src/perifericos.c src/platform.c src/portal.c src/rtc.c src/setup_usb.c src/spi_driver.c src/stream_dispatcher.c src/system_clock.c src/tags.c src/timer.c src/tuner.c src/uart_driver.c src/usb_descriptors.c src/usb_device.c src/usb_function_hid.c src/usb_hal_pic24.c src/usb_hid_stream_driver.c src/weak_stream_functions.c src/barreiraIR.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/src/ajuste_cap.o ${OBJECTDIR}/src/appl_commands.o ${OBJECTDIR}/src/appl_commands_table.o ${OBJECTDIR}/src/as3993.o ${OBJECTDIR}/src/bootloadable.o ${OBJECTDIR}/src/cancelas.o ${OBJECTDIR}/src/config_i2c3.o ${OBJECTDIR}/src/empilhadeira.o ${OBJECTDIR}/src/eth.o ${OBJECTDIR}/src/exclusao.o ${OBJECTDIR}/src/FSM_DataHora.o ${OBJECTDIR}/src/gen2.o ${OBJECTDIR}/src/global.o ${OBJECTDIR}/src/i2c.o ${OBJECTDIR}/src/logger.o ${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/mem_i2c_24LC256.o ${OBJECTDIR}/src/onewire.o ${OBJECTDIR}/src/perifericos.o ${OBJECTDIR}/src/platform.o ${OBJECTDIR}/src/portal.o ${OBJECTDIR}/src/rtc.o ${OBJECTDIR}/src/setup_usb.o ${OBJECTDIR}/src/spi_driver.o ${OBJECTDIR}/src/stream_dispatcher.o ${OBJECTDIR}/src/system_clock.o ${OBJECTDIR}/src/tags.o ${OBJECTDIR}/src/timer.o ${OBJECTDIR}/src/tuner.o ${OBJECTDIR}/src/uart_driver.o ${OBJECTDIR}/src/usb_descriptors.o ${OBJECTDIR}/src/usb_device.o ${OBJECTDIR}/src/usb_function_hid.o ${OBJECTDIR}/src/usb_hal_pic24.o ${OBJECTDIR}/src/usb_hid_stream_driver.o ${OBJECTDIR}/src/weak_stream_functions.o ${OBJECTDIR}/src/barreiraIR.o
POSSIBLE_DEPFILES=${OBJECTDIR}/src/ajuste_cap.o.d ${OBJECTDIR}/src/appl_commands.o.d ${OBJECTDIR}/src/appl_commands_table.o.d ${OBJECTDIR}/src/as3993.o.d ${OBJECTDIR}/src/bootloadable.o.d ${OBJECTDIR}/src/cancelas.o.d ${OBJECTDIR}/src/config_i2c3.o.d ${OBJECTDIR}/src/empilhadeira.o.d ${OBJECTDIR}/src/eth.o.d ${OBJECTDIR}/src/exclusao.o.d ${OBJECTDIR}/src/FSM_DataHora.o.d ${OBJECTDIR}/src/gen2.o.d ${OBJECTDIR}/src/global.o.d ${OBJECTDIR}/src/i2c.o.d ${OBJECTDIR}/src/logger.o.d ${OBJECTDIR}/src/main.o.d ${OBJECTDIR}/src/mem_i2c_24LC256.o.d ${OBJECTDIR}/src/onewire.o.d ${OBJECTDIR}/src/perifericos.o.d ${OBJECTDIR}/src/platform.o.d ${OBJECTDIR}/src/portal.o.d ${OBJECTDIR}/src/rtc.o.d ${OBJECTDIR}/src/setup_usb.o.d ${OBJECTDIR}/src/spi_driver.o.d ${OBJECTDIR}/src/stream_dispatcher.o.d ${OBJECTDIR}/src/system_clock.o.d ${OBJECTDIR}/src/tags.o.d ${OBJECTDIR}/src/timer.o.d ${OBJECTDIR}/src/tuner.o.d ${OBJECTDIR}/src/uart_driver.o.d ${OBJECTDIR}/src/usb_descriptors.o.d ${OBJECTDIR}/src/usb_device.o.d ${OBJECTDIR}/src/usb_function_hid.o.d ${OBJECTDIR}/src/usb_hal_pic24.o.d ${OBJECTDIR}/src/usb_hid_stream_driver.o.d ${OBJECTDIR}/src/weak_stream_functions.o.d ${OBJECTDIR}/src/barreiraIR.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/src/ajuste_cap.o ${OBJECTDIR}/src/appl_commands.o ${OBJECTDIR}/src/appl_commands_table.o ${OBJECTDIR}/src/as3993.o ${OBJECTDIR}/src/bootloadable.o ${OBJECTDIR}/src/cancelas.o ${OBJECTDIR}/src/config_i2c3.o ${OBJECTDIR}/src/empilhadeira.o ${OBJECTDIR}/src/eth.o ${OBJECTDIR}/src/exclusao.o ${OBJECTDIR}/src/FSM_DataHora.o ${OBJECTDIR}/src/gen2.o ${OBJECTDIR}/src/global.o ${OBJECTDIR}/src/i2c.o ${OBJECTDIR}/src/logger.o ${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/mem_i2c_24LC256.o ${OBJECTDIR}/src/onewire.o ${OBJECTDIR}/src/perifericos.o ${OBJECTDIR}/src/platform.o ${OBJECTDIR}/src/portal.o ${OBJECTDIR}/src/rtc.o ${OBJECTDIR}/src/setup_usb.o ${OBJECTDIR}/src/spi_driver.o ${OBJECTDIR}/src/stream_dispatcher.o ${OBJECTDIR}/src/system_clock.o ${OBJECTDIR}/src/tags.o ${OBJECTDIR}/src/timer.o ${OBJECTDIR}/src/tuner.o ${OBJECTDIR}/src/uart_driver.o ${OBJECTDIR}/src/usb_descriptors.o ${OBJECTDIR}/src/usb_device.o ${OBJECTDIR}/src/usb_function_hid.o ${OBJECTDIR}/src/usb_hal_pic24.o ${OBJECTDIR}/src/usb_hid_stream_driver.o ${OBJECTDIR}/src/weak_stream_functions.o ${OBJECTDIR}/src/barreiraIR.o

# Source Files
SOURCEFILES=src/ajuste_cap.c src/appl_commands.c src/appl_commands_table.c src/as3993.c src/bootloadable.c src/cancelas.c src/config_i2c3.c src/empilhadeira.c src/eth.c src/exclusao.c src/FSM_DataHora.c src/gen2.c src/global.c src/i2c.c src/logger.c src/main.c src/mem_i2c_24LC256.c src/onewire.c src/perifericos.c src/platform.c src/portal.c src/rtc.c src/setup_usb.c src/spi_driver.c src/stream_dispatcher.c src/system_clock.c src/tags.c src/timer.c src/tuner.c src/uart_driver.c src/usb_descriptors.c src/usb_device.c src/usb_function_hid.c src/usb_hal_pic24.c src/usb_hid_stream_driver.c src/weak_stream_functions.c src/barreiraIR.c



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
	${MAKE}  -f nbproject/Makefile-debug.mk dist/${CND_CONF}/${IMAGE_TYPE}/FWPORTAL.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24FJ256DA210
MP_LINKER_FILE_OPTION=,--script="src\p24FJ256DA210.gld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/src/ajuste_cap.o: src/ajuste_cap.c  .generated_files/flags/debug/1364d8e5fb376bd38ec8370ad0b19be622b96b8f .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/ajuste_cap.o.d 
	@${RM} ${OBJECTDIR}/src/ajuste_cap.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/ajuste_cap.c  -o ${OBJECTDIR}/src/ajuste_cap.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/ajuste_cap.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/appl_commands.o: src/appl_commands.c  .generated_files/flags/debug/ae2a39766c64091add64d9da2439b17573bc1555 .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/appl_commands.o.d 
	@${RM} ${OBJECTDIR}/src/appl_commands.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/appl_commands.c  -o ${OBJECTDIR}/src/appl_commands.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/appl_commands.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/appl_commands_table.o: src/appl_commands_table.c  .generated_files/flags/debug/ab06821112cfe489c88e81aa000e854087023b0f .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/appl_commands_table.o.d 
	@${RM} ${OBJECTDIR}/src/appl_commands_table.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/appl_commands_table.c  -o ${OBJECTDIR}/src/appl_commands_table.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/appl_commands_table.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/as3993.o: src/as3993.c  .generated_files/flags/debug/775c8ace12c5565da6176abf779226a9d61e076d .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/as3993.o.d 
	@${RM} ${OBJECTDIR}/src/as3993.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/as3993.c  -o ${OBJECTDIR}/src/as3993.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/as3993.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/bootloadable.o: src/bootloadable.c  .generated_files/flags/debug/56423fddac1a0b54d258215a24d9fb8ae0781ed9 .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/bootloadable.o.d 
	@${RM} ${OBJECTDIR}/src/bootloadable.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/bootloadable.c  -o ${OBJECTDIR}/src/bootloadable.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/bootloadable.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/cancelas.o: src/cancelas.c  .generated_files/flags/debug/6f25d468a4f0e415c64589444249a2f981440c2e .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/cancelas.o.d 
	@${RM} ${OBJECTDIR}/src/cancelas.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/cancelas.c  -o ${OBJECTDIR}/src/cancelas.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/cancelas.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/config_i2c3.o: src/config_i2c3.c  .generated_files/flags/debug/b00d98947fd6fb5d32b748d8e4724bdf1598647a .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/config_i2c3.o.d 
	@${RM} ${OBJECTDIR}/src/config_i2c3.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/config_i2c3.c  -o ${OBJECTDIR}/src/config_i2c3.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/config_i2c3.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/empilhadeira.o: src/empilhadeira.c  .generated_files/flags/debug/efb25784f4b994f65db0657aeb1aad955a84a19e .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/empilhadeira.o.d 
	@${RM} ${OBJECTDIR}/src/empilhadeira.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/empilhadeira.c  -o ${OBJECTDIR}/src/empilhadeira.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/empilhadeira.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/eth.o: src/eth.c  .generated_files/flags/debug/4b38d25040fe30416a10f000a93af8df20cb0395 .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/eth.o.d 
	@${RM} ${OBJECTDIR}/src/eth.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/eth.c  -o ${OBJECTDIR}/src/eth.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/eth.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/exclusao.o: src/exclusao.c  .generated_files/flags/debug/1a690622454422dee38c651cae1444f187fbc811 .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/exclusao.o.d 
	@${RM} ${OBJECTDIR}/src/exclusao.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/exclusao.c  -o ${OBJECTDIR}/src/exclusao.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/exclusao.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/FSM_DataHora.o: src/FSM_DataHora.c  .generated_files/flags/debug/22b962583a23fee44feca2bb9d4da90326f44461 .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/FSM_DataHora.o.d 
	@${RM} ${OBJECTDIR}/src/FSM_DataHora.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FSM_DataHora.c  -o ${OBJECTDIR}/src/FSM_DataHora.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/FSM_DataHora.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/gen2.o: src/gen2.c  .generated_files/flags/debug/9d49a73f461b44fbb7e2230ea0aea7afafe6ff94 .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/gen2.o.d 
	@${RM} ${OBJECTDIR}/src/gen2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/gen2.c  -o ${OBJECTDIR}/src/gen2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/gen2.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/global.o: src/global.c  .generated_files/flags/debug/25bdd6c56b3a06a4eedddd135bed6fc4315b4b1c .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/global.o.d 
	@${RM} ${OBJECTDIR}/src/global.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/global.c  -o ${OBJECTDIR}/src/global.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/global.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/i2c.o: src/i2c.c  .generated_files/flags/debug/995ce713b3e38dd423e0874fd31ba100a33ed329 .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/i2c.o.d 
	@${RM} ${OBJECTDIR}/src/i2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/i2c.c  -o ${OBJECTDIR}/src/i2c.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/i2c.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/logger.o: src/logger.c  .generated_files/flags/debug/ecb2e56f209fc6d34e74fc5cb4103160b021ea9a .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/logger.o.d 
	@${RM} ${OBJECTDIR}/src/logger.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/logger.c  -o ${OBJECTDIR}/src/logger.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/logger.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/main.o: src/main.c  .generated_files/flags/debug/ba873d02b86d2344daf28a869b06d2cc03a76c8b .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/main.o.d 
	@${RM} ${OBJECTDIR}/src/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/main.c  -o ${OBJECTDIR}/src/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/mem_i2c_24LC256.o: src/mem_i2c_24LC256.c  .generated_files/flags/debug/df2f0fad4c6b49614e8be5eb6fb26723d2f6985a .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/mem_i2c_24LC256.o.d 
	@${RM} ${OBJECTDIR}/src/mem_i2c_24LC256.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/mem_i2c_24LC256.c  -o ${OBJECTDIR}/src/mem_i2c_24LC256.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/mem_i2c_24LC256.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/onewire.o: src/onewire.c  .generated_files/flags/debug/a8ed737f12e21607268a55ee321addc13248689f .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/onewire.o.d 
	@${RM} ${OBJECTDIR}/src/onewire.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/onewire.c  -o ${OBJECTDIR}/src/onewire.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/onewire.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/perifericos.o: src/perifericos.c  .generated_files/flags/debug/1e8d30d9560ca81b8c8c02b2764b7014cff3b7 .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/perifericos.o.d 
	@${RM} ${OBJECTDIR}/src/perifericos.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/perifericos.c  -o ${OBJECTDIR}/src/perifericos.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/perifericos.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/platform.o: src/platform.c  .generated_files/flags/debug/93290555631f7541c3e3a4efc7980a4ef7210c3a .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/platform.o.d 
	@${RM} ${OBJECTDIR}/src/platform.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/platform.c  -o ${OBJECTDIR}/src/platform.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/platform.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/portal.o: src/portal.c  .generated_files/flags/debug/68234bf96ad029d693444f3e8d23fc837d0c021f .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/portal.o.d 
	@${RM} ${OBJECTDIR}/src/portal.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/portal.c  -o ${OBJECTDIR}/src/portal.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/portal.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/rtc.o: src/rtc.c  .generated_files/flags/debug/7d72ad36e6d998025f37b059ae8de8f391c91e62 .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/rtc.o.d 
	@${RM} ${OBJECTDIR}/src/rtc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/rtc.c  -o ${OBJECTDIR}/src/rtc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/rtc.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/setup_usb.o: src/setup_usb.c  .generated_files/flags/debug/234d746f203fe83101ef141ade95549314bd98c4 .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/setup_usb.o.d 
	@${RM} ${OBJECTDIR}/src/setup_usb.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/setup_usb.c  -o ${OBJECTDIR}/src/setup_usb.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/setup_usb.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/spi_driver.o: src/spi_driver.c  .generated_files/flags/debug/7d599f0a5cdb4ce2f2221854586ecc51d28cf2d8 .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/spi_driver.o.d 
	@${RM} ${OBJECTDIR}/src/spi_driver.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/spi_driver.c  -o ${OBJECTDIR}/src/spi_driver.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/spi_driver.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/stream_dispatcher.o: src/stream_dispatcher.c  .generated_files/flags/debug/7505e38397bc1ce7e5ea66d50e030f76bb67fcfe .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/stream_dispatcher.o.d 
	@${RM} ${OBJECTDIR}/src/stream_dispatcher.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/stream_dispatcher.c  -o ${OBJECTDIR}/src/stream_dispatcher.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/stream_dispatcher.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/system_clock.o: src/system_clock.c  .generated_files/flags/debug/1b7fd1813cf7d614732f38b88397c55841b3ccf2 .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/system_clock.o.d 
	@${RM} ${OBJECTDIR}/src/system_clock.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/system_clock.c  -o ${OBJECTDIR}/src/system_clock.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/system_clock.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/tags.o: src/tags.c  .generated_files/flags/debug/876561616445b2e04bea2a46b65a03c66f23174b .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/tags.o.d 
	@${RM} ${OBJECTDIR}/src/tags.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/tags.c  -o ${OBJECTDIR}/src/tags.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/tags.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/timer.o: src/timer.c  .generated_files/flags/debug/65b7908a2a5f0e29dc0b8b2423f32fe8bd333b3b .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/timer.o.d 
	@${RM} ${OBJECTDIR}/src/timer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/timer.c  -o ${OBJECTDIR}/src/timer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/timer.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/tuner.o: src/tuner.c  .generated_files/flags/debug/5176c38138383c2311a5f905145a855987d4fbfe .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/tuner.o.d 
	@${RM} ${OBJECTDIR}/src/tuner.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/tuner.c  -o ${OBJECTDIR}/src/tuner.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/tuner.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/uart_driver.o: src/uart_driver.c  .generated_files/flags/debug/ee4fecc62c87aeab08628ed24cc671c4425ee191 .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/uart_driver.o.d 
	@${RM} ${OBJECTDIR}/src/uart_driver.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/uart_driver.c  -o ${OBJECTDIR}/src/uart_driver.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/uart_driver.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/usb_descriptors.o: src/usb_descriptors.c  .generated_files/flags/debug/fbb2ea76431fa162b039cd5cb8319d0e9ed1e429 .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/src/usb_descriptors.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/usb_descriptors.c  -o ${OBJECTDIR}/src/usb_descriptors.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/usb_descriptors.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/usb_device.o: src/usb_device.c  .generated_files/flags/debug/9dc844c94dab9fe322836df0d79043de939944c4 .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/usb_device.o.d 
	@${RM} ${OBJECTDIR}/src/usb_device.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/usb_device.c  -o ${OBJECTDIR}/src/usb_device.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/usb_device.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/usb_function_hid.o: src/usb_function_hid.c  .generated_files/flags/debug/84bf9cfa03be91b7dd74af3f8fc3fee45d090c2c .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/usb_function_hid.o.d 
	@${RM} ${OBJECTDIR}/src/usb_function_hid.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/usb_function_hid.c  -o ${OBJECTDIR}/src/usb_function_hid.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/usb_function_hid.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/usb_hal_pic24.o: src/usb_hal_pic24.c  .generated_files/flags/debug/31742e2a7a18a89c2dd4710cb253f7739cfd71d .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/usb_hal_pic24.o.d 
	@${RM} ${OBJECTDIR}/src/usb_hal_pic24.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/usb_hal_pic24.c  -o ${OBJECTDIR}/src/usb_hal_pic24.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/usb_hal_pic24.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/usb_hid_stream_driver.o: src/usb_hid_stream_driver.c  .generated_files/flags/debug/a52b84c2be393409e3228c02e5615771ec68d1c0 .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/usb_hid_stream_driver.o.d 
	@${RM} ${OBJECTDIR}/src/usb_hid_stream_driver.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/usb_hid_stream_driver.c  -o ${OBJECTDIR}/src/usb_hid_stream_driver.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/usb_hid_stream_driver.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/weak_stream_functions.o: src/weak_stream_functions.c  .generated_files/flags/debug/714048c079f0fbe4c78785cd72717dd93a2e565f .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/weak_stream_functions.o.d 
	@${RM} ${OBJECTDIR}/src/weak_stream_functions.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/weak_stream_functions.c  -o ${OBJECTDIR}/src/weak_stream_functions.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/weak_stream_functions.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/barreiraIR.o: src/barreiraIR.c  .generated_files/flags/debug/b943ff14d3959289a09f106b52537630a10e5ad1 .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/barreiraIR.o.d 
	@${RM} ${OBJECTDIR}/src/barreiraIR.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/barreiraIR.c  -o ${OBJECTDIR}/src/barreiraIR.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/barreiraIR.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
else
${OBJECTDIR}/src/ajuste_cap.o: src/ajuste_cap.c  .generated_files/flags/debug/f2dc97c904ddb3cb94b6ca18377863e3b5023819 .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/ajuste_cap.o.d 
	@${RM} ${OBJECTDIR}/src/ajuste_cap.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/ajuste_cap.c  -o ${OBJECTDIR}/src/ajuste_cap.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/ajuste_cap.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/appl_commands.o: src/appl_commands.c  .generated_files/flags/debug/e7b2dd451d9fe5ee740a3173cdc7856d07fb085d .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/appl_commands.o.d 
	@${RM} ${OBJECTDIR}/src/appl_commands.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/appl_commands.c  -o ${OBJECTDIR}/src/appl_commands.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/appl_commands.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/appl_commands_table.o: src/appl_commands_table.c  .generated_files/flags/debug/109c5254efa15ea212d90f7e33285ffffe4f7f11 .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/appl_commands_table.o.d 
	@${RM} ${OBJECTDIR}/src/appl_commands_table.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/appl_commands_table.c  -o ${OBJECTDIR}/src/appl_commands_table.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/appl_commands_table.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/as3993.o: src/as3993.c  .generated_files/flags/debug/d7adfb0abc03e718d77483d92d321864e9d03ba .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/as3993.o.d 
	@${RM} ${OBJECTDIR}/src/as3993.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/as3993.c  -o ${OBJECTDIR}/src/as3993.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/as3993.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/bootloadable.o: src/bootloadable.c  .generated_files/flags/debug/c7bfa70a20b3f33bd88438452f9d2578a822ee61 .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/bootloadable.o.d 
	@${RM} ${OBJECTDIR}/src/bootloadable.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/bootloadable.c  -o ${OBJECTDIR}/src/bootloadable.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/bootloadable.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/cancelas.o: src/cancelas.c  .generated_files/flags/debug/f03a8884a3165d586c2c319ec02f4493a24ebe08 .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/cancelas.o.d 
	@${RM} ${OBJECTDIR}/src/cancelas.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/cancelas.c  -o ${OBJECTDIR}/src/cancelas.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/cancelas.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/config_i2c3.o: src/config_i2c3.c  .generated_files/flags/debug/d22e1c1b28f3d75338fec73daa029f272aa6d813 .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/config_i2c3.o.d 
	@${RM} ${OBJECTDIR}/src/config_i2c3.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/config_i2c3.c  -o ${OBJECTDIR}/src/config_i2c3.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/config_i2c3.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/empilhadeira.o: src/empilhadeira.c  .generated_files/flags/debug/faab1bd284224d9aa05e9a38b53f9fd6fd61f4a7 .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/empilhadeira.o.d 
	@${RM} ${OBJECTDIR}/src/empilhadeira.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/empilhadeira.c  -o ${OBJECTDIR}/src/empilhadeira.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/empilhadeira.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/eth.o: src/eth.c  .generated_files/flags/debug/2480819361141d16b7a20fa66e174c4ab1eff15d .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/eth.o.d 
	@${RM} ${OBJECTDIR}/src/eth.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/eth.c  -o ${OBJECTDIR}/src/eth.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/eth.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/exclusao.o: src/exclusao.c  .generated_files/flags/debug/8ee380cbc0cd2fd32e6c7781b69e00461043cc0f .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/exclusao.o.d 
	@${RM} ${OBJECTDIR}/src/exclusao.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/exclusao.c  -o ${OBJECTDIR}/src/exclusao.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/exclusao.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/FSM_DataHora.o: src/FSM_DataHora.c  .generated_files/flags/debug/44e9a7dfb9a141206ad5763175cd71f48e7befad .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/FSM_DataHora.o.d 
	@${RM} ${OBJECTDIR}/src/FSM_DataHora.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FSM_DataHora.c  -o ${OBJECTDIR}/src/FSM_DataHora.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/FSM_DataHora.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/gen2.o: src/gen2.c  .generated_files/flags/debug/82fb292b7c499cfaff160cff4eb119faa6a26c73 .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/gen2.o.d 
	@${RM} ${OBJECTDIR}/src/gen2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/gen2.c  -o ${OBJECTDIR}/src/gen2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/gen2.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/global.o: src/global.c  .generated_files/flags/debug/c7e1d51e2061140a712ce703a8309f50231cf6ce .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/global.o.d 
	@${RM} ${OBJECTDIR}/src/global.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/global.c  -o ${OBJECTDIR}/src/global.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/global.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/i2c.o: src/i2c.c  .generated_files/flags/debug/56c7ec6383275f3c1a16fed574704c7a75b212c8 .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/i2c.o.d 
	@${RM} ${OBJECTDIR}/src/i2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/i2c.c  -o ${OBJECTDIR}/src/i2c.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/i2c.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/logger.o: src/logger.c  .generated_files/flags/debug/f3095895e517ad7f8a817dd51448d77f907a7722 .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/logger.o.d 
	@${RM} ${OBJECTDIR}/src/logger.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/logger.c  -o ${OBJECTDIR}/src/logger.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/logger.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/main.o: src/main.c  .generated_files/flags/debug/6079f64ce4157859d48876dd8506691e6e28f2f .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/main.o.d 
	@${RM} ${OBJECTDIR}/src/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/main.c  -o ${OBJECTDIR}/src/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/main.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/mem_i2c_24LC256.o: src/mem_i2c_24LC256.c  .generated_files/flags/debug/cc6d5d9b3df9c26d8375a66b9e4e81225c10a9e6 .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/mem_i2c_24LC256.o.d 
	@${RM} ${OBJECTDIR}/src/mem_i2c_24LC256.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/mem_i2c_24LC256.c  -o ${OBJECTDIR}/src/mem_i2c_24LC256.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/mem_i2c_24LC256.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/onewire.o: src/onewire.c  .generated_files/flags/debug/d447d009cffac6ee97470b138db4ccecfb328b67 .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/onewire.o.d 
	@${RM} ${OBJECTDIR}/src/onewire.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/onewire.c  -o ${OBJECTDIR}/src/onewire.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/onewire.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/perifericos.o: src/perifericos.c  .generated_files/flags/debug/dc7374d9ad9ee47d23e2285637b415569422d1c8 .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/perifericos.o.d 
	@${RM} ${OBJECTDIR}/src/perifericos.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/perifericos.c  -o ${OBJECTDIR}/src/perifericos.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/perifericos.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/platform.o: src/platform.c  .generated_files/flags/debug/8a517785432355092cd6f0ce586706c6088d805b .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/platform.o.d 
	@${RM} ${OBJECTDIR}/src/platform.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/platform.c  -o ${OBJECTDIR}/src/platform.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/platform.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/portal.o: src/portal.c  .generated_files/flags/debug/a1e8d47c59e3244bd0236271d8de87d3672cf85f .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/portal.o.d 
	@${RM} ${OBJECTDIR}/src/portal.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/portal.c  -o ${OBJECTDIR}/src/portal.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/portal.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/rtc.o: src/rtc.c  .generated_files/flags/debug/28d854f565927772d1caf3080763661d7c4a4baa .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/rtc.o.d 
	@${RM} ${OBJECTDIR}/src/rtc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/rtc.c  -o ${OBJECTDIR}/src/rtc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/rtc.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/setup_usb.o: src/setup_usb.c  .generated_files/flags/debug/568f8d2fb47f781b7ee979fc19d0ba1686d57455 .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/setup_usb.o.d 
	@${RM} ${OBJECTDIR}/src/setup_usb.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/setup_usb.c  -o ${OBJECTDIR}/src/setup_usb.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/setup_usb.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/spi_driver.o: src/spi_driver.c  .generated_files/flags/debug/d64fc64d81187a9d31dba21247363bafea00aac7 .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/spi_driver.o.d 
	@${RM} ${OBJECTDIR}/src/spi_driver.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/spi_driver.c  -o ${OBJECTDIR}/src/spi_driver.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/spi_driver.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/stream_dispatcher.o: src/stream_dispatcher.c  .generated_files/flags/debug/2e7a337b331eff10a6d89f5719998c212609209e .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/stream_dispatcher.o.d 
	@${RM} ${OBJECTDIR}/src/stream_dispatcher.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/stream_dispatcher.c  -o ${OBJECTDIR}/src/stream_dispatcher.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/stream_dispatcher.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/system_clock.o: src/system_clock.c  .generated_files/flags/debug/2bc7d53b5ab19b779e53d03158f2f13e3fd24d33 .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/system_clock.o.d 
	@${RM} ${OBJECTDIR}/src/system_clock.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/system_clock.c  -o ${OBJECTDIR}/src/system_clock.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/system_clock.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/tags.o: src/tags.c  .generated_files/flags/debug/7884580886d36027b9467cb96d0c613149114ace .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/tags.o.d 
	@${RM} ${OBJECTDIR}/src/tags.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/tags.c  -o ${OBJECTDIR}/src/tags.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/tags.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/timer.o: src/timer.c  .generated_files/flags/debug/fcbd9d8a00ff8a51e1785e66236f8b8b37718e55 .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/timer.o.d 
	@${RM} ${OBJECTDIR}/src/timer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/timer.c  -o ${OBJECTDIR}/src/timer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/timer.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/tuner.o: src/tuner.c  .generated_files/flags/debug/7408b857ad1bec2238a26c42d99e23481ef8eb2d .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/tuner.o.d 
	@${RM} ${OBJECTDIR}/src/tuner.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/tuner.c  -o ${OBJECTDIR}/src/tuner.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/tuner.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/uart_driver.o: src/uart_driver.c  .generated_files/flags/debug/e70f4020fccdb614ce300fbf0ca0bd20770c060 .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/uart_driver.o.d 
	@${RM} ${OBJECTDIR}/src/uart_driver.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/uart_driver.c  -o ${OBJECTDIR}/src/uart_driver.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/uart_driver.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/usb_descriptors.o: src/usb_descriptors.c  .generated_files/flags/debug/6a706c3e7a88a520bef486781750dbea2f5d6c11 .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/src/usb_descriptors.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/usb_descriptors.c  -o ${OBJECTDIR}/src/usb_descriptors.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/usb_descriptors.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/usb_device.o: src/usb_device.c  .generated_files/flags/debug/2db0aacdab6ff54fdced210b2333699459d29918 .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/usb_device.o.d 
	@${RM} ${OBJECTDIR}/src/usb_device.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/usb_device.c  -o ${OBJECTDIR}/src/usb_device.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/usb_device.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/usb_function_hid.o: src/usb_function_hid.c  .generated_files/flags/debug/a3c3c83ff63d24f9ae47eb3d55c9400242605f76 .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/usb_function_hid.o.d 
	@${RM} ${OBJECTDIR}/src/usb_function_hid.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/usb_function_hid.c  -o ${OBJECTDIR}/src/usb_function_hid.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/usb_function_hid.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/usb_hal_pic24.o: src/usb_hal_pic24.c  .generated_files/flags/debug/d918dd45f3feac9633575467c32181edf79495f0 .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/usb_hal_pic24.o.d 
	@${RM} ${OBJECTDIR}/src/usb_hal_pic24.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/usb_hal_pic24.c  -o ${OBJECTDIR}/src/usb_hal_pic24.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/usb_hal_pic24.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/usb_hid_stream_driver.o: src/usb_hid_stream_driver.c  .generated_files/flags/debug/d51b2a7a3c9388aea0b0c9718e657221941c8edf .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/usb_hid_stream_driver.o.d 
	@${RM} ${OBJECTDIR}/src/usb_hid_stream_driver.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/usb_hid_stream_driver.c  -o ${OBJECTDIR}/src/usb_hid_stream_driver.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/usb_hid_stream_driver.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/weak_stream_functions.o: src/weak_stream_functions.c  .generated_files/flags/debug/e1722f31914d6da54261355f3aa29b2a15d2255a .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/weak_stream_functions.o.d 
	@${RM} ${OBJECTDIR}/src/weak_stream_functions.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/weak_stream_functions.c  -o ${OBJECTDIR}/src/weak_stream_functions.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/weak_stream_functions.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/barreiraIR.o: src/barreiraIR.c  .generated_files/flags/debug/6adbddb841fd6fa0600151f22748a87bb4688927 .generated_files/flags/debug/c4232f2312e8c616988f20a63e0e72bed8437aa9
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/barreiraIR.o.d 
	@${RM} ${OBJECTDIR}/src/barreiraIR.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/barreiraIR.c  -o ${OBJECTDIR}/src/barreiraIR.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/barreiraIR.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -fno-short-double -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -DUSE_LOGGER -msmart-io=1 -Wall -msfr-warn=off   
	
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
dist/${CND_CONF}/${IMAGE_TYPE}/FWPORTAL.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    src/p24FJ256DA210.gld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/FWPORTAL.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)   -mreserve=data@0x800:0x81B -mreserve=data@0x81C:0x81D -mreserve=data@0x81E:0x81F -mreserve=data@0x820:0x821 -mreserve=data@0x822:0x823 -mreserve=data@0x824:0x827 -mreserve=data@0x82A:0x84F   -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,--defsym=__MPLAB_DEBUGGER_PK3=1,$(MP_LINKER_FILE_OPTION),--heap=1950,--no-check-sections,--data-init,--pack-data,--handles,--isr,--gc-sections,--fill-upper=0,--stackguard=16,--library-path="..",--no-force-link,--smart-io,-Map="$(BINDIR_)$(TARGETBASE).map",--report-mem$(MP_EXTRA_LD_POST)  
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/FWPORTAL.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   src/p24FJ256DA210.gld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/FWPORTAL.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_debug=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--heap=1950,--no-check-sections,--data-init,--pack-data,--handles,--isr,--gc-sections,--fill-upper=0,--stackguard=16,--library-path="..",--no-force-link,--smart-io,-Map="$(BINDIR_)$(TARGETBASE).map",--report-mem$(MP_EXTRA_LD_POST)  
	${MP_CC_DIR}\\xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/FWPORTAL.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf   
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/debug
	${RM} -r dist/debug

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
