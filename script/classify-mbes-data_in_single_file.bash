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
mkdir -p $dir/classification

echo x, y, z, boosting class, gmm class > $dir/classification/classified_both_model.csv
for FILE in $dir*.hackel;
do
	echo $FILE
	python3 src/apply_both_models.py trained.model gmm_trained.model $FILE >> $dir/classification/classified_both_model.csv
done

