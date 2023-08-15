#!/bin/sh

if [ $# != 2 ]; then
    echo "- Please provide the file directory!"
	echo "- Please provide the output file name!"
    exit
fi

fileDir="/eos/cms${1}"
outFile=$2

find $fileDir -type f -name "*.root" > $outFile

# cut the first 8 characters from each line
sed -i 's/^.\{8\}//' $outFile
