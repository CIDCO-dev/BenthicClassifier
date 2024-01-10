#!/bin/bash

if [[ "$(basename "$PWD")" != "BenthicClassifier" ]]; then
	echo "please execute script from project root"
	exit 1
fi

if [[ $# -ne 2 ]]; then
	echo "usage : bash hackel_batch.bash directory outputDirectory"
	exit 1
fi

dir=$1
outDir=$2

mkdir -p $outDir

for FILE in $(ls $dir/*.csv);
do
	echo $FILE
	FILENAME=$(basename -s .csv $FILE)
	python3 src/generate_raster_map.py $FILE $outDir/$FILENAME.tif $FILENAME
done



