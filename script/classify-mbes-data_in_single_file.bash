#!/bin/bash

if [[ $# -ne 1 ]]; then
	echo "usage : bash classify-mbes-data.bash hackelDirectory"
	exit 1
fi

if [[ "$(basename $(pwd))" -ne "BenthicClassifier" ]]; then
	echo "please execute script from project root"
	exit 1
fi


dir=$1
echo $dir
mkdir -p $dir/classification

echo x, y, z, boosting class, gmm class > $dir/classification/classified_both_model.xyzc
for FILE in $dir*.hackel;
do
	echo $FILE
	#python3 src/apply-gmm-model.py gmm_trained.model $FILE >> $outDir/gmm-classified.xyzc
	#python3 src/apply-model.py trained.model $FILE >> $outDir/classified.xyzc
	python3 src/apply_both_models.py trained.model gmm_trained.model $FILE >> $dir/classification/classified_both_model.xyzc
done

