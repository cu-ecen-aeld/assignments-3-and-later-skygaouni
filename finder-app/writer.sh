#!/bin/bash

writefile=$1
writestr=$2

if [ ! $# -eq 2 ]
then
  echo "Invalid Argument Number"
  exit 1
fi

filename="${writefile##*/}"
directory_path="${writefile%/*}"

if [ ! -d $directory_path ]
then
  mkdir -p $directory_path
fi

#echo $filename
#echo $directory_path
echo $writestr > $writefile

if [ ! -e $writefile ]
then
  echo "Could not create file"
  exit 1
fi
