mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))
PROJECT_PATH =  $(mkfile_dir)/..

include $(PROJECT_PATH)/common.mk

ARDUINO_LIBRARIES += SparkFun_APDS9960

NODE_HOSTNAME=\"clock\"

COMPONENT_CXXFLAGS += -DNODE_HOSTNAME=$(NODE_HOSTNAME)
#COMPONENT_CXXFLAGS += -DVERBOSE

