export ROOT
export INCLUDE_DIRS
#ROOT = $(shell pwd)
ROOT = $(realpath .)
INCLUDE_DIRS = $(ROOT)/include $(ROOT)/api .

# 				src_dir:target_type:INCLUDE_DIRS:EXCLUDE_SRCS:LINK_FLAGS\n
MODULES_TBL = $(ROOT)/api:LIB:::\\n\
			  $(ROOT)/qrcode:LIB:::\\n\
			  $(ROOT)/core:LIB::svg.c:\\n\
			  $(ROOT)/example:EXE:::

all clean:
	set -e;\
	echo -e "$(MODULES_TBL)" >modules.tmp;\
	while read line;\
	do\
		src_dir=`echo $$line | cut -d: -f1`;\
		if ! [ -f "$$src_dir/Makefile" ];\
		then\
			make build;\
		fi;\
		make -C $$src_dir $@;\
		if [ "$(MAKECMDGOALS)" == "clean" ]; then\
			rm -rf libs;\
		fi;\
	done<modules.tmp;\
	rm -rf modules.tmp;

build build_clean:
	set -e;\
	echo -e "$(MODULES_TBL)" >modules.tmp;\
	while read line;\
	do\
		src_dir=`echo $$line | cut -d: -f1`;\
		target_type=`echo $$line | cut -d: -f2`;\
		include_dirs=`echo $$line | cut -d: -f3`;\
		exclude_srcs=`echo $$line | cut -d: -f4`;\
		link_flags=`echo $$line | cut -d: -f4`;\
		rm -rf $$src_dir/Makefile;\
		if [ $(MAKECMDGOALS) == "build_clean" ];\
		then\
			continue;\
		fi;\
		if [ $$target_type == "LIB" ];\
		then\
			target=`echo $$src_dir | sed -r 's/(.*\/)(.+)$$/\2.a/g'`;\
			echo "LIB = $$target" >Makefile.tmp;\
		fi;\
		if [ $$target_type == "EXE" ];\
		then\
			target=`echo $$src_dir | sed -r 's/(.*\/)(.+)$$/\2.out/g'`;\
			echo "EXE = $$target" >Makefile.tmp;\
		fi;\
		echo "INCLUDE_DIRS += $$include_dirs" >>Makefile.tmp;\
		echo "EXCLUDE_SRCS = $$exclude_srcs" >>Makefile.tmp;\
		echo "LINK_FLAGS = $$link_flags" >>Makefile.tmp;\
		echo "include \$$(ROOT)/rules.mk" >>Makefile.tmp;\
		mv Makefile.tmp $$src_dir/Makefile;\
	done<modules.tmp;\
	rm -rf modules.tmp


.PHONY : all clean build build_clean 
