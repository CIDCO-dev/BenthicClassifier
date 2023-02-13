#!/bin/bash

if [[ $# -ne 3 ]]; then
	echo "usage : bash hackel_batch.bash directory outputDirectory radius"
	exit 1
fi

if [[ "$(basename $(pwd))" -ne "BenthicClassifier" ]]; then
	echo "please execute script from project root"
	exit 1
fi


dir=$1
outDir=$2
radius=$3

echo $(ls ./script)

for file in $dir*; do
	head -n 100 $file | ./build/soundings_generate_features $radius >> $outDir/$FILENAME.hackel
done
