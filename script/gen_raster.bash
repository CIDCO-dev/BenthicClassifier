#!/bin/bash

gen_vrt_gbx(){
	classificationsDir=$1
	xyzFile=$2
	vrtDir=$3
	vrtFilename=$4
#	echo "1 " $xyzFile
#	echo "2 " ${vrtFilename%.*}
	output=$vrtDir/$vrtFilename
	echo "create virtual file format = " $output


	echo "<OGRVRTDataSource>
		<OGRVRTLayer name=\"${vrtFilename%.*}\">
		    <SrcDataSource>$classificationsDir$xyzFile</SrcDataSource>
		    <GeometryType>wkbPoint</GeometryType>
		    <GeometryField encoding=\"PointFromColumns\" x=\"x\" y=\"y\" z=\"boosting class\"/>
		</OGRVRTLayer>
	</OGRVRTDataSource>" > $output
	
}

gen_vrt_gmm(){
	classificationsDir=$1
	xyzFile=$2
	vrtDir=$3
	vrtFilename=$4
	echo "1 " $xyzFile
	echo "2 " ${vrtFilename%.*}
	output=$vrtDir/$vrtFilename
	echo "create virtual file format = " $output


	echo "<OGRVRTDataSource>
		<OGRVRTLayer name=\"${vrtFilename%.*}\">
		    <SrcDataSource>$classificationsDir$xyzFile</SrcDataSource>
		    <GeometryType>wkbPoint</GeometryType>
		    <GeometryField encoding=\"PointFromColumns\" x=\"x\" y=\"y\" z=\"gmm class\"/>
		</OGRVRTLayer>
	</OGRVRTDataSource>" > $output
	
}

if [[ $# -ne 3 ]]; then
	echo "usage : bash gen_raster.bash classificationsDirectory resolutionX resolutionY"
	exit 1
fi

if [[ "$(basename $(pwd))" -ne "BenthicClassifier" ]]; then
	echo "please execute script from project root"
	exit 1
fi



dir=$1
vrtGbxDir=$dir/vrts_gbx
vrtGmmDir=$dir/vrts_gmm
rasterGbxDir=$dir/rasters_gbx
rasterGmmDir=$dir/rasters_gmm
mkdir -p $rasterGbxDir
mkdir -p $rasterGmmDir
mkdir -p $vrtGbxDir
mkdir -p $vrtGmmDir

resolutionX=$2
resolutionY=$3
echo 

# gbx
for FILE in $(ls $dir);
do
	vrtFilename=${FILE%.*}.vrt
	echo $FILE
	gen_vrt_gbx $dir $FILE $vrtGbxDir $vrtFilename
	gdal_grid -outsize $resolutionX $resolutionY -a_srs EPSG:2960 -a invdist -of GTiff -ot Float64 -l ${vrtFilename%.*} $vrtGbxDir/$vrtFilename $rasterGbxDir/$FILE.tiff --config GDAL_NUM_THREADS ALL_CPUS

done


# gmm
for FILE in $(ls $dir);
do
	vrtFilename=${FILE%.*}.vrt
	echo $FILE
	gen_vrt_gmm $dir $FILE $vrtGmmDir $vrtFilename
	gdal_grid -outsize $resolutionX $resolutionY -a_srs EPSG:2960 -a invdist -of GTiff -ot Float64 -l ${vrtFilename%.*} $vrtGmmDir/$vrtFilename $rasterGmmDir/$FILE.tiff --config GDAL_NUM_THREADS ALL_CPUS
	
done



