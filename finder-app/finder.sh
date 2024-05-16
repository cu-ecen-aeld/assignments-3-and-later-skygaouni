#!/bin/bash

dir="$1"

if [[ $# -lt 2 || ! -d "$dir" ]]; then	
	echo "NOT ENOUGH ARGUMENT or $1 is not a directory "
	exit 1
else
	echo "$1 is a directory"
	number_of_files=$(find $1 -type f | wc -l)
	number_of_text=$(grep -Rnw $1 -e "$2" | wc -l)
	echo "The number of files are $number_of_files and the number of matching lines are $number_of_text"
fi