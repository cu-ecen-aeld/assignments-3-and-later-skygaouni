#!/bin/bash

if [ $# != 2 ]
then 
	echo -e "Invalid parameters, correct format should be:\nwriter.sh [writefile] [writestr]"
fi

WRITEFILE=$1
WRITESTR=$2

mkdir -p "$(dirname "${WRITEFILE}")"
echo "${WRITESTR}" > "${WRITEFILE}"

if [ $? != 0 ]
then 
	echo "Failed to create the file"
	exit 1
fi

exit 0