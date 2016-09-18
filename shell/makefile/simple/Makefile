NAME ?= simple

SRC = $(wildcard *.c *.S)
OBJS = $(SRC:%.c=%.o)
OBJS := $(OBJS:%.S=%.o)
OUT = $(NAME).out

ifndef V
Q = @
endif
CC_PRE = $Q

CC = $(CC_PRE)gcc
LD = $(CC_PRE)gcc
NM = $(CC_PRE)nm
OBJDUMP = $(CC_PRE)objdump
OBJCOPY = $(CC_PRE)objcopy

CC_FLAGS = -g -c
LD_FLAGS = -Wl,-Map=$(NAME).map

all: $(OUT)

$(OUT): $(OBJS)
	$(LD) $(LD_FLAGS)	$^	-o $@

$(OBJS):
	$(CC) $(CC_FLAGS)	$< 	-o $@

sinclude .dep
.dep: $(SRC)
	$(CC) -E -MM -MG $^ >$@

clean:
	$(Q)rm -rf $(OBJS) $(OUT) .dep $(NAME).map

ALL_FILES = $(wildcard * .dep)
DIRTY_FILES = $(filter-out $(SRC) Makefile %.h, $(ALL_FILES))

cleanup: 
	$(Q)rm -rf $(DIRTY_FILES)


.PHONY:	all clean cleanup
