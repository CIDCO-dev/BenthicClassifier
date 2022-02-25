#!/bin/sh

WORKDIR="../BenthicClassifierData/Lidar"

echo "Starting clean in ${WORKDIR}"
mkdir -p ${WORKDIR}/WithFeatures
rm ${WORKDIR}/WithFeatures/*.txt

for f in `ls ${WORKDIR}/*.txt`
do
echo "Processing $f"
FILENAME=`basename $f`
cat $f | ./build/soundings_generate_features > "${WORKDIR}/WithFeatures/$FILENAME.Hackel"
#TODO: do this in soundings_generate_features
sed -i '/nan/d' "${WORKDIR}/WithFeatures/$FILENAME.Hackel"

done

echo "Generating training data"
python3 ./src/generate-training-data.py ../BenthicClassifierData/labels.csv $WORKDIR/WithFeatures > training-data.txt
