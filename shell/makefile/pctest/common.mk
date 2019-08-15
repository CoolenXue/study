ifndef $(COM_MK)
COM_MK = 1

##################################################################
# Please define following variables and include common.mk in the Makefile for each dir 
#
# 	EXE				:Define EXE target name only in the dir to do final link
#   INCLUDE_DIRS	:The header dirs in format like "-Ipath1 -Ipath2"
#   EXCLUDE_SRCS    :The srcs files in this dir to be excluded from compiling
#   EXTRA_SRCS		:The srcs files in other dir to be included for compiling
#   LD_FLAGS		:Link options
#   CC_FLAGS		:Compile options
#   CC_FORCE		:Force to use this compiler for this dir
#   LD_FORCE		:Force to use this linker for this dir
#   SUBDIRS			:subdirs to compile
##################################################################

#dirs
DIR_OBJS = objs
DIR_EXES = $(ROOT)
DIR_LIBS = $(ROOT)/libs
DIRS = $(DIR_OBJS) $(DIR_EXES) $(DIR_LIBS) 

#cmds
MKDIR = mkdir
RM = rm -rf
LN = ln -s

CROSS_COMPILE =
CC = $(CROSS_COMPILE)gcc -c
LD = $(CROSS_COMPILE)gcc
CPP = $(CROSS_COMPILE)g++ -c
AR = $(CROSS_COMPILE)ar

ifdef CC_FORCE
CC = $(CC_FORCE)
endif
ifdef LD_FORCE
LD = $(LD_FORCE)
endif

CC_OPTS = -g
OPTI_OPTS = 
DEFINE = 
CC_FLAGS += $(CC_OPTS) $(OPTI_OPTS) $(DEFINE) -m32
AR_FLAGS = crs

#inc, link, src and obj
ifneq ($(INCLUDE_DIRS),"")
INCLUDE_DIRS := $(strip $(INCLUDE_DIRS))
endif

SRCS = $(wildcard *.c *.cpp)
SRCS_L = $(notdir $(EXTRA_SRCS))
SRCS += $(SRCS_L)
SRCS := $(filter-out $(EXCLUDE_SRCS), $(shell echo `echo $(SRCS) | sed -r 's/\s/\n/g' | sort | uniq`))	#The "shell" script is to remove repetitive files
SRCS_C = $(filter %.c, $(SRCS))
SRCS_CPP = $(filter %.cpp, $(SRCS))

OBJS_C = $(SRCS_C:%.c=$(DIR_OBJS)/%.o)
OBJS_CPP = $(SRCS_CPP:%.cpp=$(DIR_OBJS)/%.o)

CURDIR = $(shell basename `pwd`)
ifndef IS_SUBDIR
LIB = $(DIR_LIBS)/$(CURDIR).a
else
LIB = ../$(DIR_OBJS)/$(CURDIR).a
endif

ifndef EXE
all: $(DIRS) $(LIB)
else
EXE := $(DIR_EXES)/$(EXE)
all: $(DIRS) $(EXE)
endif

# dep
DIR_DEPS = deps
DEPS_C = $(SRCS_C:%.c=$(DIR_DEPS)/%.dep)
DEPS_CPP = $(SRCS_CPP:%.cpp=$(DIR_DEPS)/%.dep)

ifneq ($(MAKECMDGOALS), clean)
-include $(DEPS_C) $(DEPS_CPP)	# must be after target 'all'
endif

#rules
$(EXE): $(SUBDIRS) $(OBJS_C) $(OBJS_CPP)
	$(LD) $(CC_FLAGS) $(INCLUDE_DIRS)  -o $@ $(PREPOSED_LIBS:%=$(DIR_LIBS)/%.a) $(OBJS_C) $(OBJS_CPP) $(SUBDIRS:%=$(DIR_OBJS)/%.a) $(wildcard $(DIR_LIBS)/*) $(LD_FLAGS) 
$(LIB): $(SUBDIRS) $(OBJS_C) $(OBJS_CPP)
	$(AR) $(AR_FLAGS) $@ $(OBJS_C) $(OBJS_CPP) $(SUBDIRS:%=$(DIR_OBJS)/%.a)

$(OBJS_C): $(SRCS_C)
	$(CC) $(CC_FLAGS) $(INCLUDE_DIRS) -o $@ $(filter $(notdir $(@:%.o=%.c)), $(SRCS_C))
$(OBJS_CPP): $(SRCS_CPP)
	$(CPP) $(CC_FLAGS) $(INCLUDE_DIRS) -o $@ $(filter $(notdir $(@:%.o=%.cpp)), $(SRCS_CPP))

#dir and dep
$(DEPS_C): $(DIR_DEPS) $(SRCS_C)
	$(CC) $(INCLUDE_DIRS) -E -MM $(filter $(notdir $(@:%.dep=%.c)), $(SRCS_C)) | sed -r 's/(.*\.o)/$(DIR_OBJS)\/& $(subst /,\/,$@)/g' > $@

$(DEPS_CPP): $(DIR_DEPS) $(SRCS_CPP)
	$(CPP) $(INCLUDE_DIRS) -E -MM $(filter $(notdir $(@:%.dep=%.cpp)), $(SRCS_CPP)) | sed -r 's/(.*\.o)/$(DIR_OBJS)\/& $(subst /,\/,$@)/g' > $@

$(DIRS) $(DIR_DEPS):
	$(MKDIR) $@

$(SUBDIRS):
	IS_SUBDIR=1 make -C $@ $(MAKECMDGOALS)

$(SRCS_L): $(EXTRA_SRCS)
	if ! [ -f $@ ]; then	\
		$(LN) $(filter %/$@, $^) $@;	\
	fi

clean: $(SUBDIRS)
	$(RM) $(DIR_OBJS) $(DIR_DEPS) $(LIB) $(EXE) $(SRCS_L)
	if [ -d $(DIR_LIBS) ]; then	\
		$(RM) $(DIR_LIBS);	\
	fi

.PHONY: all clean $(EXE) $(LIB) $(SUBDIRS)

endif
