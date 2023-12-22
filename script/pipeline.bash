#!/bin/bash

gen_hackel(){
	dir=$1
	outDir=$2
	radius=$3
	bash script/gen_hackel.bash $dir $outDir $radius
}

combine_file(){
	inputDir=$1
	outputFilename=$2
	
	echo 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18 > $outputFilename
	
	for FILE in $(ls $inputDir/*.hackel);
	do
		sed '1d' $FILE >> $outputFilename
	done
}

train_models(){
	mbesTrainingData=$1
	gmmTrainingData=$2
	python3 src/train-model.py $mbesTrainingData
	python3 src/gmm-best-fit.py $gmmTrainingData 7
	# both models are saved in project root
}

classify_data(){
	hackelDirectory=$1
	bash script/classify-mbes-data_in_separate_files.bash $hackelDirectory
}

if [[ $# -ne 3 ]]; then
	echo "usage : bash pipeline.bash mbesInputDir hackelOutputDirectory hackelRadius"
	exit 1
fi

# all paths are relative to project root
if [[ "$(basename $(pwd))" != "BenthicClassifier" ]]; then
	echo "please execute script from project root"
	exit 1
fi


mbesDir=$1
hackelDir=$2
hackelRadius=$3


############ SCRIPT ###################

#gen_hackel $mbesDir $hackelDir $hackelRadius
#combine_file $hackelDir $HOME/allSurfaces.hackel

#train_models mbes-training-data.txt $HOME/allSurfaces.hackel
classify_data $hackelDir







