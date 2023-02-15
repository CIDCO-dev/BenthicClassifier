#!/bin/bash

if [[ $# -ne 1 ]]; then
	echo "usage : bash compare_models.bash classificationsDirectory"
	exit 1
fi

if [[ "$(basename $(pwd))" -ne "BenthicClassifier" ]]; then
	echo "please execute script from project root"
	exit 1
fi


dir=$1
#echo $dir
mkdir -p $dir/results


for FILE in $(ls $dir);
do
	fname=$(basename $FILE)
	fbname=${fname%.*}
	#echo $dir/results/$fbname.xyzc
	echo $dir/$FILE
	python3 src/compare-model.py $dir/$FILE > $dir/results/$fbname.csv
done

