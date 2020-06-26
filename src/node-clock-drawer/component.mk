mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))
PROJECT_PATH =  $(mkfile_dir)/..
include $(PROJECT_PATH)/common.mk

NODE_HOSTNAME=\"clock-drawer\"
COMPONENT_CXXFLAGS += -DNODE_HOSTNAME=$(NODE_HOSTNAME)
