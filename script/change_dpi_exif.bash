#!/bin/bash

# sudo apt install exiftool

if [[ $# -ne 3 ]]; then
	echo "usage : bash gen_raster.bash raster_directory_1 raster_directory_1 DPI"
	exit 1
fi

if [[ "$(basename $(pwd))" -ne "BenthicClassifier" ]]; then
	echo "please execute script from project root"
	exit 1
fi

DPI=$3


for FILE in $(ls $1);
do
	exiftool -XResolution=$DPI -YResolution=$DPI -ResolutionUnit=inches $1/$FILE
done

for FILE in $(ls $2);
do
	exiftool -XResolution=$DPI -YResolution=$DPI -ResolutionUnit=inches $2/$FILE
done
