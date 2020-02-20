SHELL=C:/Windows/System32/cmd.exe
# List all source files the application uses.
APPLICATION_SRCS = $(notdir $(wildcard ../*.c))
APPLICATION_SRCS += nrf_delay.c
APPLICATION_SRCS += app_scheduler.c
APPLICATION_SRCS += app_timer.c
APPLICATION_SRCS += app_button.c
APPLICATION_SRCS += app_gpiote.c
APPLICATION_SRCS += ble_dis.c
APPLICATION_SRCS += ble_bas.c
APPLICATION_SRCS += ble_srv_common.c
APPLICATION_SRCS += ble_conn_params.c
APPLICATION_SRCS += ble_advdata.c
APPLICATION_SRCS += ble_bondmngr.c
APPLICATION_SRCS += ble_debug_assert_handler.c
APPLICATION_SRCS += softdevice_handler.c
APPLICATION_SRCS += pstorage.c
APPLICATION_SRCS += simple_uart.c
APPLICATION_SRCS += crc16.c

# Use shell to find name of root folder. Possible but horrible to do in make.
PROJECT_NAME = $(shell basename "$(realpath ../)")

DEVICE = NRF51
BOARD = BOARD_PCA10001
SEGGER_SERIAL = 518004431

USE_SOFTDEVICE = s110
USE_EXT_SENSORS = 1

CFLAGS += -ffunction-sections -fdata-sections
LDFLAGS += -Wl,--gc-sections

PROJECT_ROOT = $(HOME)/CodeDevelopment/NordicDev
#SDK_PATH = $(PROJECT_ROOT)/sw/sdk/
SDK_PATH = ../../../../../../../../../NORDIC_DEV/nrf51822_sdk/
#TEMPLATE_PATH = $(PROJECT_ROOT)/sw/nrf51-pure-gcc-setup/template/
TEMPLATE_PATH = $(SDK_PATH)nrf51-pure-gcc-setup/template/
#TOOLCHAIN_PATH = $(HOME)/mac-build/bin/
TOOLCHAIN_PATH = $(SDK_PATH)arm-gcc/bin/
#SOFTDEVICE = $(PROJECT_ROOT)/sw/softdevice/s110_nrf51822_6.0.0_softdevice.hex
#SOFTDEVICE = $(SDK_PATH)softdevice/s110_nrf51822_7.1.0_softdevice.hex
SOFTDEVICE = $(SDK_PATH)softdevice/s110_nrf51822_6.0.0_softdevice.hex

CFLAGS = -Os

GDB_PORT_NUMBER = 2331

include $(TEMPLATE_PATH)Makefile
