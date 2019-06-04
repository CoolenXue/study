export ROOT
export PCTEST
export CC_FLAGS
export INCLUDE_DIRS
ROOT = $(realpath .)
PCTEST = 1

#TODO: libvam_xxx modules should be moved later to "$(ROOT)/../../../../AB1_libs/spu/share/libva/"
LIBVAM_DIR = $(ROOT)
VAM_DIR = $(ROOT)/..

VAM_MD = $(LIBVAM_DIR)/libvam_md $(VAM_DIR)/vam_md
VAM_OT = $(LIBVAM_DIR)/libvam_ot $(VAM_DIR)/vam_ot
VAM_SA = $(VAM_DIR)/vam_sa
VAM_LIST = $(VAM_MD) $(VAM_OT) $(VAM_SA)

CC_FLAGS = -DBUILD_FOR_ERTOS -DPCTEST -DBUILD_FOR_SPU
INCLUDE_DIRS = -I$(ROOT)/../../../../common/include -I$(ROOT)/framework -I$(ROOT)/vam_common -I$(ROOT)/.. -I./ $(VAM_LIST:%=-I%) $(VAM_LIST:%=-I%/inc)

#FIXME: some libs need to be preposed during link 
export PREPOSED_LIBS
PREPOSED_LIBS = framework

PRJ ?= smart_alert

all clean: framework vam_common $(PRJ)
	@echo "\nmake $(MAKECMDGOALS) for $(PRJ) is OK!\n"

smart_alert: $(VAM_MD) $(VAM_OT) $(VAM_SA)
	make -C smart_alert $(MAKECMDGOALS)

framework vam_common $(VAM_LIST):
	make -C $@ $(MAKECMDGOALS)

.PHONY: all clean framework vam_common $(VAM_LIST) smart_alert

