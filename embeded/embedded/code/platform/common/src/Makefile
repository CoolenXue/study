EXE = 
LIB = libcommon.a

INCLUDE_DIRS = \
    $(ROOT)/code/platform/common/inc \
    $(ROOT)/code/platform/arch/x86/simulator/inc \
    $(ROOT)/code/platform/driver/ctrlc/inc \
    $(ROOT)/code/platform/task/common/inc \
    $(ROOT)/code/platform/task/v3/inc \
    $(ROOT)/code/platform/sync/common/inc \
    $(ROOT)/code/platform/sync/v2/inc \
    $(ROOT)/code/platform/device/inc \
    $(ROOT)/code/platform/memory/common/inc \
    $(ROOT)/code/platform/timer/common/inc \

# all the error definition files should be put into ERROR_FILES variable
ERROR_FILES = \
    $(ROOT)/code/platform/common/inc/errmod.h \
    $(ROOT)/code/platform/task/common/inc/errtask.h \
    $(ROOT)/code/platform/sync/common/inc/errsync.h \
    $(ROOT)/code/platform/device/inc/errdev.h \
    $(ROOT)/code/platform/arch/x86/simulator/inc/errclock.h \
    $(ROOT)/code/platform/arch/x86/simulator/inc/errcon.h \
    $(ROOT)/code/platform/driver/ctrlc/inc/errctrlc.h \
    $(ROOT)/code/platform/memory/common/inc/errheap.h \
    $(ROOT)/code/platform/memory/common/inc/errmpool.h \
    $(ROOT)/code/platform/timer/common/inc/errtmr.h \
	
LINK_LIBS = 

include $(BUILD)/c.rule

$(DIR_OBJS)/error.dep: genmark

# for cleaning the errstr.def and genmark files
RMS += errstr.def genmark

genmark: $(DIR_TARGET)/err2str.exe $(ERROR_FILES)
	$(DIR_TARGET)/err2str.exe $(ERROR_FILES) > errstr.def
	@touch genmark
