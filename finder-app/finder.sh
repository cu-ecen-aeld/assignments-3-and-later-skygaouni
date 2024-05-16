#!/bin/bash

if [ $# != 2 ]
then

	echo -e "Invalid parameter, correct format should be:\nfinder.sh [filesdir] [searchstr]"
	exit 1
fi

FILESDIR=$1
SEARCHSTR=$2

if [ -d "${FILESDIR}" ]
then	
	FILESCOUNT=$(find "${FILESDIR}" -type f | wc -l)
	LINESCOUNT=$(grep -r "${SEARCHSTR}" "${FILESDIR}" | wc -l)
	echo "The number of files are ${FILESCOUNT} and the number of matching lines are ${LINESCOUNT}"
else
	echo "${FILESDIR}: directory doesn't exist"
	exit 1	
fi

exit 0