mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))
PROJECT_PATH =  $(mkfile_dir)/..

include $(PROJECT_PATH)/common.mk


NODE_HOSTNAME=\"doorknock\"
COMPONENT_CFLAGS += -DNODE_HOSTNAME=$(NODE_HOSTNAME)

SMING_ARCH = Esp8266
SPI_MODE = qio
SPI_SIZE = 4M
COM_SPEED_ESPTOOL = 460800
COM_PORT = /dev/ttyUSB0
COM_OPTS = --raw
COM_SPEED = 460800
COM_SPEED_ESPTOOL = $(COM_SPEED)
COM_SPEED_SERIAL = $(COM_SPEED)
DISABLE_SPIFFS = 1
