include ../common.mk 

NODE_HOSTNAME=\"musicbox-drawer\"
COMPONENT_CFLAGS += -DNODE_HOSTNAME=$(NODE_HOSTNAME)