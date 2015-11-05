#!/bin/sh
#
if [$# -lt 3]; then
cat << HELP
HELP
	exit 0
fi

# get run file name and first param
DEBUGFILE="$1"
FILEPRM="$2"

valgrind --tool=memcheck --leak-check=full $DEBUGFILE $FILEPRM 
