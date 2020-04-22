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
ifeq "$(wildcard nbproject/Makefile-local-release.mk)" "nbproject/Makefile-local-release.mk"
include nbproject/Makefile-local-release.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=release
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/EMPI_AMBEV.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/EMPI_AMBEV.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../src/as3993.c ../src/gen2.c ../src/global.c ../../../common/firmware/microchip/src/logger.c ../src/main.c ../src/timer.c ../src/tuner.c ../../../common/firmware/microchip/src/uart_driver.c ../src/usb_descriptors.c ../../../common/firmware/microchip/src/usb_device.c ../../../common/firmware/microchip/src/usb_function_hid.c ../../../common/firmware/microchip/src/usb_hal_pic24.c ../../../common/firmware/microchip/src/usb_hid_stream_driver.c ../../../common/firmware/microchip/src/bootloadable.c ../../../common/firmware/microchip/src/spi_driver.c ../src/platform.c ../src/appl_commands.c ../src/appl_commands_table.c ../../../common/firmware/microchip/src/stream_dispatcher.c ../../../common/firmware/microchip/src/weak_stream_functions.c ../../../common/firmware/microchip/src/system_clock.c ../src/lcd.c ../src/gprs.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1360937237/as3993.o ${OBJECTDIR}/_ext/1360937237/gen2.o ${OBJECTDIR}/_ext/1360937237/global.o ${OBJECTDIR}/_ext/1611813562/logger.o ${OBJECTDIR}/_ext/1360937237/main.o ${OBJECTDIR}/_ext/1360937237/timer.o ${OBJECTDIR}/_ext/1360937237/tuner.o ${OBJECTDIR}/_ext/1611813562/uart_driver.o ${OBJECTDIR}/_ext/1360937237/usb_descriptors.o ${OBJECTDIR}/_ext/1611813562/usb_device.o ${OBJECTDIR}/_ext/1611813562/usb_function_hid.o ${OBJECTDIR}/_ext/1611813562/usb_hal_pic24.o ${OBJECTDIR}/_ext/1611813562/usb_hid_stream_driver.o ${OBJECTDIR}/_ext/1611813562/bootloadable.o ${OBJECTDIR}/_ext/1611813562/spi_driver.o ${OBJECTDIR}/_ext/1360937237/platform.o ${OBJECTDIR}/_ext/1360937237/appl_commands.o ${OBJECTDIR}/_ext/1360937237/appl_commands_table.o ${OBJECTDIR}/_ext/1611813562/stream_dispatcher.o ${OBJECTDIR}/_ext/1611813562/weak_stream_functions.o ${OBJECTDIR}/_ext/1611813562/system_clock.o ${OBJECTDIR}/_ext/1360937237/lcd.o ${OBJECTDIR}/_ext/1360937237/gprs.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1360937237/as3993.o.d ${OBJECTDIR}/_ext/1360937237/gen2.o.d ${OBJECTDIR}/_ext/1360937237/global.o.d ${OBJECTDIR}/_ext/1611813562/logger.o.d ${OBJECTDIR}/_ext/1360937237/main.o.d ${OBJECTDIR}/_ext/1360937237/timer.o.d ${OBJECTDIR}/_ext/1360937237/tuner.o.d ${OBJECTDIR}/_ext/1611813562/uart_driver.o.d ${OBJECTDIR}/_ext/1360937237/usb_descriptors.o.d ${OBJECTDIR}/_ext/1611813562/usb_device.o.d ${OBJECTDIR}/_ext/1611813562/usb_function_hid.o.d ${OBJECTDIR}/_ext/1611813562/usb_hal_pic24.o.d ${OBJECTDIR}/_ext/1611813562/usb_hid_stream_driver.o.d ${OBJECTDIR}/_ext/1611813562/bootloadable.o.d ${OBJECTDIR}/_ext/1611813562/spi_driver.o.d ${OBJECTDIR}/_ext/1360937237/platform.o.d ${OBJECTDIR}/_ext/1360937237/appl_commands.o.d ${OBJECTDIR}/_ext/1360937237/appl_commands_table.o.d ${OBJECTDIR}/_ext/1611813562/stream_dispatcher.o.d ${OBJECTDIR}/_ext/1611813562/weak_stream_functions.o.d ${OBJECTDIR}/_ext/1611813562/system_clock.o.d ${OBJECTDIR}/_ext/1360937237/lcd.o.d ${OBJECTDIR}/_ext/1360937237/gprs.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1360937237/as3993.o ${OBJECTDIR}/_ext/1360937237/gen2.o ${OBJECTDIR}/_ext/1360937237/global.o ${OBJECTDIR}/_ext/1611813562/logger.o ${OBJECTDIR}/_ext/1360937237/main.o ${OBJECTDIR}/_ext/1360937237/timer.o ${OBJECTDIR}/_ext/1360937237/tuner.o ${OBJECTDIR}/_ext/1611813562/uart_driver.o ${OBJECTDIR}/_ext/1360937237/usb_descriptors.o ${OBJECTDIR}/_ext/1611813562/usb_device.o ${OBJECTDIR}/_ext/1611813562/usb_function_hid.o ${OBJECTDIR}/_ext/1611813562/usb_hal_pic24.o ${OBJECTDIR}/_ext/1611813562/usb_hid_stream_driver.o ${OBJECTDIR}/_ext/1611813562/bootloadable.o ${OBJECTDIR}/_ext/1611813562/spi_driver.o ${OBJECTDIR}/_ext/1360937237/platform.o ${OBJECTDIR}/_ext/1360937237/appl_commands.o ${OBJECTDIR}/_ext/1360937237/appl_commands_table.o ${OBJECTDIR}/_ext/1611813562/stream_dispatcher.o ${OBJECTDIR}/_ext/1611813562/weak_stream_functions.o ${OBJECTDIR}/_ext/1611813562/system_clock.o ${OBJECTDIR}/_ext/1360937237/lcd.o ${OBJECTDIR}/_ext/1360937237/gprs.o

# Source Files
SOURCEFILES=../src/as3993.c ../src/gen2.c ../src/global.c ../../../common/firmware/microchip/src/logger.c ../src/main.c ../src/timer.c ../src/tuner.c ../../../common/firmware/microchip/src/uart_driver.c ../src/usb_descriptors.c ../../../common/firmware/microchip/src/usb_device.c ../../../common/firmware/microchip/src/usb_function_hid.c ../../../common/firmware/microchip/src/usb_hal_pic24.c ../../../common/firmware/microchip/src/usb_hid_stream_driver.c ../../../common/firmware/microchip/src/bootloadable.c ../../../common/firmware/microchip/src/spi_driver.c ../src/platform.c ../src/appl_commands.c ../src/appl_commands_table.c ../../../common/firmware/microchip/src/stream_dispatcher.c ../../../common/firmware/microchip/src/weak_stream_functions.c ../../../common/firmware/microchip/src/system_clock.c ../src/lcd.c ../src/gprs.c


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
	${MAKE}  -f nbproject/Makefile-release.mk dist/${CND_CONF}/${IMAGE_TYPE}/EMPI_AMBEV.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24FJ256DA210
MP_LINKER_FILE_OPTION=,--script="..\src\p24FJ256DA210.gld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1360937237/as3993.o: ../src/as3993.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/as3993.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/as3993.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/as3993.c  -o ${OBJECTDIR}/_ext/1360937237/as3993.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360937237/as3993.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/as3993.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1360937237/gen2.o: ../src/gen2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/gen2.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/gen2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/gen2.c  -o ${OBJECTDIR}/_ext/1360937237/gen2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360937237/gen2.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/gen2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1360937237/global.o: ../src/global.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/global.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/global.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/global.c  -o ${OBJECTDIR}/_ext/1360937237/global.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360937237/global.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/global.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1611813562/logger.o: ../../../common/firmware/microchip/src/logger.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1611813562 
	@${RM} ${OBJECTDIR}/_ext/1611813562/logger.o.d 
	@${RM} ${OBJECTDIR}/_ext/1611813562/logger.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../common/firmware/microchip/src/logger.c  -o ${OBJECTDIR}/_ext/1611813562/logger.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1611813562/logger.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1611813562/logger.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/main.c  -o ${OBJECTDIR}/_ext/1360937237/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1360937237/timer.o: ../src/timer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/timer.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/timer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/timer.c  -o ${OBJECTDIR}/_ext/1360937237/timer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360937237/timer.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/timer.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1360937237/tuner.o: ../src/tuner.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/tuner.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/tuner.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/tuner.c  -o ${OBJECTDIR}/_ext/1360937237/tuner.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360937237/tuner.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/tuner.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1611813562/uart_driver.o: ../../../common/firmware/microchip/src/uart_driver.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1611813562 
	@${RM} ${OBJECTDIR}/_ext/1611813562/uart_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1611813562/uart_driver.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../common/firmware/microchip/src/uart_driver.c  -o ${OBJECTDIR}/_ext/1611813562/uart_driver.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1611813562/uart_driver.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1611813562/uart_driver.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1360937237/usb_descriptors.o: ../src/usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/usb_descriptors.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/usb_descriptors.c  -o ${OBJECTDIR}/_ext/1360937237/usb_descriptors.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360937237/usb_descriptors.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/usb_descriptors.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1611813562/usb_device.o: ../../../common/firmware/microchip/src/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1611813562 
	@${RM} ${OBJECTDIR}/_ext/1611813562/usb_device.o.d 
	@${RM} ${OBJECTDIR}/_ext/1611813562/usb_device.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../common/firmware/microchip/src/usb_device.c  -o ${OBJECTDIR}/_ext/1611813562/usb_device.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1611813562/usb_device.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1611813562/usb_device.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1611813562/usb_function_hid.o: ../../../common/firmware/microchip/src/usb_function_hid.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1611813562 
	@${RM} ${OBJECTDIR}/_ext/1611813562/usb_function_hid.o.d 
	@${RM} ${OBJECTDIR}/_ext/1611813562/usb_function_hid.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../common/firmware/microchip/src/usb_function_hid.c  -o ${OBJECTDIR}/_ext/1611813562/usb_function_hid.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1611813562/usb_function_hid.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1611813562/usb_function_hid.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1611813562/usb_hal_pic24.o: ../../../common/firmware/microchip/src/usb_hal_pic24.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1611813562 
	@${RM} ${OBJECTDIR}/_ext/1611813562/usb_hal_pic24.o.d 
	@${RM} ${OBJECTDIR}/_ext/1611813562/usb_hal_pic24.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../common/firmware/microchip/src/usb_hal_pic24.c  -o ${OBJECTDIR}/_ext/1611813562/usb_hal_pic24.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1611813562/usb_hal_pic24.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1611813562/usb_hal_pic24.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1611813562/usb_hid_stream_driver.o: ../../../common/firmware/microchip/src/usb_hid_stream_driver.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1611813562 
	@${RM} ${OBJECTDIR}/_ext/1611813562/usb_hid_stream_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1611813562/usb_hid_stream_driver.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../common/firmware/microchip/src/usb_hid_stream_driver.c  -o ${OBJECTDIR}/_ext/1611813562/usb_hid_stream_driver.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1611813562/usb_hid_stream_driver.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1611813562/usb_hid_stream_driver.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1611813562/bootloadable.o: ../../../common/firmware/microchip/src/bootloadable.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1611813562 
	@${RM} ${OBJECTDIR}/_ext/1611813562/bootloadable.o.d 
	@${RM} ${OBJECTDIR}/_ext/1611813562/bootloadable.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../common/firmware/microchip/src/bootloadable.c  -o ${OBJECTDIR}/_ext/1611813562/bootloadable.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1611813562/bootloadable.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1611813562/bootloadable.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1611813562/spi_driver.o: ../../../common/firmware/microchip/src/spi_driver.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1611813562 
	@${RM} ${OBJECTDIR}/_ext/1611813562/spi_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1611813562/spi_driver.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../common/firmware/microchip/src/spi_driver.c  -o ${OBJECTDIR}/_ext/1611813562/spi_driver.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1611813562/spi_driver.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1611813562/spi_driver.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1360937237/platform.o: ../src/platform.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/platform.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/platform.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/platform.c  -o ${OBJECTDIR}/_ext/1360937237/platform.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360937237/platform.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/platform.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1360937237/appl_commands.o: ../src/appl_commands.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/appl_commands.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/appl_commands.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/appl_commands.c  -o ${OBJECTDIR}/_ext/1360937237/appl_commands.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360937237/appl_commands.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/appl_commands.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1360937237/appl_commands_table.o: ../src/appl_commands_table.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/appl_commands_table.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/appl_commands_table.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/appl_commands_table.c  -o ${OBJECTDIR}/_ext/1360937237/appl_commands_table.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360937237/appl_commands_table.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/appl_commands_table.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1611813562/stream_dispatcher.o: ../../../common/firmware/microchip/src/stream_dispatcher.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1611813562 
	@${RM} ${OBJECTDIR}/_ext/1611813562/stream_dispatcher.o.d 
	@${RM} ${OBJECTDIR}/_ext/1611813562/stream_dispatcher.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../common/firmware/microchip/src/stream_dispatcher.c  -o ${OBJECTDIR}/_ext/1611813562/stream_dispatcher.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1611813562/stream_dispatcher.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1611813562/stream_dispatcher.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1611813562/weak_stream_functions.o: ../../../common/firmware/microchip/src/weak_stream_functions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1611813562 
	@${RM} ${OBJECTDIR}/_ext/1611813562/weak_stream_functions.o.d 
	@${RM} ${OBJECTDIR}/_ext/1611813562/weak_stream_functions.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../common/firmware/microchip/src/weak_stream_functions.c  -o ${OBJECTDIR}/_ext/1611813562/weak_stream_functions.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1611813562/weak_stream_functions.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1611813562/weak_stream_functions.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1611813562/system_clock.o: ../../../common/firmware/microchip/src/system_clock.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1611813562 
	@${RM} ${OBJECTDIR}/_ext/1611813562/system_clock.o.d 
	@${RM} ${OBJECTDIR}/_ext/1611813562/system_clock.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../common/firmware/microchip/src/system_clock.c  -o ${OBJECTDIR}/_ext/1611813562/system_clock.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1611813562/system_clock.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1611813562/system_clock.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1360937237/lcd.o: ../src/lcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/lcd.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/lcd.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/lcd.c  -o ${OBJECTDIR}/_ext/1360937237/lcd.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360937237/lcd.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/lcd.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1360937237/gprs.o: ../src/gprs.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/gprs.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/gprs.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/gprs.c  -o ${OBJECTDIR}/_ext/1360937237/gprs.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360937237/gprs.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/gprs.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/_ext/1360937237/as3993.o: ../src/as3993.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/as3993.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/as3993.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/as3993.c  -o ${OBJECTDIR}/_ext/1360937237/as3993.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360937237/as3993.o.d"      -mno-eds-warn  -g -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/as3993.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1360937237/gen2.o: ../src/gen2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/gen2.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/gen2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/gen2.c  -o ${OBJECTDIR}/_ext/1360937237/gen2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360937237/gen2.o.d"      -mno-eds-warn  -g -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/gen2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1360937237/global.o: ../src/global.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/global.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/global.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/global.c  -o ${OBJECTDIR}/_ext/1360937237/global.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360937237/global.o.d"      -mno-eds-warn  -g -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/global.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1611813562/logger.o: ../../../common/firmware/microchip/src/logger.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1611813562 
	@${RM} ${OBJECTDIR}/_ext/1611813562/logger.o.d 
	@${RM} ${OBJECTDIR}/_ext/1611813562/logger.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../common/firmware/microchip/src/logger.c  -o ${OBJECTDIR}/_ext/1611813562/logger.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1611813562/logger.o.d"      -mno-eds-warn  -g -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1611813562/logger.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/main.c  -o ${OBJECTDIR}/_ext/1360937237/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d"      -mno-eds-warn  -g -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1360937237/timer.o: ../src/timer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/timer.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/timer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/timer.c  -o ${OBJECTDIR}/_ext/1360937237/timer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360937237/timer.o.d"      -mno-eds-warn  -g -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/timer.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1360937237/tuner.o: ../src/tuner.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/tuner.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/tuner.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/tuner.c  -o ${OBJECTDIR}/_ext/1360937237/tuner.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360937237/tuner.o.d"      -mno-eds-warn  -g -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/tuner.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1611813562/uart_driver.o: ../../../common/firmware/microchip/src/uart_driver.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1611813562 
	@${RM} ${OBJECTDIR}/_ext/1611813562/uart_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1611813562/uart_driver.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../common/firmware/microchip/src/uart_driver.c  -o ${OBJECTDIR}/_ext/1611813562/uart_driver.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1611813562/uart_driver.o.d"      -mno-eds-warn  -g -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1611813562/uart_driver.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1360937237/usb_descriptors.o: ../src/usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/usb_descriptors.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/usb_descriptors.c  -o ${OBJECTDIR}/_ext/1360937237/usb_descriptors.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360937237/usb_descriptors.o.d"      -mno-eds-warn  -g -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/usb_descriptors.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1611813562/usb_device.o: ../../../common/firmware/microchip/src/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1611813562 
	@${RM} ${OBJECTDIR}/_ext/1611813562/usb_device.o.d 
	@${RM} ${OBJECTDIR}/_ext/1611813562/usb_device.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../common/firmware/microchip/src/usb_device.c  -o ${OBJECTDIR}/_ext/1611813562/usb_device.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1611813562/usb_device.o.d"      -mno-eds-warn  -g -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1611813562/usb_device.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1611813562/usb_function_hid.o: ../../../common/firmware/microchip/src/usb_function_hid.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1611813562 
	@${RM} ${OBJECTDIR}/_ext/1611813562/usb_function_hid.o.d 
	@${RM} ${OBJECTDIR}/_ext/1611813562/usb_function_hid.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../common/firmware/microchip/src/usb_function_hid.c  -o ${OBJECTDIR}/_ext/1611813562/usb_function_hid.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1611813562/usb_function_hid.o.d"      -mno-eds-warn  -g -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1611813562/usb_function_hid.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1611813562/usb_hal_pic24.o: ../../../common/firmware/microchip/src/usb_hal_pic24.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1611813562 
	@${RM} ${OBJECTDIR}/_ext/1611813562/usb_hal_pic24.o.d 
	@${RM} ${OBJECTDIR}/_ext/1611813562/usb_hal_pic24.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../common/firmware/microchip/src/usb_hal_pic24.c  -o ${OBJECTDIR}/_ext/1611813562/usb_hal_pic24.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1611813562/usb_hal_pic24.o.d"      -mno-eds-warn  -g -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1611813562/usb_hal_pic24.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1611813562/usb_hid_stream_driver.o: ../../../common/firmware/microchip/src/usb_hid_stream_driver.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1611813562 
	@${RM} ${OBJECTDIR}/_ext/1611813562/usb_hid_stream_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1611813562/usb_hid_stream_driver.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../common/firmware/microchip/src/usb_hid_stream_driver.c  -o ${OBJECTDIR}/_ext/1611813562/usb_hid_stream_driver.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1611813562/usb_hid_stream_driver.o.d"      -mno-eds-warn  -g -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1611813562/usb_hid_stream_driver.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1611813562/bootloadable.o: ../../../common/firmware/microchip/src/bootloadable.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1611813562 
	@${RM} ${OBJECTDIR}/_ext/1611813562/bootloadable.o.d 
	@${RM} ${OBJECTDIR}/_ext/1611813562/bootloadable.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../common/firmware/microchip/src/bootloadable.c  -o ${OBJECTDIR}/_ext/1611813562/bootloadable.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1611813562/bootloadable.o.d"      -mno-eds-warn  -g -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1611813562/bootloadable.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1611813562/spi_driver.o: ../../../common/firmware/microchip/src/spi_driver.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1611813562 
	@${RM} ${OBJECTDIR}/_ext/1611813562/spi_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1611813562/spi_driver.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../common/firmware/microchip/src/spi_driver.c  -o ${OBJECTDIR}/_ext/1611813562/spi_driver.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1611813562/spi_driver.o.d"      -mno-eds-warn  -g -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1611813562/spi_driver.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1360937237/platform.o: ../src/platform.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/platform.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/platform.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/platform.c  -o ${OBJECTDIR}/_ext/1360937237/platform.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360937237/platform.o.d"      -mno-eds-warn  -g -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/platform.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1360937237/appl_commands.o: ../src/appl_commands.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/appl_commands.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/appl_commands.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/appl_commands.c  -o ${OBJECTDIR}/_ext/1360937237/appl_commands.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360937237/appl_commands.o.d"      -mno-eds-warn  -g -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/appl_commands.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1360937237/appl_commands_table.o: ../src/appl_commands_table.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/appl_commands_table.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/appl_commands_table.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/appl_commands_table.c  -o ${OBJECTDIR}/_ext/1360937237/appl_commands_table.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360937237/appl_commands_table.o.d"      -mno-eds-warn  -g -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/appl_commands_table.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1611813562/stream_dispatcher.o: ../../../common/firmware/microchip/src/stream_dispatcher.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1611813562 
	@${RM} ${OBJECTDIR}/_ext/1611813562/stream_dispatcher.o.d 
	@${RM} ${OBJECTDIR}/_ext/1611813562/stream_dispatcher.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../common/firmware/microchip/src/stream_dispatcher.c  -o ${OBJECTDIR}/_ext/1611813562/stream_dispatcher.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1611813562/stream_dispatcher.o.d"      -mno-eds-warn  -g -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1611813562/stream_dispatcher.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1611813562/weak_stream_functions.o: ../../../common/firmware/microchip/src/weak_stream_functions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1611813562 
	@${RM} ${OBJECTDIR}/_ext/1611813562/weak_stream_functions.o.d 
	@${RM} ${OBJECTDIR}/_ext/1611813562/weak_stream_functions.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../common/firmware/microchip/src/weak_stream_functions.c  -o ${OBJECTDIR}/_ext/1611813562/weak_stream_functions.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1611813562/weak_stream_functions.o.d"      -mno-eds-warn  -g -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1611813562/weak_stream_functions.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1611813562/system_clock.o: ../../../common/firmware/microchip/src/system_clock.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1611813562 
	@${RM} ${OBJECTDIR}/_ext/1611813562/system_clock.o.d 
	@${RM} ${OBJECTDIR}/_ext/1611813562/system_clock.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../common/firmware/microchip/src/system_clock.c  -o ${OBJECTDIR}/_ext/1611813562/system_clock.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1611813562/system_clock.o.d"      -mno-eds-warn  -g -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1611813562/system_clock.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1360937237/lcd.o: ../src/lcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/lcd.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/lcd.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/lcd.c  -o ${OBJECTDIR}/_ext/1360937237/lcd.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360937237/lcd.o.d"      -mno-eds-warn  -g -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/lcd.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1360937237/gprs.o: ../src/gprs.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/gprs.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/gprs.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/gprs.c  -o ${OBJECTDIR}/_ext/1360937237/gprs.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360937237/gprs.o.d"      -mno-eds-warn  -g -omf=elf -fno-short-double -mlarge-code -O0 -I"../src" -I"../../../common/include" -I"../../../common/firmware/microchip/include" -I"%c30dir%/support/PIC24F/h" -I"%c30dir%/support/peripheral_24F" -I"%c30dir%/include" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/gprs.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
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
dist/${CND_CONF}/${IMAGE_TYPE}/EMPI_AMBEV.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    ../src/p24FJ256DA210.gld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/EMPI_AMBEV.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf  -mreserve=data@0x800:0x81B -mreserve=data@0x81C:0x81D -mreserve=data@0x81E:0x81F -mreserve=data@0x820:0x821 -mreserve=data@0x822:0x823 -mreserve=data@0x824:0x827 -mreserve=data@0x82A:0x84F   -Wl,--local-stack,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,$(MP_LINKER_FILE_OPTION),--no-check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--library-path="..",--no-force-link,--smart-io,-Map="$(BINDIR_)$(TARGETBASE).map",--report-mem$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/EMPI_AMBEV.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   ../src/p24FJ256DA210.gld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/EMPI_AMBEV.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -Wl,--local-stack,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--no-check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--library-path="..",--no-force-link,--smart-io,-Map="$(BINDIR_)$(TARGETBASE).map",--report-mem$(MP_EXTRA_LD_POST) 
	${MP_CC_DIR}\\xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/EMPI_AMBEV.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf  
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/release
	${RM} -r dist/release

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif