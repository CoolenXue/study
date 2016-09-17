#$
#$make -f file targets -Crdkn MACRO='final'
#$


# 0, =, (recursively open macro); :=,(one-time open macro)(can self-ref); +=;  ?=;  $(foo:.c=.o);
# 1, Makefile, "override" to avoid overrided outside   
# 2, "make MACRO='final'" give final value 
# 3, "shell: export"

#~
#~ 0, ifdef, ifeq, ifndef, ifneq;  ifeq[space]();
#~ 1, if, endif;  if, else, else, endif;
#~

all: include_test variable function 

#~
#~#rule#
#~target: prerequisites
#~[TAB]command 		
#~[TAB]command
#~
placeholder1:
placeholder2:
variable: placeholder1	placeholder2
	echo -e "\n############# VARIABLE ##############"
#~
#~1, It creates a new subshell for each command, use ';' to let them run in a same shell, and '\' to merge lines.
#~2, Use '$$' in makefile rule to represent '$' in shell.
#~
	@echo "command1_id: $$$$"
	@echo "command2_id: $$$$"
	@echo "command3_id: $$$$"; echo "command4_id: $$$$";\
	 echo "command5_id: $$$$"

	@echo -e "\$$@ = $@"
	@echo -e "\$$< = $<"
	@echo -e "\$$^ = $^"
	@echo -e "\$$(MAKE) = $(MAKE)"
	@echo -e "\$$(SHELL) = $(SHELL)"
	@echo -e "\$$(MAKECMDGOALS) = $(MAKECMDGOALS)"

function:
	@echo -e "\n############# FUNCTION ##############"
	@echo -e "\$$(abspath . ..):\n\t$(abspath . ..)"
	@echo -e "\$$(realpath . ..):\n\t$(realpath . ..)"
	@echo -e "\$$(addprefix prefix_, one two three):\n\t$(addprefix prefix_, one two three)"
	@echo -e "\$$(addsuffix _suffix, one two three):\n\t$(addsuffix _suffix, one two three)"
	@echo -e "\$$(filter %.s %.c, one.s two.c three.o four.h):\n\t$(filter %.s %.c, one.s two.c three.o four.h)"
	@echo -e "\$$(filter-out zero_%.c, zero_ab.c zero_cd.c one.c two.c):\n\t$(filter-out zero_%.c, zero_ab.c zero_cd.c one.c two.c)"
	@echo -e "\$$(notdir dir1/file1 dir2/file2):\n\t$(notdir dir1/file1 dir2/file2)"
	@echo -e "\$$(patsubst %.c, %.o, sub1.c sub2.c sub3.s sub4.h):\n\t$(patsubst %.c, %.o, sub1.c sub2.c sub3.s sub4.h)"
	@echo -e "\$$(strip foo1  bar1	foo2 bar2):\n\t$(strip foo1  bar1	foo2 bar2)"
	@echo -e "\$$(wildcard ../*):\n\t$(wildcard ../*)"

#1, make will read in all makefiles, consider each 'Makefile' as a goal target and attemp to update it.(First phase before any rule is checked.)
#2??, If any makefile have actually been changed, make starts with a clean slate and reads all the makefiles over again.

#-include inc.mk		#ignore error
ifeq ($(MAKECMDGOALS), clean)
include inc.mk
endif

ifeq ($(MAKECMDGOALS), include_test)
include inc.mk
endif

inc.mk: note.mk	
	@echo -e "Generate $@ depend on $^"
	@echo -e "inc.mk include_test: dep.tmp" > $@ 			#When one target has several rules, only one rule is allowed to have commands. 
	@echo -e "include_test:" >>$@
	@echo -e "\t@echo -e \"include_test is executed!\"" >>$@

dep.tmp:
	@touch dep.tmp
	@echo -e "Generate dep.tmp\n"

clean:
	@rm -rf inc.mk dep.tmp

#avoid conliction with real file with the same name
.PHONY: all clean
