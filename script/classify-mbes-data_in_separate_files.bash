#!/bin/bash

if [[ $# -ne 1 ]]; then
	echo "usage : bash classify-mbes-data.bash hackelDirectory"
	exit 1
fi

if [[ "$(basename $(pwd))" != "BenthicClassifier" ]]; then
	echo "please execute script from project root"
	exit 1
fi


dir=$1
echo $dir
mkdir -p $dir/classifications


for FILE in $dir/*;
do
	fname=$(basename $FILE)
	fbname=${fname%.*}
	#echo $dir/classifications/$fbname.xyzc
	echo x, y, z, boosting class, gmm class > $dir/classifications/$fbname.csv
	#python3 src/apply_both_models.py trained.model gmm_trained.model $FILE >> $dir/classifications/$fbname.csv
	python3 src/apply-model.py trained.model $FILE >> $dir/classifications/$fbname.csv
done

