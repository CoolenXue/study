ifndef $(RULE_MK)
RULE_MK = 1

##################################################################
# Please define following in Makefile
# 	EXE/LIB
#   INCLUDE_DIRS
#   EXCLUDE_SRCS
#   LINK_FLAGS(EXE)
##################################################################

#dirs
DIR_OBJS = objs
DIR_EXES = $(ROOT)
DIR_LIBS = $(ROOT)/libs
DIRS = $(DIR_OBJS) $(DIR_EXES) $(DIR_LIBS) 

#cmds
MKDIR = mkdir
RM = rm -rf

CROSS_COMPILE =
CC = $(CROSS_COMPILE)gcc -c
AR = $(CROSS_COMPILE)ar
LD = $(CROSS_COMPILE)gcc

CC_OPTS = -g
OPTI_OPTS = 
DEFINE = 
CC_FLAGS = $(CC_OPTS) $(OPTI_OPTS) $(DEFINE)
AR_FLAGS = crs

#inc, link, src and obj
ifneq ($(INCLUDE_DIRS),"")
INCLUDE_DIRS := $(strip $(INCLUDE_DIRS))
INCLUDE_DIRS := $(addprefix -I, $(INCLUDE_DIRS))
endif

SRCS = $(wildcard *.c)
SRCS := $(filter-out $(EXCLUDE_SRCS), $(SRCS))
OBJS = $(SRCS:.c=.o)
OBJS := $(addprefix $(DIR_OBJS)/, $(OBJS))

#targets 
ifneq ($(EXE), "")
EXE := $(addprefix $(DIR_EXES)/, $(EXE))
all: $(DIRS) $(EXE)
endif

ifneq ($(LIB), "")
LIB := $(addprefix $(DIR_LIBS)/, $(LIB))
all: $(DIRS) $(LIB)
endif

# dep
DIR_DEPS = deps
DEPS = $(SRCS:.c=.dep)
DEPS := $(addprefix $(DIR_DEPS)/, $(DEPS))

ifneq ($(MAKECMDGOALS), clean)
-include $(DEPS)	# must be after target 'all'
endif

#rules
$(EXE): $(OBJS)
	$(LD) $(LINK_FLAGS) $(CC_FLAGS) $(INCLUDE_DIRS)  -o $@ $^ $(wildcard $(DIR_LIBS)/*.a)
$(LIB): $(OBJS)
	$(AR) $(AR_FLAGS) $@ $^

$(DIR_OBJS)/%.o: %.c 
	$(CC) $(CC_FLAGS) $(INCLUDE_DIRS) -o $@ $(filter %.c, $^)

#dir and dep
$(DIR_DEPS)/%.dep: $(DIR_DEPS) %.c
	$(CC) $(INCLUDE_DIRS) -E -MM $(filter %.c,$^) | sed -r 's/(.*\.o)/$(DIR_OBJS)\/& $(subst /,\/,$@)/g' > $@

$(DIRS) $(DIR_DEPS):
	$(MKDIR) $@
#clean
clean:
	$(RM) $(DIR_OBJS) $(DIR_DEPS) $(EXE) $(LIB)

.PHONY: all clean

endif
