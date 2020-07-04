
include $(PROJECT_PATH)/user.mk

SUBMISSION_HTML=\"submission.html\"
UPLOAD_HTML=\"upload.html\"

VERSION := $(shell git rev-parse --verify HEAD)
COMPONENT_CXXFLAGS += -DVERSION=\"$(VERSION)\"

COMPONENT_CXXFLAGS += -DSUBMISSION_HTML=$(SUBMISSION_HTML)
COMPONENT_CXXFLAGS += -DUPLOAD_HTML=$(UPLOAD_HTML)

COMPONENT_SRCDIRS := app app/project ../common
COMPONENT_SRCFILES :=
COMPONENT_INCDIRS := ../common


## SPIFFS options
DISABLE_SPIFFS = 1
CUSTOM_HEAP = 1
ENABLE_ESPCONN = 0
ENABLE_SSL = 0

# SPIFF_FILES = files
ARDUINO_LIBRARIES := ArduinoJson5 MultipartParser OtaUpgrade

ENABLE_OTA_ENCRYPTION = 0
ENABLE_OTA_SIGNING = 0
ENABLE_OTA_ENCRYPTION = 0
ENABLE_OTA_DOWNGRADE = 0
RBOOT_ENABLED   ?= 1
RBOOT_BIG_FLASH ?= 0
RBOOT_TWO_ROMS  ?= 1
SPI_SIZE        ?= 1M

RBOOT_ROM0_ADDR ?= 0x002000
RBOOT_ROM1_ADDR ?= 0x082000

# output file for first rom (.bin will be appended)
RBOOT_ROM_0 ?= rom0
# these next options only needed when using two rom mode
RBOOT_ROM_1 ?= rom1

# # Use minicom as terminal
# TERMINAL = minicom -D $(COM_PORT) -b $(COM_SPEED_SERIAL) /Volumes/case-sensitive/opt/minicom.config

cleancommon:
	rm ../common/*.d; rm ../common/*.o
