#! /bin/echo Warning: please don't execute me directly.

#change in ".profile" as below:
#	echo -en ".profile:\n\tsource $HOME/scripts/func.sh\n"
#	. $HOME/.scripts/func.sh
#	export SPEECHD_PORT=6561

#batch rename
# for file in *; do num=`echo $file | sed -r 's/^.*\((.*)\).*$/\1/'`;if [ $num -lt 10 ]; then num="0$num";fi; newname=`echo $file|eval sed -r 's/^.*$/IM_000$num\.JPG/'`; mv "$file" "$newname";done;

export REPO_R3=http://10.6.6.67/svn/amadeus/software/R3
export REPO_R5=http://10.6.6.67/svn/amadeus/software/R5
PATH=$PATH:/opt/ba-elf-new/bin

cs()
{
	find . \( -name "*.h" -or -name "*.c" -or -name "*.S" -or -name "*.cpp" \) -and \( -not -type l \) > cscope.files
	cscope -bkq -i cscope.files
	rm cscope.files -rf
	#cscope -Rbkq *
	ctags -R
}

svn_merge()
{
	vers=$1
	if [ -z "$vers" ]; then
		echo "svn_merge <ver> <src> [dst]"
		return 1
	fi

	if echo $vers | grep ':' >/dev/null; then
		vers="-r $vers"
	else
		vers="-c $vers"
	fi

	src=$2
	if [ -z "$src" ]; then
		echo "svn_merge <ver> <src> [dst]"
		return 1
	fi

	dst="."
	if [ -n "$3" ]; then
		dst=$3
	fi

	svn diff $vers $src --summarize >file.list
	sed -r -i "s/^.*${src//\//\\\/}\///" file.list
	vim file.list

	if [ `cat file.list  | sed '/^\s*$/d' | wc -l` -eq 0 ]; then
	    echo "[INFO]: nothing to merge"
		return 0
	fi

	while read file; do
		svn merge $vers $src/$file $dst/$file
	done < file.list

	rm file.list file.list~ -rf
}


svn_clean()
{
	ignore=`echo "$*" | sed -r 's/\/(\s|$)/ /g'`
	if svn info >/dev/null; then 
		rev=`svn info | grep -r 'Rev\>' | sed -r 's/.*: //'`

		if [ -n "`svn status -q --no-ignore`" ]; then
			list=`svn status -q --no-ignore | sed -r  's/(.* )([^ ]+)$/\2/'`
			for item in $list; do
				if echo $ignore | grep -E "(^|[^/])\<$item\>([^/]|$)" >/dev/null; then
					continue
				fi
				svn revert $item
			done
		fi

		if [ -n "`svn status --no-ignore`" ]; then
			list=`svn status --no-ignore | sed -r  's/(.* )([^ ]+)$/\2/'`
			for item in $list; do
				if echo $ignore | grep -E "(^|[^/])\<$item\>([^/]|$)" >/dev/null; then
					continue
				fi
				rm -rf $item
			done
		fi

		find . -name "*~" | xargs rm -rf

		svn up -r $rev >/dev/null

		if [ -z "`svn status --no-ignore`" ]; then
			echo "clean rev$rev successful!" 
		else
			echo "clean rev$rev except:"
			svn status
		fi
	fi
}

##usage: run the script in a pre-prepared SDK codebase.
#
#rm -rf ori.txt change.list not_exist.list
#
#svn diff -r 77978:HEAD $REPO_R5/R5_fw --summarize >ori.txt
#
#while read line
#do
#	type=`echo $line | sed -r 's/(^[[:space:]]*)([MAD])(.*)/\2/'`
#	file=`echo $line | sed -r 's/^[[:space:]]*[[:alpha:]]+.*R5_fw(\/)?//'`
#	if [ -z "$file" ] || [ -z "$type" ]; then
#		echo "[ERROR]: $line"
#		continue
#	fi
#
#	if [ "$type" == "D" ]; then
#		echo "D       $file" >>change.list
#		continue
#	fi
#
#	if [ -e "$file" ]; then
#		echo "$type       $file" >>change.list
#		continue
#	else
#		echo "$file" >>not_exist.list
#	fi
#done <ori.txt

