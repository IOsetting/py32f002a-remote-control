##### Project #####

PROJECT			?= remote-controller
# `$(shell pwd)` or `.`, both works
TOP			= ../..
# The path for generated files
BUILD_DIR		= Build

# MCU types: 
#   PY32F002Ax5
#   PY32F003x6, PY32F003x8, 
#   PY32F030x6, PY32F030x8, 
MCU_TYPE		= PY32F030x6

##### Options #####

# Enable printf float %f support, y:yes, n:no
ENABLE_PRINTF_FLOAT	?= n
# Programmer, jlink or pyocd
FLASH_PROGRM	?= jlink
# Wireless part:
#   XL2400, XL2400P, XN297L
WIRELESS_PART	?= XL2400P

##### Toolchains #######

ARM_TOOCHAIN	?= /opt/gcc-arm/arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-eabi/bin

# path to JLinkExe
JLINKEXE		?= /opt/SEGGER/JLink/JLinkExe
# path to PyOCD
PYOCD_EXE		?= pyocd

##### Paths ############

# C source folders
CDIRS	:= \
		Apps/RemoteController/Driver \
		Apps/RemoteController/User
		

# C source files (if there are any single ones)
CFILES := \
		Hardware/74hc165.c \
		Hardware/fonts.c \
		Hardware/SEGGER_RTT_printf.c \
		Hardware/SEGGER_RTT.c \
		Hardware/st7567.c

ifeq ($(WIRELESS_PART),XL2400)
CFILES		+= Hardware/xl2400.c
else ifeq ($(WIRELESS_PART),XL2400P)
CFILES		+= Hardware/xl2400p.c
else ifeq ($(WIRELESS_PART),XN297L)
CFILES		+= Hardware/xn297l.c
endif

# ASM source folders
ADIRS	:= Apps/RemoteController/User
# ASM single files
AFILES	:= 

# Include paths
INCLUDES	:= Libraries/CMSIS/Core/Include \
			Libraries/CMSIS/Device/PY32F0xx/Include \
			Hardware \
			Apps/RemoteController/Driver \
			Apps/RemoteController/User

##### Library Paths ############

# Library flags
LIB_FLAGS		= $(MCU_TYPE) USE_$(WIRELESS_PART)
# JLink device (Uppercases)
JLINK_DEVICE	?= $(shell echo $(MCU_TYPE) | tr '[:lower:]' '[:upper:]')
# PyOCD device (Lowercases)
PYOCD_DEVICE	?= $(shell echo $(MCU_TYPE) | tr '[:upper:]' '[:lower:]')
# Link descript file: 
LDSCRIPT		= Libraries/LDScripts/$(PYOCD_DEVICE).ld


CFILES		+= Libraries/CMSIS/Device/PY32F0xx/Source/system_py32f0xx.c

CDIRS		+= Libraries/PY32F0xx_LL_Driver/Src \
			Libraries/PY32F0xx_LL_BSP/Src 
INCLUDES	+= Libraries/PY32F0xx_LL_Driver/Inc \
			Libraries/PY32F0xx_LL_BSP/Inc 
LIB_FLAGS   += USE_FULL_LL_DRIVER

# Startup file
AFILES	:= Libraries/CMSIS/Device/PY32F0xx/Source/gcc/startup_py32f030.s

include ../../rules.mk
