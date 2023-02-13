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


#for file in $dir*; 
#do
#	echo $file
#	cat $file | ./build/soundings_generate_features $radius >> $outDir/$FILENAME.hackel
#done


for FILE in $outDir*;
do
	echo $FILE
	python3 src/apply-gmm-model.py gmm_trained.model $FILE >> $outDir/gmm-classified.xyzc
	#python3 src/apply-model.py trained.model $FILE >> $outDir/classified.xyzc
done

